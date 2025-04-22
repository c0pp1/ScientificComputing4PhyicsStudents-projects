# Use AlmaLinux9 as the base image
FROM almalinux:9

# Set environment variables for consistent locale settings
ENV LANG=C.UTF-8 \
    LC_ALL=C.UTF-8

# Update system packages and install essential development tools
RUN dnf -y update && \
    dnf -y install nano gcc gcc-c++ gdb make cmake wget git boost-devel boost-program-options \
    gsl-devel zlib-devel && \
    dnf clean all
RUN dnf install -y python3.11 && dnf install -y python3.11-pip
# Install HDF5
RUN mkdir -p /root/hdf5_install && cd /root/hdf5_install && \
    wget https://github.com/HDFGroup/hdf5/releases/download/hdf5_1.14.6/hdf5-1.14.6.tar.gz && \
    tar -xzf hdf5-1.14.6.tar.gz && \
    cd hdf5-1.14.6 && \
    ./configure --prefix=/usr/local/hdf5 --enable-cxx && \
    make && \
    make install && \
    cd .. && \
    rm -rf hdf5-1.14.6.tar.gz
# Set environment variables for HDF5
ENV HDF5_DIR=/usr/local/hdf5 \
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/hdf5/lib \
    PATH=$PATH:/usr/local/hdf5/bin

# Create non-priviliged user
ARG USERNAME=user
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd -g $USER_GID $USERNAME && \
    useradd -m -u $USER_UID -g $USERNAME $USERNAME
RUN mkdir -p /etc/sudoers.d && \
    echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$USERNAME && \
    chmod 0440 /etc/sudoers.d/$USERNAME

# Switch to the non-priviliged user
USER $USERNAME

# Set the working directory to the user's home directory
WORKDIR /home/$USERNAME

# Install Julia
RUN curl -fsSL https://install.julialang.org | sh -s -- -y


# Define the default command to start an interactive bash shell
CMD ["/bin/bash"]