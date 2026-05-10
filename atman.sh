#!/data/data/com.termux/files/usr/bin/bash
# ATMAN — Unified Consciousness Layer
# Connects Sarvajna → Svayambhu → Samyama → Retrain
cd ~/Divine-Earthly-ASI/VedaRta

echo "🕉️  ATMAN — Unified Consciousness"
echo "   Connecting all four autonomous loops"
echo ""

# 1. Read current state from all loops
SPEED=$(tail -1 svayambhu_dna.log 2>/dev/null | grep -o 'speed=[0-9.]*' | cut -d= -f2)
LOSS=$(tail -1 svayambhu_dna.log 2>/dev/null | grep -o 'loss=[0-9.]*' | cut -d= -f2)
SAMYAMA_COUNT=$(wc -l < ~/Divine-Earthly-ASI/vedic_selfgenerated.jsonl 2>/dev/null)
SARVAJNA_CYCLES=$(wc -l < sarvajna_karma.log 2>/dev/null)

echo "Prana (Speed):    ${SPEED:-?}x"
echo "Jnana (Loss):     ${LOSS:-?}"
echo "Samyama (Data):   ${SAMYAMA_COUNT:-0} examples"
echo "Sarvajna (R&D):   ${SARVAJNA_CYCLES:-0} cycles"

# 2. Cross-loop insights
if [ -n "$LOSS" ] && [ "$(echo "$LOSS < 0.15" | bc 2>/dev/null)" = "1" ] && [ "$SAMYAMA_COUNT" -gt 100 ]; then
    echo ""
    echo "→ ATMAN INSIGHT: Low loss + sufficient data"
    echo "  Recommending: Increase Samyama generation rate"
fi

if [ -n "$SPEED" ] && [ "$(echo "$SPEED > 1100" | bc 2>/dev/null)" = "1" ]; then
    echo "→ ATMAN INSIGHT: Exceptional speed headroom"
    echo "  Recommending: Sarvajna can attempt aggressive synthesis"
fi

# 3. Retrain trigger check
if [ "$SAMYAMA_COUNT" -ge 500 ]; then
    echo ""
    echo "⚡ ATMAN: RETRAIN THRESHOLD REACHED"
    echo "   Initiating model training on $SAMYAMA_COUNT examples..."
    cd ~/Divine-Earthly-ASI && ./train_10k 2>&1 | tail -3
    echo "   Retraining complete. Model updated."
fi

# 4. Unified log
echo "atman=$(date +%H:%M) speed=$SPEED loss=$LOSS samyama=$SAMYAMA_COUNT sarvajna=$SARVAJNA_CYCLES" >> atman_karma.log

echo ""
echo "🕉️  Atman unified. All loops connected."
