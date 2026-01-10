#!/bin/bash

# Clean previous build
if [ -d "build" ]; then
    echo "Cleaning previous build..."
    rm -rf build
fi

# Create build directory
mkdir build

# Configure project
echo "Configuring project..."
cmake -S . -B build

# Build project
echo "Building project..."
make -C build
