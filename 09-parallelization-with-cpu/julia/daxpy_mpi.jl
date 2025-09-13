using MPI
MPI.Init()

root = 0
comm = MPI.COMM_WORLD
rank = MPI.Comm_rank(comm)
size = MPI.Comm_size(comm)

function daxpy(a::Float64, px::Array{Float64}, py::Array{Float64})
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

function daxpy_mpi(a::Float64, px::Array{Float64}, py::Array{Float64})
    
    # get the length of the vector assuming 
    # root has total while others have chunks
    n = 0
    if rank == root
        n = length(px)
    end
    n = MPI.Bcast(n, root, comm)
    # check if the vectors are the same length
    if length(px) != length(py)
        throw(ArgumentError("Vectors must have the same length"))
    end
    # return on trivial cases
    if n <= 0 || a == 0.0
        return
    end

    chunk_size = div(n, max(size-1, 1))
    if size < 2
        remainder = n
    else
        remainder = mod(n, size-1)
    end
    # if root process than process the remainder 
    # and send the rest to other processes
    if rank == root
        # send
        requests = MPI.MultiRequest((size-1)*2)
        for i in 2:size
            start_idx = (i-2)*chunk_size + remainder + 1
            end_idx = (i-1)*chunk_size + remainder
            MPI.Isend(px[start_idx:end_idx], comm, requests[2*(i-2)+1]; dest=i-1, tag=0)
            MPI.Isend(py[start_idx:end_idx], comm, requests[2*(i-1)]; dest=i-1, tag=1)
        end
        # process the remainder
        py[1:remainder] .+= a .* px[1:remainder]
        MPI.Waitall(requests)
        # receive
        for i in 2:size
            start_idx = (i-2)*chunk_size + remainder + 1
            end_idx = (i-1)*chunk_size + remainder
            py_view = @view py[start_idx:end_idx]
            MPI.Recv!(py_view, comm; source=i-1)
        end
    else
        # receive
        MPI.Recv!(px, comm; source=root, tag=0)
        MPI.Recv!(py, comm; source=root, tag=1)
        # compute
        @. py += a * px
        MPI.Send(py, comm; dest=root)
    end
    MPI.Barrier(comm)
end

function sum_mpi(px::Array{Float64})
    
    # get the length of the vector assuming 
    # root has total while others have chunks
    n = 0
    if rank == root
        n = length(px)
    end
    n = MPI.Bcast(n, root, comm)
    if n <= 0
        return 0.0
    end

    chunk_size = div(n, max(size-1, 1))
    if size < 2
        remainder = n
    else
        remainder = mod(n, size-1)
    end

    requests = MPI.MultiRequest(size-1)
    if rank == root
        for i in 2:size
            start_idx = (i-2)*chunk_size + 1
            end_idx = (i-1)*chunk_size
            MPI.Isend(px[start_idx:end_idx], comm, requests[i-1]; dest=i-1)
        end
        partial_sums = zeros(size)
        partial_sums[1] = sum(px[end-remainder+1:end])

        MPI.Waitall(requests)
        for i in 2:size
            partial_sums[i] = MPI.Recv(Float64, comm; source=i-1)
        end
    
        return sum(partial_sums)
    else
        MPI.Recv!(px, comm; source=root)
        local_sum = sum(px[1:chunk_size]) # specify chunk_size to avoid summing too many elemnts if px was too long
        MPI.Send(local_sum, comm; dest=root)
        return 0.
    end
end


ARRAY_SIZES = [10, 10^6, 10^8]
ASSERT_TOLERANCE = 1e-10
a = 3.
if rank==root 
    println("Using $size processes") 
end
for n in ARRAY_SIZES
    if rank==root 
        println("Array size: $n")
    end
    if rank == root
        px = fill(0.1, n)
        py = fill(7.1, n)
    else
        chunk_size = div(n, max(size-1, 1))
        px = Array{Float64, 1}(undef, chunk_size)
        py = Array{Float64, 1}(undef, chunk_size)
    end

    if rank == root
        print("\tDAXPY mpi")
        @time daxpy_mpi(a, px, py)
        # assert that the result is correct
        @assert all(abs.(py .- (a*0.1 + 7.1)) .< ASSERT_TOLERANCE) "Got $(py[1]) expected $(a*0.1 + 7.1)"
        print("\tDAXPY")
        py = fill(7.1, n)
        @time daxpy(a, px, py)

        print("\tsum mpi")
        @time sum_result = sum_mpi(py)
        #assert sum is correct
        @assert abs(sum_result - 7.4*n) < n*ASSERT_TOLERANCE "Got $sum_result expected $(7.4*n)"
        print("\tsum")
        @time sum_result = sum(py)
    else
        daxpy_mpi(a, px, py)
        sum_mpi(py)
    end

end


