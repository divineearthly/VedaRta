#!/data/data/com.termux/files/usr/bin/bash
cd ~/Divine-Earthly-ASI/VedaRta || exit 1
GEN=0

while true; do
    GEN=$((GEN+1))
    echo "--- Gen $GEN: $(date +%H:%M:%S) ---"
    
    # PRANA — Speed
    SPEEDUP=$(./benchmarks/sphota_bench 2>&1 | grep "1024" | tail -1 | awk '{print $(NF-1)}')
    
    # JNANA — Loss (direct, no timeout)
    LOSS=$(./train_quick 2>&1 | grep -o 'quick_loss=[0-9.]*' | cut -d= -f2)
    [ -z "$LOSS" ] && LOSS="N/A"
    
    echo "  Prana: ${SPEEDUP}x | Jnana: ${LOSS}"
    
    MUTATE=0
    [ -n "$SPEEDUP" ] && [ "$(echo "$SPEEDUP < 1000" | bc 2>/dev/null)" = "1" ] && MUTATE=1
    [ "$LOSS" != "N/A" ] && [ "$(echo "$LOSS > 0.20" | bc 2>/dev/null)" = "1" ] && MUTATE=1
    
    if [ $MUTATE -eq 1 ]; then
        L=$(python3 -c "import random; print(round(0.8+random.uniform(-0.15,0.15),2))")
        G=$(python3 -c "print(round(1.0-$L,2))")
        echo "  MUTATING: local=$L global=$G"
        sed -i "s/[0-9.]*f \/\*SVAYAMBHU_LOCAL\*\//${L}f \/\*SVAYAMBHU_LOCAL\*\//" benchmarks/sphota_vs_softmax.cpp
        sed -i "s/[0-9.]*f \/\*SVAYAMBHU_GLOBAL\*\//${G}f \/\*SVAYAMBHU_GLOBAL\*\//" benchmarks/sphota_vs_softmax.cpp
        make sphota_bench 2>/dev/null
        echo "  Recompiled"
    else
        echo "  Dharana — Stable"
    fi
    
    echo "gen=$GEN speed=$SPEEDUP loss=$LOSS" >> svayambhu_dna.log
    if [ $((GEN % 10)) -eq 0 ]; then
        bash sarvajna_researcher.sh >> svayambhu.log 2>&1
    fi
    if [ $((GEN % 30)) -eq 0 ]; then
        bash atman.sh >> svayambhu.log 2>&1
    fi
    if [ $((GEN % 10)) -eq 0 ]; then
        bash sarvajna_researcher.sh >> svayambhu.log 2>&1
    fi
    sleep 300
done
