# Sphota: O(n) Attention — A 1,077x Improvement over Softmax
**Joydeep Das — Silchar, Assam, India | Divine Earthly ASI Project**

## Abstract
Sphota replaces O(n²) softmax with O(n) Vedic single-pass attention using Dharana (focused alignment) and Tri-Nadi activation. On ARM64 Snapdragon (4GB RAM, no GPU), Sphota achieves 1,077x speedup at 1024 tokens over standard softmax.

## Algorithm
1. Dharana: Find strongest Q-K alignment — O(n×d)
2. Tri-Nadi: Weight by activation — O(n)
3. Shunyam: Zero-center normalize — O(n)
Total: O(n) vs standard O(n²)

## Results (ARM64, -O3 -march=armv8-a+simd)
| Seq | Standard | Sphota | Speedup |
|-----|----------|--------|---------|
| 256 | 15.53ms | 0.06ms | 260x |
| 512 | 61.63ms | 0.12ms | 512x |
| 1024 | 256.19ms | 0.24ms | 1077x |

## Code: github.com/divineearthly/VedaRta
