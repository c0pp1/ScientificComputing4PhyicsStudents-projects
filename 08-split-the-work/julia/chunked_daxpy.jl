
function daxpy_chunked(a::Float64, px::Vector{Float64}, py::Vector{Float64}, chunk_size::Int=1)
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

    # if chunk_size is less than or equal to 1, set it to n
    # for performance reasons
    if chunk_size <= 1
        chunk_size = n
    end


    for i in 1:chunk_size:n
        end_idx = min(i + chunk_size - 1, n)
        @. py[i:end_idx] += a * px[i:end_idx]
    end

end

function partial_sum(px::Vector{Float64}, chunk_size::Int=1)
    n = length(px)
    if n <= 0
        return 0.0
    end

    # if chunk_size is less than or equal to 1, set it to n
    # for performance reasons
    if chunk_size <= 1
        chunk_size = n
    end

    n_chunks = ceil(Int64, n / chunk_size)
    partial_sums = Array{Float64, 1}(undef, n_chunks)

    for i in 1:n_chunks
        start_idx = (i - 1) * chunk_size + 1
        end_idx = min(start_idx + chunk_size - 1, n)
        partial_sums[i] = sum(px[start_idx:end_idx])
    end
    return sum(partial_sums)
end


ARRAY_SIZES = [10, 10^6, 10^8]
CHUNK_SIZES = [1, 4, 8, 10]
ASSERT_TOLERANCE = 1e-10
a = 3.
for n in ARRAY_SIZES
    println("Array size: $n")
    px = fill(0.1, n)

    for chunk_size in CHUNK_SIZES

        py = fill(7.1, n)
        println("\tChunk size $chunk_size")
        print("\t\tDAXPY")
        @time daxpy_chunked(a, px, py, chunk_size)

        # assert that the result is correct
        @assert all(abs.(py .- (a*0.1 + 7.1)) .< ASSERT_TOLERANCE)

        print("\t\tPartial sum")
        @time sum_result = partial_sum(py, chunk_size)
        #assert sum is correct
        @assert abs(sum_result - 7.4*n) < n*ASSERT_TOLERANCE
    end

end


