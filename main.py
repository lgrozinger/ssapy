from ctypes import CDLL, c_uint, c_double, POINTER, pointer
import argparse


from ssapy.networks import ReactionNetwork
import ssapy.ssa as ssa


methods = {
    "dm": ssa.directmethod,
    "dmd": ssa.directmethoddelay,
    "nrm": ssa.nextreactionmethod,
    "nrmd": ssa.nextreactionmethoddelay,
    "dmpy": ssa.directmethodpy,
    "nrmpy": ssa.nextreactionmethodpy,
    "nrmdelaypy": ssa.nextreactionmethoddelaypy,
}


parser = argparse.ArgumentParser(description="Simulate chemical reaction networks.")
parser.add_argument("filename", help="File containing network specification")
parser.add_argument("maxtime", type=float, help="The maximum simulated time")
parser.add_argument(
    "--method",
    "-m",
    default="dm",
    choices=list(methods.keys()),
    help="Select the simulation algorithm to use",
)



if __name__ == "__main__":
    args = parser.parse_args()
    with open(args.filename, "r") as script:
        rn = ReactionNetwork.from_script(script.read())
        methods[args.method](rn, args.maxtime)
