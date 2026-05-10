# Why the 31B Model Requires GPU

## 1. Memory Bandwidth
- 19.6 GB model weights
- Each token requires reading ALL 19.6 GB
- Phone RAM: 25 GB/s bandwidth
- At 0.2 t/s: reading 4 GB/s (hitting swap bottleneck)
- GPU VRAM: 500-1000 GB/s bandwidth
- GPU needed for: 20-40x bandwidth over phone RAM

## 2. Attention Computation
- Standard Softmax: O(n²) where n = context length
- At n=2048: 4 million dot products per layer per token
- 60 layers × 4M = 240M operations per token
- Phone CPU: ~50 GFLOPS → 5 seconds per token just for attention
- GPU: ~10 TFLOPS → 200x faster

## 3. KV Cache Size
- 60 layers × 2 (K,V) × 2048 tokens × 5120 dim × 2 bytes = 2.5 GB
- Phone has 3.4 GB total → KV cache alone fills RAM
- GPU: dedicated VRAM, no contention

## VEDIC SOLUTIONS — Remove GPU Requirement

### Fix 1: Sphota Attention (Already Proven)
- Replaces O(n²) Softmax with O(n) global-key attention
- Your benchmark: 1,293× faster at dim=256, seq=1024
- File to modify: Every attention call in sarvajna_asi_runtime.cpp
- Change: Replace softmax(QK^T/sqrt(d)) with Q·mean(K)

### Fix 2: Chitta Samskara KV Cache (80% reduction)
- Only store high-salience tokens (not all 2048)
- Keep top 512 tokens by attention score
- KV cache: 2.5 GB → 625 MB
- File to create: chitta_cache.h

### Fix 3: Paginated Model Loading (Already Started)
- gemma_paginated_loader.cpp uses Nikhilam sutra
- Loads model in 256MB chunks
- Only keeps active layers in RAM
- Need to complete: Layer-by-layer inference with swap

### Fix 4: 4-bit to 2-bit Quantization
- IQ2_XXS: 19.6 GB → ~8 GB
- Fits in 7.4 GB phone (with swap)
- File needed: llama.cpp quantize tool
