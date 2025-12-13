# specialized Docker Development Environment for JusPrin

This directory contains resources for developing JusPrin in a consistent, containerized environment.

## Quick Start

1.  **Build and Start:**
    ```bash
    docker-compose up -d --build dev
    ```

2.  **Enter the Container:**
    ```bash
    docker-compose exec dev bash
    ```

3.  **Build JusPrin inside the container:**
    ```bash
    # Inside the container
    cmake -B build -S . -G Ninja
    cmake --build build
    ```
    *Note: We included a `build` alias for convenience.*

## Features

- **Consistent Toolchain**: Based on `mcr.microsoft.com/devcontainers/cpp`, ensuring all developers use the same compiler and tools.
- **Cached Builds**: `ccache` is configured and persisted, making repeated builds significantly faster.
- **Source Mounting**: Your local source code is mounted at `/workspace`, enabling hot-editing on your host machine while building in the container.
- **GUI Support**: Configured to forward X11 for running the GUI app from Linux hosts (requires standard X11 forwarding setup on host).

## Structure

- `dev.Dockerfile`: The definition of the development image. Includes debugging tools like `gdb` and `valgrind`.
- `../docker-compose.yml`:Orchestration file to manage volumes, networks, and build arguments.

## Troubleshooting

### GUI not showing up
Ensure you have allowed X connections on your host:
```bash
xhost +local:docker
```

### Permission issues
The container runs as the `vscode` user (UID 1000). Ensure your local files are owned by your user (usually 1000:1000 on Linux single-user systems).
