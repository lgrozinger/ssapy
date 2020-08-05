import unittest
import sys
from numpy.random import seed
import filecmp

sys.path.append("../ssapy")
import networks
import ssa


class TestNRM(unittest.TestCase):
    def setUp(self):
        seed(123456)

    def testnrm(self):
        oldstdout = sys.stdout
        with open('/tmp/testnrmresult', 'w') as f:
            sys.stdout = f
            with open('../examples/lotkavolterra.txt') as script:
                rn = networks.ReactionNetwork.from_script(script.read())
                ssa.nextreactionmethodpy(rn, 0.1)
        sys.stdout = oldstdout

        self.assertTrue(filecmp.cmp('/tmp/testnrmresult', '../examples/testnrmresult'))
