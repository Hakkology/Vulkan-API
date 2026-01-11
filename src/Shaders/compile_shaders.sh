#!/bin/bash
cd "$(dirname "$0")"

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
    echo "glslangValidator not found in $GLSLANG_VALIDATOR. Trying system glslangValidator..."
    GLSLANG_VALIDATOR=$(which glslangValidator)
    if [ -z "$GLSLANG_VALIDATOR" ]; then
        echo "glslangValidator not found in PATH either."
        exit 1
    fi
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

# Compile specific colored and textured shaders
echo "Compiling colored_shader.frag..."
"$GLSLANG_VALIDATOR" -V colored_shader.frag -o "$OUTPUT_DIR/colored_shader.frag.spv"
echo "Compiling colored_shader.vert..."
"$GLSLANG_VALIDATOR" -V colored_shader.vert -o "$OUTPUT_DIR/colored_shader.vert.spv"

echo "Compiling textured_shader.frag..."
"$GLSLANG_VALIDATOR" -V textured_shader.frag -o "$OUTPUT_DIR/textured_shader.frag.spv"
echo "Compiling textured_shader.vert..."
"$GLSLANG_VALIDATOR" -V textured_shader.vert -o "$OUTPUT_DIR/textured_shader.vert.spv"

echo "Shader compilation finished."
