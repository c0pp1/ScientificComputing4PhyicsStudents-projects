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

using Test

@testset "daxpy" begin
    # Test basic functionality
    px = [1.0, 2.0, 3.0]
    py = [4.0, 5.0, 6.0]
    daxpy(2.0, px, py)
    @test py == [6.0, 9.0, 12.0]

    # Test with decimal scalar
    px = [0.5, 1.5, -2.0]
    py = [1.0, 2.0, 3.0]
    daxpy(1.25, px, py)
    @test all(abs.(py .- [1.625, 3.875, 0.5]) .< ASSERT_TOLERANCE)

    # Test zero scalar
    px = [1.0, 2.0]
    py = [3.0, 4.0]
    daxpy(0.0, px, py)
    @test py == [3.0, 4.0]

    # Test empty vectors
    px = Float64[]
    py = Float64[]
    daxpy(2.0, px, py)
    @test py == Float64[]

    # Test mismatched lengths
    px = [1.0, 2.0]
    py = [3.0]
    @test_throws ArgumentError daxpy(2.0, px, py)

    # Unrolled version (daxpy_dotted)
    px = [1.0, 2.0, 3.0]
    py = [4.0, 5.0, 6.0]
    daxpy_dotted(2.0, px, py)
    @test py == [6.0, 9.0, 12.0]

    # Unrolled with decimal scalar
    px = [0.5, 1.5, -2.0]
    py = [1.0, 2.0, 3.0]
    daxpy_dotted(1.25, px, py)
    @test all(abs.(py .- [1.625, 3.875, 0.5]) .< ASSERT_TOLERANCE)

    # Unrolled zero scalar
    px = [1.0, 2.0]
    py = [3.0, 4.0]
    daxpy_dotted(0.0, px, py)
    @test py == [3.0, 4.0]

    # Unrolled empty vectors
    px = Float64[]
    py = Float64[]
    daxpy_dotted(2.0, px, py)
    @test py == Float64[]

    # Unrolled mismatched lengths
    px = [1.0, 2.0]
    py = [3.0]
    @test_throws ArgumentError daxpy_dotted(2.0, px, py)
end

