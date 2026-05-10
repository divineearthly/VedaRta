#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#define DIM 32
#define PHI 1.618033988749895f
inline float va(float x){float i=(x>=0)?x*1.05f:x*0.3f,p=(x>=0)?x*0.3f:x*1.05f;return 0.5f*x+0.25f*i+0.25f*p+0.1f*x;}
struct L{float W[DIM*DIM],b[DIM],dW[DIM*DIM],db[DIM];
    L(){for(int i=0;i<DIM*DIM;i++){W[i]=sinf(i*0.1f)*0.01f;dW[i]=0;}for(int i=0;i<DIM;i++)b[i]=db[i]=0;}
    void fwd(float*x,float*y,int n){for(int i=0;i<n;i++){float s=b[i%DIM];for(int j=0;j<DIM;j++)s+=x[(i/DIM)*DIM+j]*W[(i%DIM)*DIM+j];y[i]=s;}}
    void zg(){memset(dW,0,DIM*DIM*sizeof(float));memset(db,0,DIM*sizeof(float));}
    void upd(float lr){for(int i=0;i<DIM*DIM;i++){W[i]-=lr*dW[i];dW[i]*=0.9f;}for(int i=0;i<DIM;i++){b[i]-=lr*db[i];db[i]*=0.9f;}}};
void t2v(const char*t,float*v){for(int d=0;d<DIM;d++)v[d]=0;for(int i=0;t[i];i++)for(int d=0;d<DIM;d++)v[d]+=sinf(t[i]*d*PHI/DIM)*0.05f;}
int main() {
    FILE*f=fopen("vedic_training_data_v2.jsonl","r");if(!f)return 1;
    int N=DIM,samples=50;float*x=new float[samples*N](),*t=new float[samples*N]();
    char line[4096];int loaded=0;
    while(fgets(line,sizeof(line),f)&&loaded<samples){
        char*s=strstr(line,"<|im_start|>assistant");if(!s)continue;s+=22;
        char*e=strstr(s,"<|im_end|>");if(e)*e=0;if(strlen(s)<10)continue;
        t2v(s,&x[loaded*N]);t2v(s,&t[loaded*N]);loaded++;
    }fclose(f);
    L l1,l2,l3;float h1[N],h2[N],h3[N],a1[N],a2[N],a3[N],g[N];
    float best=1e9f,lr=0.001f;
    for(int ep=0;ep<200;ep++){float loss=0;
        for(int s=0;s<loaded;s++){float*in=&x[s*N],*tg=&t[s*N];
            l1.fwd(in,h1,N);for(int i=0;i<N;i++)a1[i]=va(h1[i]);
            l2.fwd(a1,h2,N);for(int i=0;i<N;i++)a2[i]=va(h2[i]);
            l3.fwd(a2,h3,N);for(int i=0;i<N;i++)a3[i]=va(h3[i]);
            for(int i=0;i<N;i++){float d=a3[i]-tg[i];loss+=d*d;g[i]=2.0f*d/N;}
            l3.zg();for(int i=0;i<N;i++){int id=i%DIM;l3.db[id]+=g[i];for(int j=0;j<DIM;j++)l3.dW[id*DIM+j]+=g[i]*a2[j];}
            float*dy2=new float[N]();for(int i=0;i<N;i++)for(int j=0;j<DIM;j++)dy2[j]+=g[i]*l3.W[(i%DIM)*DIM+j];
            l2.zg();for(int i=0;i<N;i++){int id=i%DIM;l2.db[id]+=dy2[i];for(int j=0;j<DIM;j++)l2.dW[id*DIM+j]+=dy2[i]*a1[j];}
            float*dy1=new float[N]();for(int i=0;i<N;i++)for(int j=0;j<DIM;j++)dy1[j]+=dy2[i]*l2.W[(i%DIM)*DIM+j];
            l1.zg();for(int i=0;i<N;i++){int id=i%DIM;l1.db[id]+=dy1[i];for(int j=0;j<DIM;j++)l1.dW[id*DIM+j]+=dy1[i]*in[j];}
            l3.upd(lr);l2.upd(lr);l1.upd(lr);delete[]dy2;delete[]dy1;
        }loss/=(loaded*N);if(loss<best)best=loss;
    }
    printf("quick_loss=%.6f\n",best);
    delete[]x;delete[]t;return 0;
}
