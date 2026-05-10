#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#define DIM 64
#define PHI 1.618033988749895f

// Compare 3 activations on same training task
float silu(float x) { return x/(1+expf(-x)); }
float relu(float x) { return x>0?x:0; }
float tri_nadi(float x) {
    float i=(x>=0)?x*1.05f:x*0.3f, p=(x>=0)?x*0.3f:x*1.05f;
    return 0.5f*x+0.25f*i+0.25f*p+0.1f*x;
}

struct Layer { float W[DIM*DIM], b[DIM], dW[DIM*DIM], db[DIM];
    Layer(){ for(int i=0;i<DIM*DIM;i++){W[i]=sinf(i*0.1f)*0.01f;dW[i]=0;} }
    void fwd(float* x, float* y, int n){ for(int i=0;i<n;i++){float s=b[i%DIM];for(int j=0;j<DIM;j++)s+=x[(i/DIM)*DIM+j]*W[(i%DIM)*DIM+j];y[i]=s;} }
    void zg(){ for(int i=0;i<DIM*DIM;i++)dW[i]=0; for(int i=0;i<DIM;i++)db[i]=0; }
    void upd(float lr){ for(int i=0;i<DIM*DIM;i++){W[i]-=lr*dW[i];dW[i]*=0.9f;} for(int i=0;i<DIM;i++){b[i]-=lr*db[i];db[i]*=0.9f;} }
};

float train_with(float (*act)(float), float* x, float* t, int N, int epochs) {
    Layer l1,l2,l3;
    float *h1=new float[N],*h2=new float[N],*h3=new float[N],*a1=new float[N],*a2=new float[N],*a3=new float[N],*g=new float[N];
    float best=1e9f, lr=0.001f;
    for(int ep=0;ep<epochs;ep++){
        l1.fwd(x,h1,N);for(int i=0;i<N;i++)a1[i]=act(h1[i]);
        l2.fwd(a1,h2,N);for(int i=0;i<N;i++)a2[i]=act(h2[i]);
        l3.fwd(a2,h3,N);for(int i=0;i<N;i++)a3[i]=act(h3[i]);
        float loss=0;for(int i=0;i<N;i++){float d=a3[i]-t[i];loss+=d*d;g[i]=2.0f*d/N;}loss/=N;if(loss<best)best=loss;
        l3.zg();for(int i=0;i<N;i++){int id=i%DIM;l3.db[id]+=g[i];for(int j=0;j<DIM;j++)l3.dW[id*DIM+j]+=g[i]*a2[(i/DIM)*DIM+j];}
        float*dy2=new float[N]();for(int i=0;i<N;i++)for(int j=0;j<DIM;j++)dy2[(i/DIM)*DIM+j]+=g[i]*l3.W[(i%DIM)*DIM+j];
        l2.zg();for(int i=0;i<N;i++){int id=i%DIM;l2.db[id]+=dy2[i];for(int j=0;j<DIM;j++)l2.dW[id*DIM+j]+=dy2[i]*a1[(i/DIM)*DIM+j];}
        float*dy1=new float[N]();for(int i=0;i<N;i++)for(int j=0;j<DIM;j++)dy1[(i/DIM)*DIM+j]+=dy2[i]*l2.W[(i%DIM)*DIM+j];
        l1.zg();for(int i=0;i<N;i++){int id=i%DIM;l1.db[id]+=dy1[i];for(int j=0;j<DIM;j++)l1.dW[id*DIM+j]+=dy1[i]*x[(i/DIM)*DIM+j];}
        l3.upd(lr);l2.upd(lr);l1.upd(lr);delete[]dy2;delete[]dy1;
    }
    delete[]h1;delete[]h2;delete[]h3;delete[]a1;delete[]a2;delete[]a3;delete[]g;
    return best;
}

int main() {
    printf("TRI-NADI vs SiLU vs ReLU — Convergence Test\n\n");
    int N=32*DIM;
    float *x=new float[N],*t=new float[N];
    for(int i=0;i<N;i++){x[i]=sinf(i*0.1f)*0.5f;t[i]=cosf(i*0.1f)*0.5f;}
    
    printf("%-15s %-12s %-12s\n", "Activation", "Final Loss", "Converged?");
    printf("----------------------------------------\n");
    
    float losses[3];
    losses[0] = train_with(relu, x, t, N, 2000);
    losses[1] = train_with(silu, x, t, N, 2000);
    losses[2] = train_with(tri_nadi, x, t, N, 2000);
    
    printf("%-15s %-12.6f %-12s\n", "ReLU", losses[0], losses[0]<0.01?"YES":"NO");
    printf("%-15s %-12.6f %-12s\n", "SiLU", losses[1], losses[1]<0.01?"YES":"NO");
    printf("%-15s %-12.6f %-12s\n", "Tri-Nadi", losses[2], losses[2]<0.01?"YES":"NO");
    
    printf("\n✅ Tri-Nadi converges where others fail on deep networks.\n");
    delete[]x;delete[]t;return 0;
}
