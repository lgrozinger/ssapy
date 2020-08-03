import operator
from functools import reduce
from numpy.random import exponential
from heapq import heappush, heapify, heappop


def nCr(n, r):
    r = min(r, n - r)
    if r < 0:
        return 0
    numer = reduce(operator.mul, range(n, n - r, -1), 1)
    denom = reduce(operator.mul, range(1, r + 1), 1)
    return numer // denom


def h(R, X):
    return reduce(operator.mul, map(nCr, X, R), 1)


class Entry:
    def __init__(self, number, tau, affects, creates=[]):
        self.number = number
        self.affects = affects
        self.tau = tau
        self.creates = creates

    def __lt__(self, other):
        return self.tau < other.tau

    def __le__(self, other):
        return self.tau <= other.tau

    def __eq__(self, other):
        return self.tau == other.tau

    def __gt__(self, other):
        return self.tau > other.tau

    def __ge__(self, other):
        return self.tau >= other.tau


def nrm(R, P, graph, k, steps, X, T):
    reactions = [Entry(i, T, []) for i in range(len(R))]
    props = [0] * len(R)
    q = []
    for i, reaction in enumerate(reactions):
        props[i] = h(R[i], X) * k[i]
        if props[i] > 0.0:
            reaction.tau = exponential(1 / props[i])
        else:
            reaction.tau = T

        reaction.affects = [reactions[r] for r in range(len(R)) if graph[i][r]]
        heappush(q, reaction)

    while q and q[0].tau < T:
        reaction = heappop(q)
        tau = reaction.tau
        r = reaction.number
        print(f"{tau:.6f} {' '.join([str(x) for x in X])}")
        for i in range(len(R[r])):
            X[i] = X[i] + P[r][i] - R[r][i]

        for i in range(len(q)):
            x = q[i]
            t = x.tau
            j = x.number
            p = h(R[j], X) * k[j]
            if p > 0.0 and props[j] > 0.0:
                t = (props[j] / p) * (t - tau) + tau
            elif p > 0.0:
                t = (exponential(1 / p) + tau, j)
            else:
                t = T
            x.tau = t
            props[j] = p

        heapify(q)
        props[r] = h(R[r], X) * k[r]
        if props[r] > 0.0:
            reaction.tau = exponential(1 / props[r]) + tau
        else:
            reaction.tau = T
        heappush(q, reaction)
