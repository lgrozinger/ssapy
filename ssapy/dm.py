import operator
from functools import reduce
from numpy.random import exponential, random, choice


def nCr(n, r):
    r = min(r, n-r)
    if r < 0:
        return 0
    numer = reduce(operator.mul, range(n, n-r, -1), 1)
    denom = reduce(operator.mul, range(1, r+1), 1)
    return numer // denom


def h(R, X):
    return reduce(operator.mul, map(nCr, X, R), 1)


def dm(R, P, k, X, T):
    t = 0.0
    props = [h(r, X) * k[i] for i, r in enumerate(R)]
    a0 = sum(props)
    
    while t < T and a0 > 0.0:
        print(f"{t:.6f} {' '.join(map(str, X))}")
        deltat = exponential(1 / a0)
        r = choice(len(k), p=[ai / a0 for ai in props])

        t += deltat
        for i in range(len(X)):
            X[i] += P[r][i] - R[r][i]

        for i in range(len(props)):
            props[i] = h(R[i], X) * k[i]
        a0 = sum(props)

        
    
