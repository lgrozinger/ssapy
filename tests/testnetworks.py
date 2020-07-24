import sys
import unittest

sys.path.append('../ssapy')
import networks

class TestNetworks(unittest.TestCase):

    def testR(self):
        with open('../examples/lotkavolterra.txt') as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [[1, 0], [1, 1], [0, 1]]
            actually = rn.R
            self.assertEqual(expected, actually)

    
    def testP(self):
        with open('../examples/lotkavolterra.txt') as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [[2, 0], [0, 2], [0, 0]]
            actually = rn.P
            self.assertEqual(expected, actually)

    def testX(self):
        with open('../examples/lotkavolterra.txt') as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [1000, 1000]
            actually = rn.X
            self.assertEqual(expected, actually)

    def testk(self):
        with open('../examples/lotkavolterra.txt') as f:
            rn = networks.ReactionNetwork.from_script(f.read())
            expected = [10.0, 0.01, 10.0]
            actually = rn.k
            self.assertEqual(expected, actually)
