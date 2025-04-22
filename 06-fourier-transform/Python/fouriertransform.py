import numpy as np

def FFT(inp_matrix,out_size: list = None):
    
    if out_size is None:
        out_size = inp_matrix.shape
    M = inp_matrix.shape[0]
    N = inp_matrix.shape[1]
    Mfin = out_size[0]
    Nfin = out_size[1]
    
    omegal = np.exp(-1j*2*np.pi/M)
    Wl = np.array([
        [ omegal**(j*k)for j in range(M) ] for k in range(Mfin)
    ])
    omegar = np.exp(-1j*2*np.pi/N)
    Wr = np.array([
        [ omegar**(j*k) for j in range(N) ] for k in range(Nfin)
    ])
    Wr = Wr.T
    return (1/(M*N))*Wl@inp_matrix@Wr

def invFFT(inp_matrix,out_size: list = None):
    
    if out_size is None:
        out_size = inp_matrix.shape
    M = inp_matrix.shape[0]
    N = inp_matrix.shape[1]
    Mfin = out_size[0]
    Nfin = out_size[1]
    
    omegal = np.exp(1j*2*np.pi/M)
    Wl = np.array([
        [ omegal**(j*k)for j in range(M) ] for k in range(Mfin)
    ])
    omegar = np.exp(1j*2*np.pi/N)
    Wr = np.array([
        [ omegar**(j*k) for j in range(N) ] for k in range(Nfin)
    ])
    Wr = Wr.T
    return Wl@inp_matrix@Wr
    
if __name__ == "__main__":
    N = 1000
    A = np.random.normal(loc=1,scale=1,size=(N,N))
    Acomp = A.astype(np.complex128)
    
    C = FFT(Acomp)
    Acomp2 = invFFT(C)

    error_abs = np.abs(Acomp-Acomp2)
    error_rel = np.zeros((N,N))
    for i in range(N):
        for j in range(N):
            error_rel[i,j] = np.abs((Acomp[i,j]-Acomp2[i,j])/Acomp[i,j])
    mean_error_abs = np.mean(error_abs)
    median_error_abs = np.median(error_abs)
    mean_error_rel = np.mean(error_rel)
    median_error_rel = np.median(error_rel)
    print("FFT c2c")
    print(f"mean_error_abs: {mean_error_abs:.3e}")
    print(f"median_error_abs: {median_error_abs:.3e}")
    print(f"mean_error_rel: {mean_error_rel:.3e}")
    print(f"median_error_rel: {median_error_rel:.3e}")
    print("\n")
    
    R = FFT(A)
    A2 = invFFT(R)
    
    error_abs = np.abs(Acomp-Acomp2)
    error_rel = np.zeros((N,N))
    for i in range(N):
        for j in range(N):
            error_rel[i,j] = np.abs((Acomp[i,j]-Acomp2[i,j])/Acomp[i,j])
    mean_error_abs = np.mean(error_abs)
    median_error_abs = np.median(error_abs)
    mean_error_rel = np.mean(error_rel)
    median_error_rel = np.median(error_rel)

    print("FFT r2c")
    print(f"mean_error_abs: {mean_error_abs:.3e}")
    print(f"median_error_abs: {median_error_abs:.3e}")
    print(f"mean_error_rel: {mean_error_rel:.3e}")
    print(f"median_error_rel: {median_error_rel:.3e}")
    print("\n")
    
    print(C[0,0],R[0,0])
        
    