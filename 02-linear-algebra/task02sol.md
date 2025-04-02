# Task 02 - Computational Hello World

## 1. Daxpy
**Task:** Write a code to perform the linear algebra operation $\vec{d}=a\vec{x}+\vec{y}$, for different vector sizes $N=10,10^6,10^8$.
### 1.1. Python implementation
```python
import numpy as np
from time import time
def daxy(a:float, x:list[float], y:list[float],fnumpy:bool =True):
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
    dims = [10,int(1e6),int(1e8)]
    a = 3
    x_val = 0.1
    y_val = 7.1
    print("Benchmarking without numpy:\n")
    for N in dims:
        print(f"N = {N}")
        x = [x_val for it in range(N)]
        y = [y_val for it in range(N)]
        res = daxy(a,x,y,fnumpy=False)
        assert all(abs(component - 7.4)<1e-6 for component in res), f"{res[0]} != 7.4"
        print('\n')
    print("Benchmarking with numpy:\n")
    for N in dims:
        print(f"N = {N}")
        x = [x_val for it in range(N)]
        y = [y_val for it in range(N)]
        res = daxy(a,x,y)
        assert all(abs(component - 7.4)<1e-6 for component in res), f"{res[0]} != 7.4"
        print('\n')
```
### 1.2. C++ implementation   
```cpp
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
using namespace std;

vector<float> daxy(float a, const vector<float>& x, const vector<float>& y){
    int vecSize = x.size();
    vector<float> d(vecSize);

    auto start = chrono::high_resolution_clock::now();
    cout << "Initiating computation...\n";
    for (int it = 0; it < vecSize; it++){
        d[it] = a*x[it]+y[it];
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Computation concluded in " << duration.count() << " seconds.\n";
    return d;
}
bool allEqual(const vector<float> array, float val, float tol = 0){
    int N = array.size();
    for (int it = 0;it<N; it++){
        if (abs(array[it]-val)>tol){
            return false;
        }
    }
    return true;
}
int main(){
    const int a = 3;
    const float x_val = 0.1;
    const float y_val = 7.1;
    int dims[3] = {10,static_cast<int>(1e6),static_cast<int>(1e8)};
    float exp_res = x_val+y_val;

    for (int it = 0;it<3;it++){
        cout << "N = " << dims[it] << "\n";
        vector<float> x(dims[it], x_val);
        vector<float> y(dims[it], y_val);
        vector<float> res = daxy(a,x,y);
        if (!allEqual(res,exp_res,1e-6)){
            cout << "The operation has not been properly computed.\n";
            break;
        }
    }
```
## 2. Matrix multiplication
**Task:** Write a code to perform a matrix multiplication $C=AB$ for $N\times N$ matrices with $N=10,100,10000$.

### 2.1. Python implementation
```python
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
```
### 2.2. C++ implementation
```cpp
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
using namespace std;

vector<vector<double>> matmul(const vector<vector<double>>& A, const vector<vector<double>>& B){
    int vecSize = A.size();
    vector<vector<double>> C(vecSize,vector<double>(vecSize,0.0f));

    auto start = chrono::high_resolution_clock::now();
    cout << "Initiating computation...\n";
    for (int row = 0; row < vecSize; row++){
        if (row % (vecSize/100) == 0){
            cout << "|";
        }
        for (int col = 0; col < vecSize; col++){
            double sum = 0;
            for(int k = 0; k < vecSize; k++){
                sum += A[row][k]*B[k][col];
            }
            C[row][col] = sum;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "\nComputation concluded in " << duration.count() << " seconds.\n";
    return C;
}
bool allEqual(const vector<vector<double>> array, double val, double tol = 0){
    int N = array.size();
    for (int it = 0;it<N; it++){
        for (int it2 = 0; it2<N;it2++){
            if (abs(array[it][it2]-val)>tol){
                return false;
            }
        }

    }
    return true;
}
int main(){
    const double x_val = 3;
    const double y_val = 7.1;
    int dims[3] = {10,static_cast<int>(1e2),static_cast<int>(1e4)};
    double exp_res = x_val*y_val;

    for (int it = 0;it<3;it++){
        cout << "N = " << dims[it] << "\n";
        vector<vector<double>> x(dims[it], vector<double>(dims[it],x_val));
        vector<vector<double>> y(dims[it], vector<double>(dims[it],y_val));
        vector<vector<double>> res = matmul(x,y);
        if (!allEqual(res,exp_res*dims[it],1)){
            cout << "The operation has not been properly computed.\n";
            break;
        }
    }
    


    return 0;
}
```
## 3. Questions
**I. Did you find any problems in running the codes for some N. If so, do you
have an idea why?**

The only problem we encountered was when runing the matrix multiplication for $N=10000$. In fact, the only way to finsih the computation was in the Python implementation when using the `numpy` library. That is given due to the fact that the naive implementation of the matrix multiplication is $O(N^3)$, whereas `numpy`has implemented a more optimized algorithm. 

**II. Where you able to test correctly the sum and product of points 1-3? If so,
how? If not, what was the problem?**

Yes. Given that we need to compare two `float`, we have defined some tolerance and checked for each element of the array that the absolute difference between the obtained result and the expected one is below that tolerance.



