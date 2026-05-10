#!/data/data/com.termux/files/usr/bin/bash
echo "🕉️  Activating VedaRta Sovereign Bridge..."

# 1. Environment Setup
LLAMA_DIR="$HOME/Divine-Earthly-ASI/llama.cpp"
VEDA_DIR="$HOME/Divine-Earthly-ASI/VedaRta"
KERNELS_DIR="$VEDA_DIR/kernels"

# 2. Autonomous Transformation
if [ -d "$LLAMA_DIR" ]; then
    echo "🔗 Linking VedaRta Kernels to Big Model..."
    
    # Inject Master Header
    grep -q "vr_kernels.h" "$LLAMA_DIR/src/llama.cpp" || \
    sed -i '1i #include "vr_kernels.h"' "$LLAMA_DIR/src/llama.cpp"

    # Swap dot product with Vedic Logic
    sed -i 's/ggml_vec_dot_f32/veda_vec_dot_f32/g' "$LLAMA_DIR/src/llama-quant.cpp"
    
    # Move DNA to source folder
    cp "$KERNELS_DIR/"*.h "$LLAMA_DIR/src/"
    
    echo "✅ DNA Merged. Triggering Re-birth (CMake Build)..."
    
    # 3. CMake Rebuild Logic (Autonomous)
    cd "$LLAMA_DIR"
    mkdir -p build
    cmake -B build -DGGML_NEON=ON
    cmake --build build --config Release -j$(nproc)
    
    echo "🕉️  Swayam-Vaha Complete. Generation 3 Initialized."
else
    echo "❌ Error: Big Model not found."
fi
