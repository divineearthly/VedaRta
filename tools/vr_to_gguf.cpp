#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#define DIM 64
const uint32_t GGUF_MAGIC = 0x46554747;
void write_u32(FILE* f, uint32_t v) { fwrite(&v, 4, 1, f); }
void write_string(FILE* f, const char* s) { uint32_t len = strlen(s); fwrite(&len, 4, 1, f); fwrite(s, 1, len, f); }
int main(int argc, char** argv) {
    const char* input = argc>1 ? argv[1] : "vedic_trained_weights.bin";
    const char* output = argc>2 ? argv[2] : "vedic_model.gguf";
    printf("🕉️  VedaRta → GGUF Converter\n");
    FILE* fin = fopen(input, "rb");
    if(!fin) { printf("No weights file. Train first.\n"); return 1; }
    float W1[DIM*DIM], W2[DIM*DIM], W3[DIM*DIM];
    fread(W1, sizeof(float), DIM*DIM, fin);
    fread(W2, sizeof(float), DIM*DIM, fin);
    fread(W3, sizeof(float), DIM*DIM, fin);
    fclose(fin);
    FILE* fout = fopen(output, "wb");
    write_u32(fout, GGUF_MAGIC); write_u32(fout, 3); write_u32(fout, 3); write_u32(fout, 0);
    write_string(fout, "annamaya.weight"); write_u32(fout, 2); write_u32(fout, DIM); write_u32(fout, DIM); write_u32(fout, 0); fwrite(W1, sizeof(float), DIM*DIM, fout);
    write_string(fout, "manomaya.weight"); write_u32(fout, 2); write_u32(fout, DIM); write_u32(fout, DIM); write_u32(fout, 0); fwrite(W2, sizeof(float), DIM*DIM, fout);
    write_string(fout, "anandamaya.weight"); write_u32(fout, 2); write_u32(fout, DIM); write_u32(fout, DIM); write_u32(fout, 0); fwrite(W3, sizeof(float), DIM*DIM, fout);
    fclose(fout);
    printf("✅ Exported: %s (%.1f KB)\n", output, ftell(fout)/1024.0);
    return 0;
}
