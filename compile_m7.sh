#!/bin/bash

# 1. Absolute SDK Paths
export SDK_ROOT=/home/sharedaccount/m7_sdk_full/mcuxsdk
export ARMGCC_DIR=/usr
export TOOLCHAIN_FILE=$SDK_ROOT/cmake/toolchain/armgcc.cmake
export MAKE_PATH=/usr/bin/make

# 2. Establish Project Root
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_ROOT"

# Wipe and recreate build directory
rm -rf firmware/build
mkdir -p firmware/build
cd firmware/build

# 3. The "Syntax Fix" Command
# We define folder_terminator to prevent the CMake 'if' statement crash
cmake \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
    -DCMAKE_MAKE_PROGRAM="$MAKE_PATH" \
    -DSdkRootDirPath="$SDK_ROOT" \
    -DDEVICE=MIMX8ML8 \
    -Ddevice_variable_path="$SDK_ROOT/devices/i.MX/i.MX8MP/MIMX8ML8/variable.cmake" \
    -Dboard_variable_path="$SDK_ROOT/examples/_boards/evkmimx8mp/variable.cmake" \
    -DPROJECT_NAME=rpmsg_lite_str_echo_rtos_remote_cm7 \
    -Dcomponent=remote \
    -Dcore_id=remote \
    -Dfolder_terminator=remote \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=release \
    ..

# 4. Build
make -j$(nproc)
