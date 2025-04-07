
function square_matmul(a::Matrix{T}, b::Matrix{T}) where T
    n = size(a, 1)
    if size(a, 2) != n || size(b, 1) != n || size(b, 2) != n
        throw(ArgumentError("Matrices must be square and of the same size"))
    end
    
    c = zeros(T, n, n)
    for i in 1:n
        for j in 1:n
            for k in 1:n
                c[i, j] += a[i, k] * b[k, j]
            end
        end
    end
    return c
    
end

using LinearAlgebra
using BenchmarkTools

ARRAY_SIZES = [10, 10^2, 10^3, 10^4]
ASSERT_TOLERANCE = 1e-8
RUN_BENCHMARKS = true

if RUN_BENCHMARKS
    # exclude last element for benchmarking
    # to avoid long runtimes
    for n in ARRAY_SIZES[1:end-1]
        A = fill(3., n, n)
        B = fill(7.1, n, n)
        println("Benchmarking square_matmul with n = $n")
        display( @benchmark square_matmul($A, $B) )
        println("Benchmarking built-in matmul with n = $n")
        display( @benchmark $A * $B )
        println("Benchmarking BLAS matmul with n = $n")
        display( @benchmark BLAS.gemm('N', 'N', $A, $B) )
    end
end

for n in ARRAY_SIZES
    println("Testing square_matmul with n = $n")
    A = fill(3., n, n)
    B = fill(7.1, n, n)

    C = square_matmul(A, B)
    println("Testing built-in matmul with n = $n")
    C_bi = A * B

    # check correctness
    expected_result = 3. * 7.1 * n
    @assert all(@. abs(C - expected_result) < ASSERT_TOLERANCE)
    @assert all(@. abs(C_bi - expected_result) < ASSERT_TOLERANCE)

    println("--------------------------------------")
end