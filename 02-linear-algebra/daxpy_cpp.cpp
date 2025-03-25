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
    float exp_res = 7.4;

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
    


    return 0;
}