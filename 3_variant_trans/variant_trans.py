from variant_trans import *
import numpy as np
m=Matrix(2,2)
m[0,0]=1.0
m[0,1]=1.0
m[1,0]=1.0
m[1,1]=1.0

print(m)

n=np.array([[1.1, 2.1], [3.1,4.1]])
m2=Matrix(n)

print(m2)
