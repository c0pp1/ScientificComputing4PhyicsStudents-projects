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

    for i in 1:n
        py[i] += a*px[i]
    end

end

function daxpy_dotted(a::Float64, px::Vector{Float64}, py::Vector{Float64})
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

using BenchmarkTools


ARRAY_SIZES = [10, 10^6, 10^8]
ASSERT_TOLERANCE = 1e-10
RUN_BENCHAMRKS = false
a = 3.
for n in ARRAY_SIZES
    px = fill(0.1, n)
    py = fill(7.1, n)

    daxpy(a, px, py)
    # print the first few results
    println("First 5 elements:")
    println(py[1:5])

    # assert that the result is correct
    @assert all(abs.(py .- (a*0.1 + 7.1)) .< ASSERT_TOLERANCE)
end

if RUN_BENCHAMRKS
    n = ARRAY_SIZES[end]
    px = fill(0.1, n)
    py = fill(7.1, n)
    println("Benchmarking daxpy with n = $n")
    display( @benchmark daxpy($a, $px, $py) )
    println("Benchmarking daxpy_dotted with n = $n")
    display( @benchmark daxpy_dotted($a, $px, $py) )
end

