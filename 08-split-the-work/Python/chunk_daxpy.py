import numpy as np
from time import time
def daxpy(a:float, x:list[float], y:list[float],fnumpy:bool =True, chunk_size: int = None):
    
    N = len(x)
    assert N == len(y), f"x and y must have the same dimension ({N}!={len(y)})"
    if chunk_size is None:
        chunk_size = N
    print(f"Initiating computation...")
    stime = time()
    n_chunks = N // chunk_size + 1
    if fnumpy:
        d_chunks = []
        for chunk in range(n_chunks):
            start = chunk * chunk_size
            end = min(start + chunk_size, N)
            x_chunk = x[start:end]
            y_chunk = y[start:end]
            d_chunks.append(a * np.array(x_chunk) + np.array(y_chunk))
        d = np.concatenate(d_chunks)
    else:
        d = []
        for chunk in range(n_chunks):
            start = chunk * chunk_size
            end = min(start + chunk_size, N)
            if start < N:
                d.extend([a*x[it]+y[it] for it in range(start, end)])
    print(f"Computation concluded in {time()-stime} seconds!")
    return d

if __name__ == "__main__":
    dims = [10,int(1e6),int(1e8)]
    a = 3
    x_val = 0.1
    y_val = 7.1
    print("Benchmarking without numpy:\n")
    for N in dims:
        print(f"N = {N}")
        x = [x_val for it in range(N)]
        y = [y_val for it in range(N)]
        res = daxpy(a,x,y,fnumpy=False, chunk_size= 8)
        assert all(abs(component - 7.4)<1e-6 for component in res), f"{res[0]} != 7.4"
        print('\n')
    print("Benchmarking with numpy:\n")
    for N in dims:
        print(f"N = {N}")
        x = [x_val for it in range(N)]
        y = [y_val for it in range(N)]
        res = daxpy(a,x,y, chunk_size = 8)
        assert all(abs(component - 7.4)<1e-6 for component in res), f"{res[0]} != 7.4"
        print('\n')
    