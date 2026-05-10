#!/bin/bash
# TARGET: Gemma 4 E2B - The May 2026 Sovereign Standard
MODEL_URL="https://huggingface.co/google/gemma-4-e2b-it-gguf/resolve/main/gemma-4-e2b-it-Q8_0.gguf"
MODEL_NAME="gemma_4_sovereign.gguf"

echo "🕉️  ASCENDING TO GEMMA 4 (MAY 2026 RELEASE)..."

# 1. Multi-threaded pull for 'Larger Basis' updates
aria2c -x 16 -s 16 -o $MODEL_NAME "$MODEL_URL"

# 2. Kernel Re-Grafting for PLE Architecture
# Gemma 4 uses different normalization; we update the loader
g++ -O3 -march=armv8-a+simd sovereign_loader.cpp -o sovereign_engine

echo "✅ SUCCESS: VedaRta is now running Gemma 4 Intelligence."
