#!/bin/bash
set -e

# PGO Build Script for JusPrin
# Usage: ./build_pgo.sh

echo "=============================================="
echo "    JusPrin Profile Guided Optimization Build"
echo "=============================================="

BUILD_DIR="build_pgo"
PGO_DATA_DIR="$(pwd)/pgo_data"
BENCHMARK_CMD="./src/jus-prin --help" # TODO: Replace with representative workload

# Clean previous builds
echo "[1/4] Cleaning..."
rm -rf "$BUILD_DIR"
rm -rf "$PGO_DATA_DIR"
mkdir -p "$PGO_DATA_DIR"

# Step 1: Build with instrumentation
echo "[2/4] Building with Instrumentation..."
cmake -B "$BUILD_DIR" -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DSLIC3R_PGO_GENERATE=ON \
    -DSLIC3R_PGO_DIR="$PGO_DATA_DIR" \
    -DSLIC3R_GUI=OFF # Build headless for faster compilation if possible, revert if GUI needed for profile
    
cmake --build "$BUILD_DIR" -j$(nproc)

# Step 2: Run workload
echo "[3/4] Running Profiling Workload..."
echo "Running: $BENCHMARK_CMD"
# We run the command. Note: For real PGO, this should slice a complex model.
# Since we can't easily automate GUI interactions here, we run available CLI commands.
# Ideally, integration tests should be run here.
"$BUILD_DIR/src/jus-prin" --help > /dev/null
# Add more CLI commands here contextually relevant to slicing
# "$BUILD_DIR/src/jus-prin" --slice tests/data/box.stl --output /dev/null

# For Clang, we need to merge profiles
if [ -f "$PGO_DATA_DIR/default.profraw" ]; then
    echo "Merging Clang profiles..."
    llvm-profdata merge -output="$PGO_DATA_DIR/merged.profdata" "$PGO_DATA_DIR"/*.profraw
fi

# Step 3: Build optimized
echo "[4/4] Building Optimized Binary..."
# Reconfigure for usage
cmake -B "$BUILD_DIR" -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DSLIC3R_PGO_GENERATE=OFF \
    -DSLIC3R_PGO_USE=ON \
    -DSLIC3R_PGO_DIR="$PGO_DATA_DIR" \
    -DSLIC3R_GUI=ON 

cmake --build "$BUILD_DIR" -j$(nproc)

echo "=============================================="
echo "PGO Build Complete!"
echo "Optimized binary: $BUILD_DIR/src/jus-prin"
echo "=============================================="
