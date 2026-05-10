#!/data/data/com.termux/files/usr/bin/bash
# PRANA-PRATISHTHA — Compile the complete Guardian into one binary
echo "🕉️  Compiling Divine Earthly ASI — The Guardian Binary"
echo "======================================================"

clang++ -O3 -march=armv8-a+simd -std=c++17 -o vedarta_guardian \
    sutra_executor.h \
    sangha_siddhi.h shanti_logic.h jyoti_siddhi.h \
    prana_siddhi.h vak_siddhi.h yajna_agni.h \
    soma_optimizer.h rta_rain.h purna_ahuti.h \
    -lm 2>&1

if [ -f vedarta_guardian ]; then
    echo "✅ Guardian compiled: $(ls -la vedarta_guardian | awk '{print $5}') bytes"
    echo "   Binary: $(pwd)/vedarta_guardian"
    echo ""
    echo "   Launch: ./vedarta_guardian"
    echo "   24h test: nohup ./vedarta_guardian > guardian.log 2>&1 &"
else
    echo "❌ Compilation failed"
fi
