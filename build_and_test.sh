#!/bin/bash

# CTRLxT OS Build and Test Script
# This script compiles and tests the core components of CTRLxT OS

set -e  # Exit on error

# Define colors for output
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;34m"
RESET="\033[0m"

echo -e "${BLUE}CTRLxT OS Build and Test Script${RESET}"
echo -e "${BLUE}=================================${RESET}"

# Create build directories
mkdir -p build/bin

# Set compiler and flags
CC=gcc
CFLAGS="-Wall -Wextra -std=c11 -I./src"
LDFLAGS="-lm" # Add math library for quantum calculations

# Function to build a component
build_component() {
    component_name=$1
    source_files=(${@:2})
    
    echo -e "\n${YELLOW}Building ${component_name}...${RESET}"
    
    # Compile object files
    object_files=()
    for src in "${source_files[@]}"; do
        obj="build/$(basename "${src%.*}").o"
        echo "Compiling $src -> $obj"
        $CC $CFLAGS -c "$src" -o "$obj"
        object_files+=("$obj")
    done
    
    # Link into a test executable
    test_name="build/bin/test_${component_name}"
    echo "Linking ${object_files[*]} -> $test_name"
    $CC "${object_files[@]}" -o "$test_name" $LDFLAGS
    
    echo -e "${GREEN}Successfully built ${component_name}${RESET}"
    
    # Return the test executable path
    echo "$test_name"
}

# Function to run a test
run_test() {
    test_exe=$1
    test_name=$(basename "$test_exe")
    
    echo -e "\n${YELLOW}Running test ${test_name}...${RESET}"
    
    if [[ -x "$test_exe" ]]; then
        "$test_exe"
        if [[ $? -eq 0 ]]; then
            echo -e "${GREEN}Test ${test_name} passed!${RESET}"
            return 0
        else
            echo -e "${RED}Test ${test_name} failed!${RESET}"
            return 1
        fi
    else
        echo -e "${RED}Error: ${test_exe} is not executable${RESET}"
        return 1
    fi
}

# Build and test the newly implemented components
echo -e "\n${BLUE}Building and testing Quantum Integration...${RESET}"
cd tests
make clean
make
run_test "./quantum_integration_test"
cd ..

# Build and test the Hardware Abstraction Layer (HAL)
echo -e "\n${BLUE}Building and testing Hardware Abstraction Layer...${RESET}"
hal_test=$(build_component "hal" \
    "src/kernel/hal/hal.c" \
    "src/kernel/hal/arch/x86/x86_hal.c" \
    "tests/unit/test_hal.c")
run_test "$hal_test"

# Build and test the Memory Management System
echo -e "\n${BLUE}Building and testing Memory Management System...${RESET}"
mm_test=$(build_component "memory_manager" \
    "src/kernel/hal/hal.c" \
    "src/kernel/hal/arch/x86/x86_hal.c" \
    "src/kernel/memory/memory_manager.c" \
    "tests/unit/test_memory_manager.c")
run_test "$mm_test"

# Build and test the Process Management System
echo -e "\n${BLUE}Building and testing Process Management System...${RESET}"
pm_test=$(build_component "process_manager" \
    "src/kernel/hal/hal.c" \
    "src/kernel/hal/arch/x86/x86_hal.c" \
    "src/kernel/memory/memory_manager.c" \
    "src/kernel/process/process_manager.c" \
    "tests/unit/test_process_manager.c")
run_test "$pm_test"

# Build and test the Process Scheduler
echo -e "\n${BLUE}Building and testing Process Scheduler...${RESET}"
scheduler_test=$(build_component "scheduler" \
    "src/kernel/hal/hal.c" \
    "src/kernel/hal/arch/x86/x86_hal.c" \
    "src/kernel/memory/memory_manager.c" \
    "src/kernel/process/process_manager.c" \
    "src/kernel/process/scheduler.c" \
    "tests/unit/test_scheduler.c")
run_test "$scheduler_test"

# Build and test the Quantum Message Bus
echo -e "\n${BLUE}Building and testing Quantum Message Bus...${RESET}"
qbus_test=$(build_component "quantum_message_bus" \
    "src/quantum/messaging/quantum_message_bus.c" \
    "tests/unit/test_quantum_message_bus.c")
run_test "$qbus_test"

echo -e "\n${GREEN}All build and tests completed successfully!${RESET}"