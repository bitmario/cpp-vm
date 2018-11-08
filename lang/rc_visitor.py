class PrintVisitor:
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

    def visit_Program(self, node):
        for c in node.items:
            c.accept(self)

    def visit_VarDecl(self, node):
        self.addline("")
        node.type.accept(self)
        self.result += " "
        node.ident.accept(self)
        self.result += ";"

    def visit_FuncDecl(self, node):
        self.addline("")
        node.rtype.accept(self)
        self.result += " "
        node.ident.accept(self)
        self.result += " "
        node.args.accept(self)
        self.result += ";"

    def visit_FuncDef(self, node):
        self.addline("")
        node.rtype.accept(self)
        self.result += " "
        node.ident.accept(self)
        self.result += " "
        node.args.accept(self)
        self.result += " {"
        self.indent()
        node.body.accept(self)
        self.dedent()
        self.addline("}")

    def visit_FuncArg(self, node):
        node.type.accept(self)
        self.result += " "
        node.ident.accept(self)

    def visit_FuncArgs(self, node):
        self.result += "("
        for a in node.args:
            a.accept(self)
            self.result += ", "
        self.result += ")"

    def visit_Statements(self, node):
        for s in node.statements:
            s.accept(self)

    def visit_AssignStatement(self, node):
        self.addline("")
        node.ident.accept(self)
        self.result += " = "
        node.value.accept(self)
        self.result += ";"

    def visit_ReturnStatement(self, node):
        self.addline("return ")
        node.expr.accept(self)

    def visit_IfStatement(self, node):
        self.addline("if (")
        node.condition.accept(self)
        self.result += ") {"
        self.indent()
        node.true_block.accept(self)
        self.dedent()
        self.addline("}")
        if node.else_block:
            self.result += " else {"
            self.indent()
            node.else_block.accept(self)
            self.dedent()
            self.addline("}")

    def visit_WhileStatement(self, node):
        self.addline("")
        self.result += "while ("
        node.condition.accept(self)
        self.result += ") {"
        self.indent()
        node.body.accept(self)
        self.dedent()
        self.addline("}")

    def visit_UnaryOp(self, node):
        self.result += node.op
        node.right.accept(self)

    def visit_BinaryOp(self, node):
        node.left.accept(self)
        self.result += node.op
        node.right.accept(self)

    def visit_ComparisonOp(self, node):
        node.left.accept(self)
        self.result += node.comp
        node.right.accept(self)

    def visit_LogicOp(self, node):
        node.left.accept(self)
        self.result += node.op
        node.right.accept(self)

    def visit_BoolConst(self, node):
        self.result += str(node.value)

    def visit_IntConst(self, node):
        self.result += str(node.value)

    def visit_Identifier(self, node):
        self.result += str(node.name)

    def visit_IdentifierExp(self, node):
        node.identifier.accept(self)

    def visit_Type(self, node):
        self.result += node.name
