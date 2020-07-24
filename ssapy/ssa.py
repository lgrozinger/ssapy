from ctypes import *
import argparse
import pathlib
import os

from networks import ReactionNetwork


cdll.LoadLibrary('./libssa.so')
libssa = CDLL('libssa.so')


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


def nextreactionmethod(reactionnetwork, T):
    R = reactionnetwork.foreignR
    P = reactionnetwork.foreignP
    x = reactionnetwork.foreignx
    k = reactionnetwork.foreignk
    n = reactionnetwork.n
    m = reactionnetwork.m
    steps = reactionnetwork.foreignsteps
    libssa.ssa_nrm(R, P, n, m, k, x, steps, c_double(T))


parser = argparse.ArgumentParser(description='Simulate chemical reaction networks.')
parser.add_argument('filename', help='File contained network specification')
parser.add_argument('maxtime', type=float, help='The maximum simulated time') 
parser.add_argument('--method', '-m',
                    default='dm',
                    choices=['dm', 'nrm'],
                    help='Select the simulation algorithm to use')

methods = {
    'dm': directmethod,
    'nrm': nextreactionmethod
}

if __name__ == '__main__':
    args = parser.parse_args()
    with open(args.filename, 'r') as script:
        rn = ReactionNetwork.from_script(script.read())
        methods[args.method](rn, args.maxtime)

