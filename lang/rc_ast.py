class Node:
    def __init__(self, children=None):
        self.parent = None
        if children:
            for c in children:
                c.parent = self
            self.children = children
        else:
            self.children = []

    def add_child(self, node):
        node.parent = self
        self.children.append(node)

    def add_children(self, nodes):
        for n in nodes:
            n.parent = self
        self.children.extend(nodes)

    def eval(self):
        return [child.eval() for child in self.children]


class Program(Node):
    def __init__(self, children=None):
        super().__init__(children)


class VarDecl(Node):
    def __init__(self, _type, ident, value=None):
        super().__init__((_type, ident, value))


class FuncDecl(Node):
    def __init__(self, rtype, ident, args):
        super().__init__((rtype, ident, args))


class FuncDef(Node):
    def __init__(self, rtype, ident, args, body):
        super().__init__((rtype, ident, args, body))


class FuncArg(Node):
    def __init__(self, _type, ident):
        super().__init__((_type, ident))


class FuncArgs(Node):
    def __init__(self, args=None):
        super().__init__(args)


class Statements(Node):
    def __init__(self, statements=None):
        super().__init__(statements)


class AssignStatement(Node):
    def __init__(self, ident, value):
        super().__init__((ident, value))


class ReturnStatement(Node):
    def __init__(self, value):
        super().__init__((value,))


class IfStatement(Node):
    def __init__(self, condition, true_block, else_block=None):
        super().__init__((condition, true_block, else_block))


class WhileStatement(Node):
    def __init__(self, condition, true_block):
        super().__init__((condition, true_block))


class UnaryOp(Node):
    def __init__(self, op, right):
        super().__init__((right,))
        self.op = op


class BinaryOp(Node):
    def __init__(self, left, op, right):
        super().__init__((left, right))
        self.op = op


class Comparison(Node):
    def __init__(self, left, comp, right):
        super().__init__((left, right))
        self.comp = comp


class Logic(Node):
    def __init__(self, left, op, right):
        super().__init__((left, right))
        self.op = op


class BoolConst(Node):
    def __init__(self, value):
        super().__init__()
        self.value = True if value == "true" else False


class IntConst(Node):
    def __init__(self, value):
        super().__init__()
        self.value = int(value)


class Identifier(Node):
    def __init__(self, name):
        super().__init__()
        self.name = name


class IdentifierExp(Node):
    def __init__(self, identifier):
        super().__init__((identifier,))


class Type(Node):
    def __init__(self, name):
        super().__init__()
        self.name = name

# class For(Node):
#     def __init__(self, assignement, max_, body):
#         self.type = "for"
#         self.assignement = assignement
#         self.max = max_
#         self.body = body

#     def children(self):
#         return (self.assignement, self.max, self.body)

# class FuncCall(Node):
#     def __init__(self, identifier, args):
#         self.type = "funccall"
#         self.identifier = identifier
#         self.args = args

#     def children(self):
#         return tuple([self.type] + self.args)
