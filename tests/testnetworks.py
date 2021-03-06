import sys
import unittest

sys.path.append("../ssapy")
import networks


class TestNetworks(unittest.TestCase):
    def testR(self):
        with open("../examples/lotkavolterra.txt") as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [[1, 0], [1, 1], [0, 1]]
            actually = rn.R
            self.assertEqual(expected, actually)

    def testP(self):
        with open("../examples/lotkavolterra.txt") as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [[2, 0], [0, 2], [0, 0]]
            actually = rn.P
            self.assertEqual(expected, actually)

    def testX(self):
        with open("../examples/lotkavolterra.txt") as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [1000, 1000]
            actually = rn.X
            self.assertEqual(expected, actually)

    def testk(self):
        with open("../examples/lotkavolterra.txt") as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [10.0, 0.01, 10.0]
            actually = rn.k
            self.assertEqual(expected, actually)

    def testV(self):
        with open("../examples/lotkavolterra.txt") as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [[1, 0], [-1, 1], [0, -1]]
            actually = rn.V
            self.assertEqual(expected, actually)

    def testadjmatrix(self):
        with open("../examples/lotkavolterra.txt") as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [[1, 1, 0], [1, 1, 1], [0, 1, 1]]
            actually = rn.adjmatrix
            self.assertEqual(expected, actually)

    def testcreates(self):
        with open('../examples/delaysyntax.txt') as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [[0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0],
                        [0, 1, 0, 0, 0],
                        [0, 0, 0, 0, 0],
                        [0, 0, 0, 1, 0]]
            actually = rn.creates
            self.assertEqual(expected, actually)
