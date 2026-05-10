#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "vr_activation.h"
#define DIM 64
#define PHI 1.618033988749895f
inline float va(float x){float i=(x>=0)?x*1.05f:x*0.3f,p=(x>=0)?x*0.3f:x*1.05f;return 0.5f*x+0.25f*i+0.25f*p+0.1f*x;}
int main(int argc, char** argv) {
    printf("🕉️  VedaRta Runtime v1.0\n   Pure Vedic inference — no llama.cpp, no Qwen\n\n");
    const char* input = argc>1 ? argv[1] : "Urdhva Tiryagbhyam";
    printf("Input: %s\n", input);
    float embedding[DIM] = {0};
    for(int i=0; input[i]; i++) for(int d=0; d<DIM; d++) embedding[d] += sinf(input[i] * d * PHI / DIM) * 0.1f;
    float h1[DIM], h2[DIM], out[DIM];
    for(int d=0;d<DIM;d++) h1[d]=va(embedding[d]);
    for(int d=0;d<DIM;d++) h2[d]=sphota_activation(h1[d], PHI);
    for(int d=0;d<DIM;d++) out[d]=va(h2[d]*0.5f);
    printf("\nPancha Kosha output (first 8 dims): ");
    for(int d=0;d<8;d++) printf("%.3f ", out[d]);
    printf("\nOutput norm: %.4f\n", [&]{float s=0;for(int d=0;d<DIM;d++)s+=out[d]*out[d];return sqrtf(s);}());
    printf("\n✅ VedaRta inference complete.\n");
    return 0;
}
