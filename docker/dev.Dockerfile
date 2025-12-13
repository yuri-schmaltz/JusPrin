# Dockerfile for JusPrin Development Environment
# Focus: Fast iteration, debugging tools, VS Code support

ARG BASE_IMAGE=mcr.microsoft.com/devcontainers/cpp:ubuntu-22.04
FROM ${BASE_IMAGE}

LABEL maintainer="JusPrin Contributors"
LABEL description="Development environment for JusPrin"

# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install essential development and debugging tools
RUN apt-get update && apt-get install -y \
    cmake \
    gdb \
    valgrind \
    cppcheck \
    clang-format \
    clang-tidy \
    ninja-build \
    ccache \
    # GUI support libraries (match main Dockerfile dependencies)
    libgtk-3-dev \
    libglew-dev \
    libglu1-mesa-dev \
    libdbus-1-dev \
    libsecret-1-dev \
    libcurl4-openssl-dev \
    libudev-dev \
    libwebkit2gtk-4.1-dev \
    locales \
    # Networking tools
    net-tools \
    iputils-ping \
    # Cleanup
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

# Set up locale
RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen && \
    locale-gen en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

# Configure ccache for faster rebuilds
ENV PATH="/usr/lib/ccache:$PATH"
RUN mkdir -p /home/vscode/.ccache && \
    chown -R vscode:vscode /home/vscode/.ccache

# Create workspace directory
WORKDIR /workspace

# Switch to non-root user (vscode) provided by base image
USER vscode

# Set up some helpful aliases
RUN echo "alias build='cmake -B build -S . -G Ninja && cmake --build build'" >> ~/.bashrc
RUN echo "alias test='ctest --test-dir build'" >> ~/.bashrc

# Default entrypoint keeps container running
CMD ["sleep", "infinity"]
