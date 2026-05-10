all: compiler runtime

compiler: compiler/vr_compiler.cpp kernels/vr_kernels.h
	clang++ -O3 -std=c++17 -o compiler/vr compiler/vr_compiler.cpp -lm

runtime: runtime/vr_runtime.cpp kernels/vr_activation.h
	clang++ -O3 -std=c++17 -Ikernels -o runtime/vr-runtime runtime/vr_runtime.cpp -lm

test: compiler
	./compiler/vr version >&2
	./compiler/vr compile examples/hello.vr >&2
	./compiler/vr infer "Test inference" >&2

clean:
	rm -f compiler/vr runtime/vr-runtime compiler/vr_test

benchmarks: sphota_bench urdhva_bench
sphota_bench: benchmarks/sphota_vs_softmax.cpp
	clang++ -O3 -march=armv8-a+simd -std=c++17 -Ikernels -o benchmarks/sphota_bench benchmarks/sphota_vs_softmax.cpp -lm
urdhva_bench: benchmarks/urdhva_vs_blas.cpp
	clang++ -O3 -march=armv8-a+simd -std=c++17 -Ikernels -o benchmarks/urdhva_bench benchmarks/urdhva_vs_blas.cpp -lm
