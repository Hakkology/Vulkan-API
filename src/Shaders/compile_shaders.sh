#!/bin/bash

# glslangValidator path
GLSLANG_VALIDATOR_PATH="../../lib/include/ShaderCompiler"

# choose proper glslangValidator according to OS.
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    GLSLANG_VALIDATOR="$GLSLANG_VALIDATOR_PATH/glslangValidator"
elif [[ "$OSTYPE" == "msys"* ]] || [[ "$OSTYPE" == "win32" ]]; then
    GLSLANG_VALIDATOR="$GLSLANG_VALIDATOR_PATH/glslangValidator.exe"
else
    echo "Unsupported OS"
    exit 1
fi

# Check if glslangValidator exists
if [ ! -f "$GLSLANG_VALIDATOR" ]; then
    echo "glslangValidator not found in $GLSLANG_VALIDATOR"
    exit 1
fi

# Directory for SPIR-V output
OUTPUT_DIR="./spv"
mkdir -p "$OUTPUT_DIR"

# Compile .frag and .vert shaders to SPIR-V
found=false
for SHADER in *.{frag,vert}; do
    if [ -f "$SHADER" ]; then
        found=true
        echo "Compiling $SHADER..."
        "$GLSLANG_VALIDATOR" -V "$SHADER" -o "$OUTPUT_DIR/$SHADER.spv"
    fi
done

if [ "$found" = false ]; then
    echo "No shaders found to compile."
    exit 1
fi

echo "Shader compilation finished."
