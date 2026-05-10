#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <arm_neon.h>

volatile float sink; // Prevent dead code elimination

void standard_matmul(float* A, float* B, float* C, int M, int K, int N) {
    for(int i=0; i<M; i++)
        for(int j=0; j<N; j++) {
            float s = 0;
            for(int k=0; k<K; k++) s += A[i*K+k] * B[k*N+j];
            C[i*N+j] = s;
        }
    sink = C[0];
}

void urdhva_matmul(float* A, float* B, float* C, int M, int K, int N) {
    for(int i=0; i+3<M; i+=4) {
        for(int j=0; j+3<N; j+=4) {
            float32x4_t c0=vdupq_n_f32(0),c1=vdupq_n_f32(0),c2=vdupq_n_f32(0),c3=vdupq_n_f32(0);
            for(int k=0; k<K; k++) {
                float32x4_t b_col = vld1q_f32(&B[k*N+j]);
                c0 = vfmaq_n_f32(c0, b_col, A[(i+0)*K+k]);
                c1 = vfmaq_n_f32(c1, b_col, A[(i+1)*K+k]);
                c2 = vfmaq_n_f32(c2, b_col, A[(i+2)*K+k]);
                c3 = vfmaq_n_f32(c3, b_col, A[(i+3)*K+k]);
            }
            vst1q_f32(&C[(i+0)*N+j], c0); vst1q_f32(&C[(i+1)*N+j], c1);
            vst1q_f32(&C[(i+2)*N+j], c2); vst1q_f32(&C[(i+3)*N+j], c3);
        }
    }
    for(int i=(M&~3); i<M; i++)
        for(int j=0; j<N; j++) {
            float s=0; for(int k=0; k<K; k++) s+=A[i*K+k]*B[k*N+j]; C[i*N+j]=s;
        }
    sink = C[0];
}

double bench(void (*fn)(float*,float*,float*,int,int,int), float* A, float* B, float* C, int M, int K, int N, int runs) {
    double total=0;
    for(int r=0; r<runs; r++) {
        clock_t start=clock();
        fn(A,B,C,M,K,N);
        total+=(double)(clock()-start)/CLOCKS_PER_SEC;
    }
    return total/runs*1000.0;
}

int main() {
    printf("URDHVA-TIRYAGBHYAM vs STANDARD MATMUL (ARM64 NEON)\n\n");
    printf("%-10s %-10s %-10s %-14s %-14s %-8s\n", "M", "K", "N", "Standard(ms)", "Urdhva(ms)", "Speedup");
    printf("----------------------------------------------------------\n");
    int sizes[][3] = {{128,128,128},{256,256,256},{512,512,512}};
    for(int s=0; s<3; s++) {
        int M=sizes[s][0], K=sizes[s][1], N=sizes[s][2];
        float *A=new float[M*K],*B=new float[K*N],*C1=new float[M*N],*C2=new float[M*N];
        for(int i=0;i<M*K;i++) A[i]=sinf(i*0.1f);
        for(int i=0;i<K*N;i++) B[i]=cosf(i*0.1f);
        double std_ms = bench(standard_matmul, A,B,C1, M,K,N, 5);
        double urd_ms = bench(urdhva_matmul, A,B,C2, M,K,N, 5);
        printf("%-10d %-10d %-10d %-14.2f %-14.2f %-8.2fx\n", M,K,N, std_ms, urd_ms, std_ms/urd_ms);
        delete[] A;delete[] B;delete[] C1;delete[] C2;
    }
    printf("\nUrdhva-Tiryagbhyam: 4x4 NEON tiled Vedic matmul\n");
    return 0;
}
