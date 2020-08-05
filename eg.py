from ssapy import networks
from ssapy import ssa


with open("examples/lotkavolterra.txt") as script:
    rn = networks.ReactionNetwork.from_script(script.read())

ssa.nextreactionmethodpy(rn, 1.0)
