#ifndef BRIDGE_VEDA_H
#define BRIDGE_VEDA_H

#include "kernels/vr_kernels.h"
#include "kernels/vr_activation.h"
#include "kernels/shunya_siddhi.h"

// Define the 31B Chakra (Layer) structure
#define TOTAL_CHAKRAS 60
#define VEDA_DIM 6144

// Force the engine to use Sphota Burst for text generation
extern "C" {
    float sphota_burst(float x, float scale);
    void urdhva_matmul(const float* a, const float* b, float* c, int m, int n, int k);
}

#endif
