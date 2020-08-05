import ply.lex as lex

tokens = (
    'SPECIES',
    'FLOATING',
    'INTEGRAL',
    'TOFROM',
    'EMPTYSET',
    'TO',
    'AT'
    )

t_ignore = ' \t\n'
literals = '+=()'

def t_TOFROM(t):
    r'<=>'
    return t

def t_TO(t):
    r'>'
    return t

def t_AT(t):
    r'(@)|(at)'
    return t

def t_SPECIES(t):
    r'[a-zA-Z][a-zA-Z_0-9]*'
    return t

def t_FLOATING(t):
    r'-?\d+\.\d+'
    t.value = float(t.value)
    return t

def t_INTEGRAL(t):
    r'-?\d+'
    t.value = int(t.value)
    return t

def t_EMPTYSET(t):
    r'_'
    return t

def t_error(t):
    pass

def build():
    return lex.lex()

def read(string):
    reader = lex.lex()
    reader.input(string)

    tokens = []
    token = reader.token()
    while token:
        tokens.append(token)
        token = reader.token()

    return tokens
