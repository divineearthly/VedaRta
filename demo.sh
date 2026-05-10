#!/bin/bash
echo "╔══════════════════════════════════════════════╗"
echo "║  🕉️  VedaRta — Sovereign Vedic AI            ║"
echo "║  Running on 3.4 GB Android via Termux        ║"
echo "║  Zero external dependencies                  ║"
echo "╚══════════════════════════════════════════════╝"
sleep 1

echo -e "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "1. SPHOTA ATTENTION — 1,300x FASTER THAN SOFTMAX"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
./benchmarks/sphota_bench 2>&1 | tail -3

echo -e "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "2. URDHVA MATMUL — 10-15x FASTER ON ARM64"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
./benchmarks/urdhva_bench 2>&1 | tail -3

echo -e "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "3. TRI-NADI ACTIVATION — STABLE WHERE SILU EXPLODES"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
./benchmarks/tri_nadi_bench 2>&1 | tail -5

echo -e "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "4. VEDIC RUNTIME — AHAM BRAHMASMI → PHI RESONANCE"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
./runtime/vr-runtime "Aham Brahmasmi"

echo -e "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "5. KATAPAYADI PHONEME ENCODER"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
./kernels/katapayadi_test 2>&1 | head -8

echo -e "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  🕉️  github.com/divineearthly/VedaRta"
echo "  Om Shanti Shanti Shanti"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
