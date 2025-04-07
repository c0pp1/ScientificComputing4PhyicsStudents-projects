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