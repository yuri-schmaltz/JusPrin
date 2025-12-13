# Profile-Guided Optimization (PGO) Guide

Profile-Guided Optimization (PGO) improves application performance by using data from a representative run of the program to optimize the final build. This allows the compiler to inline hot functions, optimize branch prediction, and reorganize code layout.

## Benefits

-   **Performance**: Typically 5-20% faster execution for optimized paths.
-   **Binary Size**: Can reduce binary size for cold code paths.

## Prerequisites

-   **Linux**: GCC or Clang (llvm-profdata required for Clang).
-   **Windows**: MSVC.
-   **CMake**: 3.13+.

## Automated Usage

We provided scripts to automate the 3-step PGO process (Build Instrument -> Run -> Build Optimized).

### Linux / Mac
```bash
chmod +x scripts/build_pgo.sh
./scripts/build_pgo.sh
```

### Windows
```cmd
scripts\build_pgo.bat
```

## Manual Usage (CMake)

You can manually control PGO via CMake options if you want to integrate it into your CI/CD pipeline or custom workflow.

### 1. Build with Instrumentation
```bash
cmake -B build -S . -DSLIC3R_PGO_GENERATE=ON
cmake --build build
```

### 2. Generate Profile
Run the application through representative use cases. The more realistic the usage, the better the optimization.
```bash
./build/src/jus-prin --slice complex_model.stl
```

### 3. Build Optimized
```bash
cmake -B build -S . -DSLIC3R_PGO_GENERATE=OFF -DSLIC3R_PGO_USE=ON
cmake --build build
```

## Tuning

The PGO scripts run a simple `--help` command by default. For maximum performance gains, you should modify the script to run actual slicing tasks:
```bash
# In scripts/build_pgo.sh
"$BUILD_DIR/src/jus-prin" --slice tests/data/complex_model.stl --output /tmp/out.gcode
```
