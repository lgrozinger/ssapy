import ply.yacc as yacc

from read import tokens
from read import build

def p_exprs(p):
    '''exprs : expr exprs'''
    p[0] = [p[1]] + p[2]

def p_exprsempty(p):
    '''exprs : empty'''
    p[0] = []

def p_expr(p):
    '''expr : reaction
            | assignment'''
    p[0] = p[1]

def p_singlestepreaction(p):
    '''reaction : specieslist TO specieslist AT FLOATING'''
    p[0] = (p[1], p[3], float(p[5]), 1)

def p_multistepreaction(p):
    '''reaction : specieslist INTEGRAL TO specieslist AT FLOATING'''
    p[0] = (p[1], p[4], float(p[6]), int(p[2]))

def p_reactiondbl(p):
    '''reaction : specieslist TOFROM specieslist AT FLOATING'''
    pass

def p_specieslist(p):
    '''specieslist : specieslist '+' SPECIES'''
    p[0] = p[1] + [p[3]]

def p_specieslist_1(p):
    '''specieslist : SPECIES'''
    p[0] = [p[1]]

def p_specieslist_empty(p):
    '''specieslist : EMPTYSET'''
    p[0] = []

def p_assignment(p):
    '''assignment : SPECIES '=' INTEGRAL'''
    p[0] = (p[1], p[3])

def p_empty(p):
    '''empty :'''
    pass

def parse(string):
    parser = yacc.yacc()
    return parser.parse(string, lexer=build())
        
