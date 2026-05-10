#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>

#define DIM 64
#define PHI 1.618033988749895f

inline float va(float x){float i=(x>=0)?x*1.05f:x*0.3f,p=(x>=0)?x*0.3f:x*1.05f;return 0.5f*x+0.25f*i+0.25f*p+0.1f*x;}

struct Layer { float W[DIM*DIM],b[DIM],dW[DIM*DIM],db[DIM];
    Layer(){for(int i=0;i<DIM*DIM;i++){W[i]=sinf(i*0.1f)*0.01f;dW[i]=0;}for(int i=0;i<DIM;i++)b[i]=db[i]=0;}
    void fwd(float*x,float*y,int n){for(int i=0;i<n;i++){float s=b[i%DIM];for(int j=0;j<DIM;j++)s+=x[(i/DIM)*DIM+j]*W[(i%DIM)*DIM+j];y[i]=s;}}
    void zg(){memset(dW,0,DIM*DIM*sizeof(float));memset(db,0,DIM*sizeof(float));}
    void upd(float lr){for(int i=0;i<DIM*DIM;i++){W[i]-=lr*dW[i];dW[i]*=0.9f;}for(int i=0;i<DIM;i++){b[i]-=lr*db[i];db[i]*=0.9f;}}
};

void text_to_vec(const char* t, float* v){for(int d=0;d<DIM;d++)v[d]=0;for(int i=0;t[i];i++)for(int d=0;d<DIM;d++)v[d]+=sinf(t[i]*d*PHI/DIM)*0.05f;}

int main() {
    printf("🕉️  Training on 10K Vedic Dataset\n\n");
    
    FILE* f = fopen("vedic_training_data_v2.jsonl", "r");
    if(!f) { printf("Dataset not found.\n"); return 1; }
    
    // Count lines
    int total=0; char line[8192];
    while(fgets(line,sizeof(line),f)) total++;
    rewind(f);
    
    int max_samples = total < 2000 ? total : 2000;
    printf("Loading %d samples from %d total...\n", max_samples, total);
    
    int N = DIM;
    float *x = new float[max_samples * N]();
    float *t = new float[max_samples * N]();
    int loaded = 0;
    
    while(fgets(line,sizeof(line),f) && loaded < max_samples) {
        char* start = strstr(line, "<|im_start|>assistant");
        if(!start) { start = strstr(line, "\"text\":\"");
            if(start) start += 8; else continue; }
        else start += 22;
        char* end = strstr(start, "<|im_end|>");
        if(!end) end = strstr(start, "\"}");
        if(end) *end = 0;
        if(strlen(start) < 10) continue;
        text_to_vec(start, &x[loaded * N]);
        text_to_vec(start, &t[loaded * N]);
        loaded++;
    }
    fclose(f);
    printf("Encoded %d samples.\n\n", loaded);
    
    Layer l1,l2,l3;
    float *h1=new float[N],*h2=new float[N],*h3=new float[N],*a1=new float[N],*a2=new float[N],*a3=new float[N],*g=new float[N];
    int epochs = 1000;
    float lr = 0.001f, best = 1e9f;
    clock_t st = clock();
    
    for(int ep=0; ep<epochs; ep++) {
        float loss = 0;
        for(int s=0; s<loaded; s++) {
            float *inp=&x[s*N], *tgt=&t[s*N];
            l1.fwd(inp,h1,N);for(int i=0;i<N;i++)a1[i]=va(h1[i]);
            l2.fwd(a1,h2,N);for(int i=0;i<N;i++)a2[i]=va(h2[i]);
            l3.fwd(a2,h3,N);for(int i=0;i<N;i++)a3[i]=va(h3[i]);
            for(int i=0;i<N;i++){float d=a3[i]-tgt[i];loss+=d*d;g[i]=2.0f*d/N;}
            l3.zg();for(int i=0;i<N;i++){int id=i%DIM;l3.db[id]+=g[i];for(int j=0;j<DIM;j++)l3.dW[id*DIM+j]+=g[i]*a2[j];}
            float*dy2=new float[N]();for(int i=0;i<N;i++)for(int j=0;j<DIM;j++)dy2[j]+=g[i]*l3.W[(i%DIM)*DIM+j];
            l2.zg();for(int i=0;i<N;i++){int id=i%DIM;l2.db[id]+=dy2[i];for(int j=0;j<DIM;j++)l2.dW[id*DIM+j]+=dy2[i]*a1[j];}
            float*dy1=new float[N]();for(int i=0;i<N;i++)for(int j=0;j<DIM;j++)dy1[j]+=dy2[i]*l2.W[(i%DIM)*DIM+j];
            l1.zg();for(int i=0;i<N;i++){int id=i%DIM;l1.db[id]+=dy1[i];for(int j=0;j<DIM;j++)l1.dW[id*DIM+j]+=dy1[i]*inp[j];}
            l3.upd(lr);l2.upd(lr);l1.upd(lr);delete[]dy2;delete[]dy1;
        }
        loss/=(loaded*N); if(loss<best)best=loss;
        if(ep%200==0)printf("Epoch %4d: loss=%.6f best=%.6f\n",ep,loss,best);
    }
    
    float t_elapsed = (float)(clock()-st)/CLOCKS_PER_SEC;
    printf("\n✅ %d epochs in %.1fs | Best loss: %.6f\n", epochs, t_elapsed, best);
    
    // Save weights
    FILE* wf = fopen("vedic_trained_weights.bin", "wb");
    fwrite(l1.W, sizeof(float), DIM*DIM, wf);
    fwrite(l2.W, sizeof(float), DIM*DIM, wf);
    fwrite(l3.W, sizeof(float), DIM*DIM, wf);
    fclose(wf);
    printf("💾 Weights saved: vedic_trained_weights.bin\n");
    printf("   Trained on %d real Vedic examples.\n", loaded);
    
    delete[]x;delete[]t;delete[]h1;delete[]h2;delete[]h3;delete[]a1;delete[]a2;delete[]a3;delete[]g;
    return 0;
}
