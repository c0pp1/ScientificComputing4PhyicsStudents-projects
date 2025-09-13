# julia has its own way to do multithreading, therefore we will use that instead of omp
using Base.Threads

function daxpy(a::Float64, px::Vector{Float64}, py::Vector{Float64})
    # get the length of the vector
    n = length(px)
    # check if the vectors are the same length
    if n != length(py)
        throw(ArgumentError("Vectors must have the same length"))
    end
    # return on trivial cases
    if n <= 0 || a == 0.0
        return
    end

    @. py += a * px

end

function daxpy_threaded(a::Float64, px::Vector{Float64}, py::Vector{Float64})
    # get the length of the vector
    n = length(px)
    # check if the vectors are the same length
    if n != length(py)
        throw(ArgumentError("Vectors must have the same length"))
    end
    # return on trivial cases
    if n <= 0 || a == 0.0
        return
    end
    @threads for i in 1:n
        py[i] += a * px[i]
    end
end

function partial_sum_threaded(px::Vector{Float64})
    n = length(px)
    if n <= 0
        return 0.0
    end

    chunk_size = floor(Int64, n / nthreads())
    remainder = n % nthreads()

    partial_sums = zeros(nthreads()+1)
    if remainder > 0
        partial_sums[end] = sum(px[end-remainder+1:end])  # handle remainder
    end
    @threads for i in 1:nthreads()
        partial_sums[i] = sum(px[(i-1)*chunk_size+1:i*chunk_size])
    end
    return sum(partial_sums)
end


ARRAY_SIZES = [10, 10^6, 10^8]
ASSERT_TOLERANCE = 1e-10
a = 3.
println("Using $(nthreads()) threads")
for n in ARRAY_SIZES
    println("Array size: $n")
    px = fill(0.1, n)
    py = fill(7.1, n)
    print("\tDAXPY multithreaded")
    @time daxpy_threaded(a, px, py)# assert that the result is correct
    @assert all(abs.(py .- (a*0.1 + 7.1)) .< ASSERT_TOLERANCE) "Got $(py[1]) expected $(a*0.1 + 7.1)"
    print("\tDAXPY")
    py = fill(7.1, n)
    @time daxpy(a, px, py)

    print("\tsum multithreaded")
    @time sum_result = partial_sum_threaded(py)
    #assert sum is correct
    @assert abs(sum_result - 7.4*n) < n*ASSERT_TOLERANCE "Got $sum_result expected $(7.4*n)"
    print("\tsum")
    @time sum_result = sum(py)

end


