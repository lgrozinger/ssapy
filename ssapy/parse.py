import ply.yacc as yacc


from ssapy.read import tokens
from ssapy.read import build


def p_exprs(p):
    """exprs : expr exprs"""
    p[0] = [p[1]] + p[2]


def p_exprsempty(p):
    """exprs : empty"""
    p[0] = []


def p_expr(p):
    """expr : reaction
            | assignment"""
    p[0] = p[1]


def p_singlestepreaction(p):
    """reaction : stoich TO stoich AT FLOATING"""
    p[0] = (p[1], p[3], float(p[5]), 1)


def p_multistepreaction(p):
    """reaction : stoich INTEGRAL TO stoich AT FLOATING"""
    p[0] = (p[1], p[4], float(p[6]), int(p[2]))


def p_reactiondbl(p):
    """reaction : stoich TOFROM stoich AT FLOATING"""
    pass


def p_stoich(p):
    """stoich : stoichelem '+' stoich"""
    p[0] = [p[1]] + p[3]


def p_stoich1(p):
    """stoich : stoichelem"""
    p[0] = [p[1]]


def p_stoich0(p):
    """stoich : EMPTYSET"""
    p[0] = []


def p_stoichreaction(p):
    """stoichelem : '(' reaction ')'"""
    p[0] = p[2]


def p_stoichspecies(p):
    """stoichelem : SPECIES"""
    p[0] = p[1]


def p_assignment(p):
    """assignment : SPECIES '=' INTEGRAL"""
    p[0] = (p[1], p[3])


def p_empty(p):
    """empty :"""
    pass


def parse(string):
    parser = yacc.yacc()
    return parser.parse(string, lexer=build())
