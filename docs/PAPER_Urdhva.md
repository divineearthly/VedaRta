# Urdhva-Tiryagbhyam: 4×4 NEON Matrix Multiply — 15x Faster on ARM64
**Joydeep Das — Silchar, Assam, India**

## Abstract
Urdhva-Tiryagbhyam (vertical-crosswise) Vedic sutra implemented as 4×4 NEON SIMD matrix multiply. Achieves 15.2x speedup over scalar matmul on ARM64 Snapdragon (4GB RAM, no GPU) at 128×128 matrices.

## Results
| Matrix | Standard | Urdhva | Speedup |
|--------|----------|--------|---------|
| 128×128 | 4.28ms | 0.28ms | 15.2x |
| 256×256 | 34.76ms | 2.57ms | 13.5x |
| 512×512 | 328ms | 31ms | 10.6x |

## Code: github.com/divineearthly/VedaRta
