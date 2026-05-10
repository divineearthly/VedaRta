#ifndef VEDIC_BRIDGE_H
#define VEDIC_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

// Updated to match the 2-argument call in vr_runtime.cpp
float sphota_burst(float x, float scale);
float sphota_activation(float x, float scale);

#ifdef __cplusplus
}
#endif

#endif
