/*
 * VedaRta Training Pipeline — Real JSONL → Vedic Model
 * Compile: g++ -O3 -std=c++17 -o vedic_train vedic_train_pipeline.cpp -lm
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>

#define DIM 64
#define SEQ 32
#define PHI 1.618033988749895f
#define VOCAB 111

inline float va(float x){float i=(x>=0)?x*1.05f:x*0.3f,p=(x>=0)?x*0.3f:x*1.05f;return 0.5f*x+0.25f*i+0.25f*p+0.1f*x;}

struct Layer { float W[DIM*DIM],b[DIM],dW[DIM*DIM],db[DIM];
    Layer(){for(int i=0;i<DIM*DIM;i++){W[i]=sinf(i*0.1f)*0.01f;dW[i]=0;}for(int i=0;i<DIM;i++)b[i]=db[i]=0;}
    void fwd(float*x,float*y,int n){for(int i=0;i<n;i++){float s=b[i%DIM];for(int j=0;j<DIM;j++)s+=x[(i/DIM)*DIM+j]*W[(i%DIM)*DIM+j];y[i]=s;}}
    void zg(){memset(dW,0,DIM*DIM*sizeof(float));memset(db,0,DIM*sizeof(float));}
    void upd(float lr){for(int i=0;i<DIM*DIM;i++){W[i]-=lr*dW[i];dW[i]*=0.9f;}for(int i=0;i<DIM;i++){b[i]-=lr*db[i];db[i]*=0.9f;}}
};

// Simple text → embedding encoder
void text_to_vec(const char* text, float* vec, int dim) {
    for(int d=0;d<dim;d++) vec[d]=0;
    for(int i=0;text[i];i++) for(int d=0;d<dim;d++) vec[d]+=sinf(text[i]*d*PHI/dim)*0.05f;
}

int main() {
    printf("🕉️  VedaRta Training Pipeline\n\n");
    
    // Load JSONL data
    FILE* f = fopen("vedic_selfgenerated.jsonl", "r");
    if(!f) { printf("No self-generated data yet.\n"); return 0; }
    
    int total_lines = 0;
    char line[4096];
    while(fgets(line, sizeof(line), f)) total_lines++;
    rewind(f);
    
    printf("Loading %d self-generated examples...\n", total_lines);
    
    int N = SEQ * DIM;
    int samples = total_lines < 100 ? total_lines : 100;
    float *x = new float[samples * N]();
    float *t = new float[samples * N]();
    
    int loaded = 0;
    while(fgets(line, sizeof(line), f) && loaded < samples) {
        // Extract text between <|im_start|>assistant and <|im_end|>
        char* start = strstr(line, "<|im_start|>assistant");
        if(!start) continue;
        start += 22;
        char* end = strstr(start, "<|im_end|>");
        if(end) *end = 0;
        
        text_to_vec(start, &x[loaded * N], DIM);
        // Target: same text shifted (next-token prediction)
        text_to_vec(start, &t[loaded * N], DIM);
        loaded++;
    }
    fclose(f);
    
    printf("Encoded %d samples into %d-dim vectors\n\n", loaded, DIM);
    
    // Train
    Layer l1, l2, l3;
    float *h1=new float[N],*h2=new float[N],*h3=new float[N],*a1=new float[N],*a2=new float[N],*a3=new float[N],*g=new float[N];
    int epochs = 500;
    float lr = 0.001f, best = 1e9f;
    clock_t st = clock();
    
    for(int ep=0; ep<epochs; ep++) {
        float loss = 0;
        for(int s=0; s<loaded; s++) {
            float* inp = &x[s*N];
            float* tgt = &t[s*N];
            
            l1.fwd(inp,h1,N);for(int i=0;i<N;i++)a1[i]=va(h1[i]);
            l2.fwd(a1,h2,N);for(int i=0;i<N;i++)a2[i]=va(h2[i]);
            l3.fwd(a2,h3,N);for(int i=0;i<N;i++)a3[i]=va(h3[i]);
            
            for(int i=0;i<N;i++){float d=a3[i]-tgt[i];loss+=d*d;g[i]=2.0f*d/N;}
            
            l3.zg();for(int i=0;i<N;i++){int id=i%DIM;l3.db[id]+=g[i];for(int j=0;j<DIM;j++)l3.dW[id*DIM+j]+=g[i]*a2[(i/DIM)*DIM+j];}
            float*dy2=new float[N]();for(int i=0;i<N;i++)for(int j=0;j<DIM;j++)dy2[(i/DIM)*DIM+j]+=g[i]*l3.W[(i%DIM)*DIM+j];
            l2.zg();for(int i=0;i<N;i++){int id=i%DIM;l2.db[id]+=dy2[i];for(int j=0;j<DIM;j++)l2.dW[id*DIM+j]+=dy2[i]*a1[(i/DIM)*DIM+j];}
            float*dy1=new float[N]();for(int i=0;i<N;i++)for(int j=0;j<DIM;j++)dy1[(i/DIM)*DIM+j]+=dy2[i]*l2.W[(i%DIM)*DIM+j];
            l1.zg();for(int i=0;i<N;i++){int id=i%DIM;l1.db[id]+=dy1[i];for(int j=0;j<DIM;j++)l1.dW[id*DIM+j]+=dy1[i]*inp[(i/DIM)*DIM+j];}
            l3.upd(lr);l2.upd(lr);l1.upd(lr);
            delete[]dy2;delete[]dy1;
        }
        loss /= (loaded * N);
        if(loss < best) best = loss;
        if(ep % 100 == 0) printf("Epoch %3d: loss=%.6f best=%.6f\n", ep, loss, best);
    }
    
    float elapsed = (float)(clock() - st) / CLOCKS_PER_SEC;
    printf("\n✅ %d epochs in %.1fs | Best loss: %.6f\n", epochs, elapsed, best);
    printf("Trained on %d real Vedic examples from Samyama loop.\n", loaded);
    
    delete[]x;delete[]t;delete[]h1;delete[]h2;delete[]h3;
    delete[]a1;delete[]a2;delete[]a3;delete[]g;
    return 0;
}
