import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..','..', '02-linear-algebra','Python')))

import unittest

from daxpy_v2 import daxpy

class TestDaxpy(unittest.TestCase):
    def test_daxpy(self):
        a = 3
        x_val = 0.1
        y_val = 7.1
        dims = [10, int(1e3), int(1e5)]
        tol = 1e-6
        for N in dims:
            x = [x_val for _ in range(N)]
            y = [y_val for _ in range(N)]
            res = daxpy(a, x, y, fnumpy=False)
            self.assertTrue(all([abs(component - (a*x_val+y_val)) < tol for component in res]), f"{res[0]} != {a*x_val+y_val}")
        
        for N in dims:
            x = [x_val for _ in range(N)]
            y = [y_val for _ in range(N)]
            res = daxpy(a, x, y)
            self.assertTrue(all([abs(component - (a*x_val+y_val)) < tol for component in res]), f"{res[0]} != {a*x_val+y_val}")