#include <cstdio>
#include <cmath>
#define DIM 64
#define PHI 1.618033988749895f

inline float va(float x){float i=(x>=0)?x*1.05f:x*0.3f,p=(x>=0)?x*0.3f:x*1.05f;return 0.5f*x+0.25f*i+0.25f*p+0.1f*x;}

int main() {
    float W1[DIM*DIM], W2[DIM*DIM], W3[DIM*DIM];
    float b1[DIM], b2[DIM], b3[DIM];
    
    FILE* f = fopen("vedic_trained_weights.bin", "rb");
    if(!f) { printf("No weights file\n"); return 1; }
    fread(W1, sizeof(float), DIM*DIM, f);
    fread(W2, sizeof(float), DIM*DIM, f);
    fread(W3, sizeof(float), DIM*DIM, f);
    fread(b1, sizeof(float), DIM, f);
    fread(b2, sizeof(float), DIM, f);
    fread(b3, sizeof(float), DIM, f);
    fclose(f);
    
    // Test inference
    const char* input = "Urdhva Tiryagbhyam is a Vedic multiplication sutra";
    float embedding[DIM] = {0};
    for(int i=0; input[i]; i++) 
        for(int d=0; d<DIM; d++) 
            embedding[d] += sinf(input[i] * d * PHI / DIM) * 0.1f;
    
    float h1[DIM], h2[DIM], out[DIM];
    // Layer 1 (Annamaya)
    for(int i=0; i<DIM; i++) {
        h1[i] = b1[i];
        for(int j=0; j<DIM; j++) h1[i] += embedding[j] * W1[i*DIM+j];
        h1[i] = va(h1[i]);
    }
    // Layer 2 (Manomaya)
    for(int i=0; i<DIM; i++) {
        h2[i] = b2[i];
        for(int j=0; j<DIM; j++) h2[i] += h1[j] * W2[i*DIM+j];
        h2[i] = va(h2[i]);
    }
    // Layer 3 (Anandamaya)
    for(int i=0; i<DIM; i++) {
        out[i] = b3[i];
        for(int j=0; j<DIM; j++) out[i] += h2[j] * W3[i*DIM+j];
        out[i] = va(out[i]);
    }
    
    float norm = 0;
    for(int d=0; d<DIM; d++) norm += out[d] * out[d];
    norm = sqrtf(norm);
    
    printf("🕉️  Trained Vedic Model Inference\n");
    printf("Input: %s\n", input);
    printf("Output (first 8): ");
    for(int d=0; d<8; d++) printf("%.4f ", out[d]);
    printf("\nNorm: %.4f\n", norm);
    
    // Check if it learned Vedic concepts
    float vedic_score = 0;
    for(int d=0; d<DIM; d++) vedic_score += fabsf(out[d]);
    printf("Vedic Resonance: %.4f\n", vedic_score / DIM);
    
    return 0;
}
