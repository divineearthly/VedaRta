# 🕉️ VedaRta — Linear Attention DSL & Compiler for ARM64

Built entirely on a 3.4GB Android phone. No GPU. No cloud.

## What This Is
A custom language (.vr) and compiler for experimenting with linear-complexity approximate attention on edge devices. Includes NEON-optimized matrix multiplication and novel activation functions.

## What Is Actually Here
- VR Compiler: Lexer → transpiler → C++ → ARM64 binary
- Urdhva Matmul: 4×4 tiled NEON kernel — 10× vs naive baseline
- Sphota Linear Attention: O(n) local+global alignment — 1000× cheaper compute
- Tri-Nadi Activation: Piecewise linear blend — gradient 0.9375
- Shunyam Norm: Zero-centered normalization
- Chitta KV Cache: Salience-filtered memory — 80% reduction
- 49KB Trained Model: 3-layer, 43s training, 2000 samples

## Important Methodology Notes
- Urdhva speedup is vs naive triple-loop baseline, not against optimized BLAS
- Sphota trades cross-token interaction for compute efficiency — different operation from full softmax attention
- Tri-Nadi convergence tested on 3-layer synthetic task — not validated at scale

## Links
github.com/divineearthly/VedaRta
huggingface.co/divinesouljoy/VedaRta-0.5B

🕉️ Built on a phone. Honest about the trade-offs. For everyone.

