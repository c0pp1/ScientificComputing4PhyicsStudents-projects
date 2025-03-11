# Use AlmaLinux9 as the base image
FROM almalinux:9

# Set environment variables for consistent locale settings
ENV LANG=C.UTF-8 \
    LC_ALL=C.UTF-8

# Update system packages and install essential development tools
RUN dnf -y update && \
    dnf -y install nano gcc gcc-c++ make cmake wget git boost-devel boost-program-options && \
    dnf clean all

# Install Julia
RUN curl -fsSL https://install.julialang.org | sh -s -- -y

# Define the default command to start an interactive bash shell
CMD ["/bin/bash"]