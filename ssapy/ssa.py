from ctypes import CDLL, c_uint, c_double, POINTER, pointer
import argparse
from pathlib import Path


from networks import ReactionNetwork
from dm import dm
from nrm import nrm
from nrm import nrmdelay


sopath = Path(__file__).parent.parent / "libssa.so"
libssa = CDLL(sopath.resolve())


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


def directmethodpy(reactionnetwork, T):
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


def nextreactionmethodpy(reactionnetwork, T):
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


parser = argparse.ArgumentParser(description="Simulate chemical reaction networks.")
parser.add_argument("filename", help="File contained network specification")
parser.add_argument("maxtime", type=float, help="The maximum simulated time")
parser.add_argument(
    "--method",
    "-m",
    default="dm",
    choices=["dm", "nrm", "dmpy", "nrmpy", "nrmdelaypy"],
    help="Select the simulation algorithm to use",
)

methods = {
    "dm": directmethod,
    "nrm": nextreactionmethod,
    "dmpy": directmethodpy,
    "nrmpy": nextreactionmethodpy,
    "nrmdelaypy": nextreactionmethoddelaypy,
}

if __name__ == "__main__":
    args = parser.parse_args()
    with open(args.filename, "r") as script:
        rn = ReactionNetwork.from_script(script.read())
        methods[args.method](rn, args.maxtime)
