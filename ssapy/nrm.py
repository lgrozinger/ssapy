import operator
from functools import reduce
from numpy.random import exponential, gamma
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
        self.destroys = []

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


def linkreactions(reactions, affects, creates=None):
    m = len(reactions)
    if creates is None:
        creates = [[0] * m for _ in range(m)]
    for i in range(m):
        reactions[i].affects = [r for r in reactions if affects[i][r.number]]
        reactions[i].creates = [r for r in reactions if creates[i][r.number]]
    return reactions


def initialprops(R, X, k, steps=None):
    m = len(k)
    if steps is None:
        steps = [1] * m
    return [h(R[i], X) * k[i] if steps[i] == 1 else 0.0 for i in range(m)]


def heapreactions(reactions, propensities, T):
    q = []
    for i, reaction in enumerate(reactions):
        if propensities[i] > 0.0:
            reaction.tau = exponential(1 / propensities[i])
        else:
            reaction.tau = T
        heappush(q, reaction)
    return q


def nrm(R, P, graph, k, steps, X, T):
    m = len(k)
    props = initialprops(R, X, k)
    reactions = [Entry(i, T, []) for i in range(m)]
    reactions = linkreactions(reactions, graph)
    q = heapreactions(reactions, props, T)

    while q and q[0].tau < T:
        r = q[0]
        t = r.tau
        print(f"{r.tau:.6f} {' '.join([str(x) for x in X])}")
        for i in range(len(R[r.number])):
            X[i] += P[r.number][i] - R[r.number][i]

        for s in r.affects:
            p = h(R[s.number], X) * k[s.number]
            if s.number == r.number and p > 0.0:
                s.tau = exponential(1 / p) + t
            elif p > 0.0 and props[s.number] > 0.0:
                s.tau = (props[s.number] / p) * (s.tau - t) + t
            elif p > 0.0:
                s.tau = exponential(1 / p) + t
            else:
                s.tau = T
            props[s.number] = p
        heapify(q)


def reactionupdate(r, R, X, k, ps, t, T):
    p = h(R[r.number], X) * k[r.number]
    if p > 0.0 and ps[r.number] > 0.0:
        r.tau = (ps[r.number] / p) * (r.tau - t) + t
    elif p > 0.0:
        r.tau = exponential(1 / p) + t
    else:
        r.tau = T
    ps[r.number] = p


def nrmdelay(R, P, graph, creates, k, steps, X, T):
    m = len(k)
    M = [1 if steps[i] == 1 else 0 for i in range(m)]
    props = initialprops(R, X, k)
    reactions = [Entry(i, T, []) for i in range(m)]
    reactions = linkreactions(reactions, graph, creates)
    q = heapreactions(reactions, props, T)

    while q and q[0].tau < T:
        r = q[0]
        t = r.tau

        print(
            f"{r.tau:.6f} {' '.join([str(x) for x in X])} {' '.join([str(m) for m in M])}"
        )

        for i in range(len(R[r.number])):
            X[i] += P[r.number][i] - R[r.number][i]

        for s in r.destroys:
            s.tau = 0.0
            heapify(q)
            heappop(q)
            M[s.number] -= 1

        for s in r.affects:
            if s.number == r.number:
                p = h(R[r.number], X) * k[r.number]
                if p > 0.0:
                    r.tau = exponential(1 / p) + t
                else:
                    r.tau = T
                props[r.number] = p
            else:
                reactionupdate(s, R, X, k, props, t, T)

        for s in r.creates:
            newone = Entry(s.number, T, s.affects, s.creates)
            p = h(R[s.number], X) * k[s.number]
            if p > 0.0:
                newone.tau = gamma(steps[s.number], 1 / p) + r.tau
            props[s.number] = p
            newone.destroys.append(newone)
            M[s.number] += 1
            heappush(q, newone)

        heapify(q)
