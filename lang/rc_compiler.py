import random
import string
from enum import IntEnum

from rc_ast import *


class CompilerContext:
    class Register:
        def __init(self):
            pass

    def __init__(self):
        self.registers = ["r{}".format(i) for i in range(6)] + [
            "t{}".format(i) for i in range(10)
        ]


class Compiler:
    def __init__(self):
        self.result = ""
        self.indent_count = 0

    def add(self, s):
        self.result += s

    def addline(self, s):
        self.result += "\n{}{}".format(" " * self.indent_count, s)

    def indent(self):
        self.indent_count += 4

    def dedent(self):
        self.indent_count -= 4

    def unique_label(self):
        return "loc_{}".format(
            "".join(random.choices(string.ascii_uppercase + string.digits, k=6))
        )


class ASMCompiler(Compiler):
    def __init__(self):
        super().__init__()

    def emit_label(self, label):
        self.addline(".{}:".format(label))

    def emit_label_ref(self, label):
        self.addline(".{}".format(label))

    def emit_push(self, register):
        self.addline("push  {}".format(register))

    def emit_pop(self, register):
        self.addline("pop  {}".format(register))

    def emit_mov(self, dest, src):
        self.addline("mov  {}, {}".format(dest, src))

    def emit_loadp(self, dest, src, nbytes):
        instr = {1: "loadb_p", 2: "loadw_p", 4: "load_p"}[nbytes]
        self.addline("{}  {}, {}".format(instr, dest, src))

    def emit_storp(self, dest, src, nbytes):
        instr = {1: "storb_p", 2: "storw_p", 4: "stor_p"}[nbytes]
        self.addline("{}  {}, {}".format(instr, dest, src))

    def emit_lcons(self, dest, val, nbytes):
        instr = {1: "lconsb", 2: "lconsw", 4: "lcons"}[nbytes]
        self.addline("{}  {}, {}".format(instr, dest, val))

    def emit_jmp(self, dest):
        self.addline("jmp  .{}".format(dest))

    def emit_jz(self, dest, val):
        self.addline("jz  {}, .{}".format(val, dest))

    def emit_jnz(self, dest, val):
        self.addline("jnz  {}, .{}".format(val, dest))

    def emit_call(self, dest):
        self.addline("call  .{}".format(dest))

    def emit_ret(self):
        self.addline("ret")

    def emit_artihmetic(self, op, dest, x, y, unsigned=False):
        instr = {
            "+": "add",
            "-": "sub",
            "*": "mul" if unsigned else "imul",
            "/": "div" if unsigned else "idiv",
            "<<": "shl",
            ">>": "shr" if unsigned else "ishr",
            "%": "mod" if unsigned else "imod",
            "&": "and",
            "|": "or",
            "^": "xor",
        }[op]
        self.addline("{}  {}, {}, {}".format(instr, dest, x, y))

    def emit_comparison(self, op, dest, x, y, unsigned=False):
        instr = {
            "==": "je",
            "!=": "jne",
            ">": "ja" if unsigned else "jg",
            ">=": "jae" if unsigned else "jge",
            "<": "jb" if unsigned else "jl",
            ">=": "jbe" if unsigned else "jle",
        }[op]

        true_label = self.unique_label()
        end_label = self.unique_label()
        
        self.addline("{}  {}, {}, .{}".format(instr, x, y, true_label))
        self.emit_lcons(dest, 0, 1)
        self.emit_jmp(end_label)
        self.emit_label(true_label)
        self.emit_lcons(dest, 1, 1)
        self.emit_label(end_label)

    def emit_func_init(self):
        for i in (0, 1, 5):
            self.emit_push("r{}".format(i))
        self.emit_push("ra")
        self.emit_push("bp")
        self.emit_mov("bp", "sp")

    def emit_func_cleanup(self):
        self.emit_pop("bp")
        self.emit_pop("ra")
        for i in (5, 1, 0):
            self.emit_pop("r{}".format(i))

    def emit_func_return(self, val):
        self.emit_mov("t0", val)
        self.emit_func_cleanup()
        #self.emit_ret()
        # for now...
        self.addline("printi t0, 1")
        self.addline("halt")


class ASMCompileVisitor(ASMCompiler):
    def child_accept(self, parent, child):
        child.parent = parent
        child.accept(self)

    def visit_Program(self, node):
        self.emit_jmp("main")
        for c in node.items:
            self.child_accept(node, c)

    def visit_VarDecl(self, node):
        pass

        # self.addline("")
        # self.child_accept(node, node.type)
        # self.add(" ")
        # self.child_accept(node, node.ident)
        # self.add(";")

    def visit_FuncDef(self, node):
        # prologue
        self.emit_label(node.ident.name)
        self.indent()
        self.emit_func_init()

        # allocate space for locals
        locals_size = node.scope.stack_offset
        if locals_size > 0:
            self.emit_lcons("r1", locals_size, 2)
            self.emit_artihmetic("-", "sp", "sp", "r1")

        # TODO: args and return handling

        # body
        self.child_accept(node, node.body)

        # epilogue
        self.emit_lcons("r0", 0, 1)
        self.emit_func_return("r0")
        self.dedent()

    def visit_FuncArg(self, node):
        self.child_accept(node, node.type)
        self.add(" ")
        self.child_accept(node, node.ident)

    def visit_FuncArgs(self, node):
        self.add("(")
        for i in range(len(node.args)):
            if i > 0:
                self.add(", ")
            self.child_accept(node, node.args[i])
        self.add(")")

    def visit_StatementBlock(self, node):
        self.child_accept(node, node.statements)

    def visit_Statements(self, node):
        for s in node.statements:
            self.child_accept(node, s)

    def visit_AssignStatement(self, node):
        self.child_accept(node, node.value)
        self.emit_lcons("r5", node.symbol.offset, 2)
        self.emit_artihmetic("-", "r5", "bp", "r5")
        self.emit_storp("r5", "r0", node.symbol.type_size())

    def visit_ReturnStatement(self, node):
        self.child_accept(node, node.expr)
        self.emit_func_return("r0")

    def visit_IfStatement(self, node):
        else_label = self.unique_label()
        end_label = self.unique_label()

        self.child_accept(node, node.condition)
        self.emit_jz(else_label, "r0")
        self.child_accept(node, node.true_block)
        if node.else_block:
            self.emit_jmp(end_label)
        self.emit_label(else_label)
        if node.else_block:
            self.child_accept(node, node.else_block)
            self.emit_label(end_label)

    def visit_WhileStatement(self, node):
        start_label = self.unique_label()
        end_label = self.unique_label()

        self.emit_label(start_label)
        self.child_accept(node, node.condition)
        self.emit_jz(end_label, "r0")
        self.child_accept(node, node.body)
        self.emit_jmp(start_label)
        self.emit_label(end_label)

    def visit_UnaryOp(self, node):
        self.add(node.op)
        self.child_accept(node, node.right)

    def visit_BinaryOp(self, node):
        self.child_accept(node, node.left)
        self.emit_push("r0")
        self.child_accept(node, node.right)
        self.emit_pop("r1")
        self.emit_artihmetic(node.op, "r0", "r1", "r0")

    def visit_ComparisonOp(self, node):
        self.child_accept(node, node.left)
        self.emit_push("r0")
        self.child_accept(node, node.right)
        self.emit_pop("r1")
        self.emit_comparison(node.comp, "r0", "r1", "r0")

    def visit_LogicOp(self, node):
        self.child_accept(node, node.left)
        self.emit_push("r0")
        self.child_accept(node, node.right)
        self.emit_pop("r1")

        self.child_accept(node, node.right)

    def visit_IntConst(self, node):
        self.emit_lcons("r0", node.value, 4)

    def visit_Identifier(self, node):
        pass

    def visit_IdentifierExp(self, node):
        self.emit_lcons("r5", node.symbol.offset, 2)
        self.emit_artihmetic("-", "r5", "bp", "r5")
        self.emit_loadp("r0", "r5", node.symbol.type_size())

    def visit_ExpGroup(self, node):
        self.child_accept(node, node.expression)

    def visit_Type(self, node):
        self.add(node.name)
