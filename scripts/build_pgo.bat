@echo off
setlocal

echo ==============================================
echo     JusPrin Profile Guided Optimization Build
echo ==============================================

set BUILD_DIR=build_pgo
set PGO_DATA_DIR=%CD%\pgo_data

REM Clean
echo [1/4] Cleaning...
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
if exist "%PGO_DATA_DIR%" rmdir /s /q "%PGO_DATA_DIR%"
mkdir "%PGO_DATA_DIR%"

REM Step 1: Instrument
echo [2/4] Building with Instrumentation...
cmake -B "%BUILD_DIR%" -S . ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DSLIC3R_PGO_GENERATE=ON ^
    -DSLIC3R_PGO_DIR="%PGO_DATA_DIR%"
    
cmake --build "%BUILD_DIR%" --config Release

REM Step 2: Workload
echo [3/4] Running Profiling Workload...
"%BUILD_DIR%\src\Release\jus-prin.exe" --help

REM Step 3: Optimize
echo [4/4] Building Optimized Binary...
cmake -B "%BUILD_DIR%" -S . ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DSLIC3R_PGO_GENERATE=OFF ^
    -DSLIC3R_PGO_USE=ON ^
    -DSLIC3R_PGO_DIR="%PGO_DATA_DIR%"

cmake --build "%BUILD_DIR%" --config Release

echo ==============================================
echo PGO Build Complete!
echo Optimized binary: %BUILD_DIR%\src\Release\jus-prin.exe
echo ==============================================
