import rc_lex

import rc_ast as ast
from ply import yacc

tokens = rc_lex.tokens

precedence = (
    ("left", "AND", "OR"),
    ("left", "EQ", "NE"),
    ("left", "LT", "LE", "GT", "GE"),
    ("left", "PLUS", "MINUS"),
    ("left", "MULTIPLY", "DIVIDE"),
    ("right", "UNOT", "UMINUS"),
)


def p_program(p):
    """program : program_item
               | program program_item"""
    if len(p) == 2:
        p[0] = ast.Program()
        p[0].add_child(p[1])
    else:
        p[1].add_child(p[2])
        p[0] = p[1]


def p_program_item(p):
    """program_item : var_decl 
                    | func_decl 
                    | func_def"""
    p[0] = p[1]


def p_var_decl(p):
    """var_decl : type ident SEMI
                | type ident EQUALS expression SEMI"""
    if len(p) == 4:
        p[0] = ast.VarDecl(p[1], p[2])
    else:
        p[0] = ast.VarDecl(p[1], p[2], p[4])


def p_func_decl(p):
    """func_decl : type ident LPAREN func_args RPAREN SEMI"""
    p[0] = ast.FuncDecl(p[1], p[2], p[4])


def p_func_def(p):
    """func_def : type ident LPAREN func_args RPAREN func_body"""
    p[0] = ast.FuncDef(p[1], p[2], p[4], p[6])


def p_func_arg(p):
    """func_arg : type ident"""
    p[0] = ast.FuncArg(p[1], p[2])


def p_func_args(p):
    """func_args : 
                 | func_arg
                 | func_args COMMA func_arg"""
    if len(p) == 1:
        p[0] = ast.FuncArgs()
    elif len(p) == 2:
        p[0] = ast.FuncArgs([p[1]])
    else:
        p[1].add_child(p[3])
        p[0] = p[1]


def p_func_body(p):
    """func_body : statements"""
    p[0] = p[1]


def p_type(p):
    """type : INT 
            | BOOL"""
    p[0] = ast.Type(p[1])


def p_statements(p):
    """statements : statement"""
    p[0] = ast.Statements([p[1]])


def p_statements_block(p):
    """statements : statement_block"""
    p[0] = p[1]


def p_statement_block(p):
    """statement_block : LBRACE RBRACE
                       | LBRACE statement_list RBRACE"""
    if len(p) == 3:
        p[0] = ast.Statements()
    else:
        p[0] = p[2]


def p_statement_list(p):
    """statement_list : statement
                      | statement_list statement"""
    if len(p) == 2:
        p[0] = ast.Statements([p[1]])
    else:
        p[1].add_child(p[2])
        p[0] = p[1]


def p_statement(p):
    """statement : assign_statement
                 | return_statement
                 | if_statement
                 | while_statement
                 | expr_statement
                 | var_decl"""
    p[0] = p[1]


def p_statement_assign(p):
    """assign_statement : ident EQUALS expression SEMI"""
    p[0] = ast.AssignStatement(p[1], p[3])


def p_statement_return(p):
    """return_statement : RETURN expression SEMI"""
    p[0] = ast.ReturnStatement(p[2])


def p_statement_if(p):
    """if_statement : IF LPAREN expression RPAREN statement_block
                    | IF LPAREN expression RPAREN statement_block ELSE statement_block"""
    if len(p) == 6:  # simple if without else
        p[0] = ast.IfStatement(p[3], p[5])
    else:  # if with else
        p[0] = ast.IfStatement(p[3], p[5], p[7])


def p_statement_while(p):
    """while_statement : WHILE LPAREN expression RPAREN statements"""
    p[0] = ast.WhileStatement(p[3], p[6])


def p_statement_expr(p):
    """expr_statement : expression SEMI"""
    p[0] = p[1]


def p_expression(p):
    """expression : binop_expression
                  | unop_expression
                  | comparison_expression
                  | logic_expression
                  | bool_expression
                  | num_expression
                  | group_expression
                  | ident_expression"""
    p[0] = p[1]


def p_expression_binop(p):
    """binop_expression : expression PLUS expression
                        | expression MINUS expression
                        | expression MULTIPLY expression
                        | expression DIVIDE expression"""
    p[0] = ast.BinaryOp(p[1], p[2], p[3])


def p_comparison_expression(p):
    """comparison_expression : expression EQ expression
                             | expression NE expression
                             | expression LT expression
                             | expression GT expression
                             | expression LE expression
                             | expression GE expression"""
    p[0] = ast.Comparison(p[1], p[2], p[3])


def p_expression_logic(p):
    """logic_expression : expression OR expression
                        | expression AND expression"""
    p[0] = ast.Logic(p[1], p[2], p[3])


def p_expression_logic_not(p):
    """unop_expression : NOT expression %prec UNOT"""
    p[0] = ast.UnaryOp(p[1], p[2])


def p_expression_uminus(p):
    """unop_expression : MINUS expression %prec UMINUS"""
    p[0] = ast.UnaryOp(p[1], p[2])


def p_expression_bool(p):
    """bool_expression : TRUE
                       | FALSE"""
    p[0] = ast.BoolConst(p[1])


def p_expression_number(p):
    """num_expression : NUMBER"""
    p[0] = ast.IntConst(p[1])


def p_expression_ident(p):
    """ident_expression : ident"""
    p[0] = ast.IdentifierExp(p[1])


def p_expression_group(p):
    """group_expression : LPAREN expression RPAREN"""
    p[0] = p[2]


def p_ident(p):
    """ident : ID"""
    p[0] = ast.Identifier(p[1])


def p_error(p):
    if p:
        print("Syntax error at '%s'" % p.value)
    else:
        print("Syntax error at EOF")


yacc.yacc()


def parse(code, debug=False):
    return yacc.parse(code, debug=debug)


# visitor = sast.NodeVisitor()

while 1:
    try:
        s = input("> ")
    except EOFError:
        break
    if not s:
        continue

    x = yacc.parse(s, debug=0)
    if x:
        print(x.children)
