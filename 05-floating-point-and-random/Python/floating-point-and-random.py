import numpy as np
import numpy.random as r
from time import time

def mysum(vec: list[complex], method: int = 2):
    if method == 1:
        sum = 0
        for ind in range(len(vec)):
            sum += vec[ind]
    elif method == 2:
        sum = np.sum(vec)
    elif method == 3:
        sum = 0
        lost_bits = 0
        for ind in range(len(vec)):
            y = vec[ind] - lost_bits
            t = sum + y
            lost_bits = t-sum-y
            sum = t
    else:
        assert ValueError("The implemented methods for summing are {1 : for loop, 2 : numpy.sum, 3 : KahanSum}")
    return sum

def daxpy(a:float, x:list[float], y:list[float],fnumpy:bool =True):
    N = len(x)
    assert N == len(y), f"x and y must have the same dimension ({N}!={len(y)})"
    print(f"Initiating computation...")
    stime = time()
    if fnumpy:
        x = np.array(x)
        y = np.array(y)
        d = a*x+y
    else:
        d = [a*x[it]+y[it] for it in range(N)]
    print(f"Computation concluded in {time()-stime} seconds!")
    return d

if __name__ == "__main__":
    ## Task a)
    vec = np.array([1.,1e16,1e-16,-0.5],dtype=np.float32)
    sums = [mysum(vec, method = meth) for meth in range(1,4)]
    diffs = []
    for ind1 in range(len(sums)):
        for ind2 in range(ind1):
            diff = np.abs(sums[ind1]-sums[ind2])
            diffs.append(diff)
            if diff != 0:
                print(f"The difference between methods {ind1} and {ind2} is {diff:.3e}.")
                
    ## Task b)
    dims = [int(1e6),int(1e8)]
    a = 1
    mean = 0
    stdv = 1
    
    for N in dims:
        print(f"N = {N}")
        x = r.normal(mean,stdv,N)
        y = r.normal(mean,stdv,N)
        res = daxpy(a,x,y)
        res = np.array(res)
        res_mean = res.mean()
        res_stdv = res.std()
        
        assert np.allclose([res_mean,res_stdv],[0,np.sqrt(2)],atol=1e-2), f"{[res_mean,res_stdv]} != [0,\sqrt{{2}}]"
        print('\n')
    
    