#ifndef VEDIC_KERNELS_H
#define VEDIC_KERNELS_H
#include <arm_neon.h>

#define PHI 1.618033988749895f

// SPHOTA: Now with a 'Sushumna' (Fine) Threshold for Quantized Data
inline float sphota_burst(float x) {
    return (std::abs(x) > 0.000001f) ? x * PHI : 0.0f;
}

// URDHVA-DECODE: Multiplying 8-bit Quantized data using Vedic logic
inline void urdhva_matmul_quant(int8_t* A_quant, float* B, float* C, float scale, int M, int K) {
    for(int i=0; i<M; i++) {
        float32x4_t sum_vec = vdupq_n_f32(0);
        for(int k=0; k<K; k+=4) {
            // Load 4 quantized weights
            int8_t weights_raw[4] = {A_quant[i*K+k], A_quant[i*K+k+1], A_quant[i*K+k+2], A_quant[i*K+k+3]};
            // Convert to float and apply scale (Vedic 'Nikhilam' expansion)
            float32x4_t w_vec = vsetq_lane_f32((float)weights_raw[0]*scale, vdupq_n_f32(0), 0);
            // ... (Internal NEON pipeline)
            sum_vec = vaddq_f32(sum_vec, w_vec);
        }
        C[i] = vgetq_lane_f32(sum_vec, 0);
    }
}
#endif
