#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <arm_neon.h>

#define PHI 1.618033988749895f

void softmax_attention(float* Q, float* K, float* V, float* out, int seq, int dim) {
    float scale = 1.0f / sqrtf(dim);
    float* scores = new float[seq * seq];
    for(int i=0; i<seq; i++)
        for(int j=0; j<seq; j++) {
            float s = 0;
            for(int d=0; d<dim; d++) s += Q[i*dim+d] * K[j*dim+d];
            scores[i*seq+j] = s * scale;
        }
    for(int i=0; i<seq; i++) {
        float max_val = scores[i*seq];
        for(int j=1; j<seq; j++) if(scores[i*seq+j] > max_val) max_val = scores[i*seq+j];
        float sum = 0;
        for(int j=0; j<seq; j++) { scores[i*seq+j] = expf(scores[i*seq+j] - max_val); sum += scores[i*seq+j]; }
        for(int j=0; j<seq; j++) scores[i*seq+j] /= sum;
    }
    for(int i=0; i<seq; i++)
        for(int d=0; d<dim; d++) {
            out[i*dim+d] = 0;
            for(int j=0; j<seq; j++) out[i*dim+d] += scores[i*seq+j] * V[j*dim+d];
        }
    delete[] scores;
}

inline float tri_nadi(float x) {
    float ida=(x>=0)?x*1.05f:x*0.3f, pingala=(x>=0)?x*0.3f:x*1.05f;
    return 0.5f*x + 0.25f*ida + 0.25f*pingala + 0.1f*x;
}

// Samashti-Sphota: Global context + local focus
void sphota_attention(float* Q, float* K, float* V, float* out, int seq, int dim) {
    float* alignment = new float[seq];
    float* global_context = new float[dim]();
    // Samashti: Aggregate global context from all keys
    for(int i=0; i<seq; i++)
        for(int d=0; d<dim; d++) global_context[d] += K[i*dim+d] / seq;
    for(int i=0; i<seq; i++) {
        float local = 0, global = 0;
        for(int d=0; d<dim; d++) {
            local  += Q[i*dim+d] * K[i*dim+d];
            global += Q[i*dim+d] * global_context[d];
        }
        alignment[i] = tri_nadi(local / sqrtf(dim)) * 0.54f /*SVAYAMBHU_LOCAL*/ + tri_nadi(global / sqrtf(dim)) * 0.46f /*SVAYAMBHU_GLOBAL*/;
    }
    float sum_weights = 0;
    for(int i=0; i<seq; i++) sum_weights += alignment[i];
    for(int i=0; i<seq; i++) {
        float w = alignment[i] / (sum_weights + 1e-8f);
        for(int d=0; d<dim; d++) out[i*dim+d] = w * V[i*dim+d];
    }
    delete[] alignment; delete[] global_context;
}

double bench(void (*fn)(float*,float*,float*,float*,int,int), float* Q, float* K, float* V, float* out, int seq, int dim, int runs) {
    double total = 0;
    for(int r=0; r<runs; r++) {
        clock_t start = clock();
        fn(Q, K, V, out, seq, dim);
        total += (double)(clock() - start) / CLOCKS_PER_SEC;
    }
    return total / runs * 1000.0;
}

int main() {
    printf("SPHOTA vs SOFTMAX Benchmark\n");
    printf("%-8s %-8s %-12s %-12s %-8s\n", "Dim", "Seq", "Softmax(ms)", "Sphota(ms)", "Speedup");
    int dims[] = {64, 128, 256};
    int seqs[]  = {32, 64, 128, 256, 512, 1024};
    for(int di=0; di<3; di++) {
        int dim = dims[di];
        for(int si=0; si<6; si++) {
            int seq = seqs[si];
            float* Q = new float[seq*dim]; float* K = new float[seq*dim];
            float* V = new float[seq*dim]; float* out1 = new float[seq*dim]; float* out2 = new float[seq*dim];
            for(int i=0; i<seq*dim; i++) { Q[i]=sinf(i*0.1f)*0.5f; K[i]=cosf(i*0.1f)*0.5f; V[i]=sinf(i*0.05f)*0.5f; }
            double sm = bench(softmax_attention, Q, K, V, out1, seq, dim, 3);
            double sp = bench(sphota_attention, Q, K, V, out2, seq, dim, 10);
            printf("%-8d %-8d %-12.2f %-12.2f %-8.1fx\n", dim, seq, sm, sp, sm/sp);
            delete[] Q; delete[] K; delete[] V; delete[] out1; delete[] out2;
        }
    }
    return 0;
}
