import pickle
from pybind_pickle import *

p = Pickleable("test_value")
p.setExtra(15)
data = pickle.dumps(p, 2)