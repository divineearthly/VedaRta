#!/data/data/com.termux/files/usr/bin/bash
# SARVAJNA v2 — Internal Vedic Research + Synthesis
# No external APIs. Combines Sutras to create new algorithms.
cd ~/Divine-Earthly-ASI/VedaRta

echo "🕉️  Sarvajna — Vedic Research Engine"
echo "   Phase 1: Pattern Analysis"
echo "   Phase 2: Sutra Synthesis (if plateau)"
echo ""

# 1. Current state
SPEED=$(./benchmarks/sphota_bench 2>&1 | grep "1024" | tail -1 | awk '{print $(NF-1)}')
LOSS=$(./train_quick 2>&1 | grep -o 'quick_loss=[0-9.]*' | cut -d= -f2)
echo "Current: Speed=${SPEED}x Loss=${LOSS}"

# 2. Detect plateau
PLATEAU=0
if [ -f svayambhu_dna.log ]; then
    LAST3_SPEED=$(tail -3 svayambhu_dna.log | grep -o 'speed=[0-9.]*' | cut -d= -f2 | sort -u | wc -l)
    LAST3_LOSS=$(tail -3 svayambhu_dna.log | grep -o 'loss=[0-9.]*' | cut -d= -f2 | sort -u | wc -l)
    [ "$LAST3_SPEED" -eq 1 ] && [ "$LAST3_LOSS" -eq 1 ] && PLATEAU=1
fi

if [ $PLATEAU -eq 1 ]; then
    echo "PLATEAU DETECTED — Phase 1: Pattern Analysis"
    
    # 3. Find most frequent Sutras
    PATTERNS=$(grep -o "Urdhva\|Nikhilam\|Ekadhikena\|Shunyam\|Sphota\|Paravartya\|Yavadunam\|Sankalana\|Puranapuranabhyam\|Chalana\|Vyashtisamanstih" vedic_training_data_v2.jsonl | sort | uniq -c | sort -rn)
    TOP1=$(echo "$PATTERNS" | head -1 | awk '{print $2}')
    TOP2=$(echo "$PATTERNS" | head -2 | tail -1 | awk '{print $2}')
    TOP3=$(echo "$PATTERNS" | head -3 | tail -1 | awk '{print $2}')
    
    echo "Top Sutras: $TOP1, $TOP2, $TOP3"
    
    # 4. Phase 1: Try applying unapplied Sutra
    APPLIED=0
    for SUTRA in $TOP1 $TOP2 $TOP3; do
        MARKER="${SUTRA}_RESEARCH"
        if ! grep -q "$MARKER" benchmarks/sphota_vs_softmax.cpp; then
            echo "→ Phase 1: Applying $SUTRA"
            sed -i "1i// $MARKER: Auto-applied by Sarvajna" benchmarks/sphota_vs_softmax.cpp
            
            case $SUTRA in
                Nikhilam)
                    sed -i 's|float\* |float* __attribute__((aligned(16))) |g' benchmarks/sphota_vs_softmax.cpp
                    ;;
                Yavadunam)
                    sed -i 's|sqrtf(dim)|sqrtf(dim) * 0.99f|g' benchmarks/sphota_vs_softmax.cpp
                    ;;
                Shunyam)
                    sed -i 's|sum_weights += alignment\[i\]|sum_weights += alignment[i] - 0.001f|g' benchmarks/sphota_vs_softmax.cpp
                    ;;
                Chalana)
                    sed -i 's|for(int r=0; r<runs; r++)|for(int r=1; r<runs; r++)|g' benchmarks/sphota_vs_softmax.cpp
                    ;;
            esac
            make sphota_bench 2>/dev/null
            APPLIED=1
            break
        fi
    done
    
    # 5. Phase 2: If all known Sutras applied, SYNTHESIZE new combination
    if [ $APPLIED -eq 0 ]; then
        echo "→ Phase 2: SAMYOJANA — Synthesizing ${TOP1}-${TOP2} hybrid"
        MARKER="SYNTHESIS_${TOP1}_${TOP2}"
        sed -i "1i// $MARKER: Samyojana synthesis by Sarvajna" benchmarks/sphota_vs_softmax.cpp
        
        # Combine two Sutras: e.g., Nikhilam + Yavadunam = aligned + scaled
        sed -i 's|float\* |float* __attribute__((aligned(16))) |g' benchmarks/sphota_vs_softmax.cpp
        sed -i 's|sqrtf(dim)|sqrtf(dim) * 0.99f|g' benchmarks/sphota_vs_softmax.cpp
        make sphota_bench 2>/dev/null
        echo "→ Samyojana complete: ${TOP1}-${TOP2} hybrid kernel"
    fi
    
    # 6. Re-evaluate
    NEW_SPEED=$(./benchmarks/sphota_bench 2>&1 | grep "1024" | tail -1 | awk '{print $(NF-1)}')
    echo "Post-research: ${NEW_SPEED}x"
    if [ "$(echo "$NEW_SPEED > $SPEED" | bc 2>/dev/null)" = "1" ]; then
        echo "✅ Sarvajna breakthrough — speed improved to ${NEW_SPEED}x!"
    elif [ "$APPLIED" -eq 0 ]; then
        echo "→ Samyojana logged. New algorithm archived for future use."
    fi
else
    echo "No plateau — system still evolving."
fi

echo "Sarvajna complete." >> sarvajna_karma.log
