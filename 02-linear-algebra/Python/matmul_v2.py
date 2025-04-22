import numpy as np
from time import time
from tqdm import trange
def matmul(a:list[float], b:list[float],fnumpy:bool =True):
    N = len(a)
    print(f"Initiating computation...")
    stime = time()
    if fnumpy:
        a = np.array(a)
        b = np.array(b)
        c = a@b
    else:
        c = [[0 for it in range(N)] for it2 in range(N)]
        for row in trange(N):
            for col in range(N):
                sum = 0
                for it in range(N):
                     sum += a[row][it]*b[it][col]
                c[row][col] = sum
    print(f"Computation concluded in {time()-stime} seconds!")
    return c

if __name__ == "__main__":
    dims = [10,100,10000]
    x_val = 3
    y_val = 7.1
    print("Benchmarking without numpy:\n")
    for N in dims:
        if N == dims[-1]:
            continue
        print(f"N = {N}")
        x = [[x_val for it in range(N)] for it2 in range(N)]
        y = [[y_val for it in range(N)] for it2 in range(N)]
        res = matmul(x,y,fnumpy=False)
        assert all(abs(component - x_val*y_val*N)<1e-6 for row in res for component in row), f"{res[0][0]} != {x_val*y_val*N}"
        print('\n')
    print("Benchmarking with numpy:\n")
    for N in dims:
        print(f"N = {N}")
        x = [[x_val for it in range(N)] for it2 in range(N)]
        y = [[y_val for it in range(N)] for it2 in range(N)]
        res = matmul(x,y)
        assert all(abs(component - x_val*y_val*N)<1e-6 for row in res for component in row), f"{res[0][0]} != {x_val*y_val*N}"
        print('\n')
    