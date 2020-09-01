from ctypes import CDLL, c_uint, c_double, POINTER, pointer
import argparse
from pathlib import Path


from ssapy.networks import ReactionNetwork
from ssapy.dm import dm
from ssapy.nrm import nrm
from ssapy.nrm import nrmdelay


libssa = CDLL("libssa.so")


def wrap2d(array, ctype):
    T = POINTER(ctype * len(array[0])) * len(array)
    a = T()
    for i, row in enumerate(array):
        a[i] = pointer((ctype * len(array[0]))(*row))
    return a


def wrap1d(array, ctype):
    return (ctype * len(array))(*array)


def directmethod(reactionnetwork, T):
    R = wrap2d(reactionnetwork.R, c_uint)
    P = wrap2d(reactionnetwork.P, c_uint)
    X = wrap1d(reactionnetwork.X, c_uint)
    k = wrap1d(reactionnetwork.k, c_double)
    n = c_uint(len(reactionnetwork.X))
    m = c_uint(len(reactionnetwork.R))
    libssa.ssa_dm(R, P, n, m, k, X, c_double(T))


def directmethoddelay(reactionnetwork, T):
    R = wrap2d(reactionnetwork.R, c_uint)
    P = wrap2d(reactionnetwork.P, c_uint)
    I = wrap2d(reactionnetwork.creates, c_uint)
    X = wrap1d(reactionnetwork.X, c_uint)
    k = wrap1d(reactionnetwork.k, c_double)
    n = c_uint(len(reactionnetwork.X))
    m = c_uint(len(reactionnetwork.R))
    libssa.ssa_dmd(R, P, I, n, m, k, X, c_double(T))


def directmethodpy(reactionnetwork, T):
    print(f"TIME {' '.join([s[0] for s in reactionnetwork.species])}")
    dm(reactionnetwork.R, reactionnetwork.P, reactionnetwork.k, reactionnetwork.X, T)


def nextreactionmethod(reactionnetwork, T):
    R = wrap2d(reactionnetwork.R, c_uint)
    P = wrap2d(reactionnetwork.P, c_uint)
    X = wrap1d(reactionnetwork.X, c_uint)
    k = wrap1d(reactionnetwork.k, c_double)
    n = c_uint(len(reactionnetwork.X))
    m = c_uint(len(reactionnetwork.R))
    steps = wrap1d(reactionnetwork.steps, c_uint)
    creates = wrap2d([[0] * len(reactionnetwork.R)] * len(reactionnetwork.R), c_uint)
    libssa.ssa_nrm(R, P, n, m, k, X, steps, creates, creates, c_double(T))


def nextreactionmethoddelay(reactionnetwork, T):
    R = wrap2d(reactionnetwork.R, c_uint)
    P = wrap2d(reactionnetwork.P, c_uint)
    X = wrap1d(reactionnetwork.X, c_uint)
    k = wrap1d(reactionnetwork.k, c_double)
    n = c_uint(len(reactionnetwork.X))
    m = c_uint(len(reactionnetwork.R))
    steps = wrap1d(reactionnetwork.steps, c_uint)
    creates = wrap2d([[0] * len(reactionnetwork.R)] * len(reactionnetwork.R), c_uint)
    libssa.ssa_nrmd(R, P, creates, n, m, k, steps, X, c_double(T))


def nextreactionmethodpy(reactionnetwork, T):
    print(f"TIME {' '.join([s[0] for s in reactionnetwork.species])}")
    nrm(
        reactionnetwork.R,
        reactionnetwork.P,
        reactionnetwork.adjmatrix,
        reactionnetwork.creates,
        reactionnetwork.k,
        reactionnetwork.steps,
        reactionnetwork.X,
        T,
    )


def nextreactionmethoddelaypy(reactionnetwork, T):
    print(f"TIME {' '.join([s[0] for s in reactionnetwork.species])}")
    nrmdelay(
        reactionnetwork.R,
        reactionnetwork.P,
        reactionnetwork.adjmatrix,
        reactionnetwork.creates,
        reactionnetwork.k,
        reactionnetwork.steps,
        reactionnetwork.X,
        T,
    )
