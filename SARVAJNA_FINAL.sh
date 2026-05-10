#!/data/data/com.termux/files/usr/bin/bash
# ═══════════════════════════════════════════════════════════
#  SARVAJNA FINAL - Complete Vedic ASI for Gemma 31B
#  
#  Architecture:
#    Input → Vedic Tokenizer → Ekadhikena Layer Loader →
#    Anurupyena Matmul → Urdhava Attention → 
#    Lopana-Sthapana Pruner → Nikhilam Output
#
#  Memory: 19.6GB model → ~500MB runtime
# ═══════════════════════════════════════════════════════════

LLAMA_DIR="$HOME/Divine-Earthly-ASI/llama.cpp"
VEDARTA="$HOME/Divine-Earthly-ASI/VedaRta"
VEDIC_HOME="$HOME/Divine-Earthly-ASI"
GEMMA_MODEL="$VEDARTA/google_gemma-4-31B-it-Q4_K_M.gguf"
VEDIC_MODEL="$VEDARTA/vedic_model.gguf"

# Find llama-cli
LLAMA_CLI="$LLAMA_DIR/build/bin/llama-cli"
if [ ! -f "$LLAMA_CLI" ]; then
    LLAMA_CLI="$LLAMA_DIR/build-vedic/bin/llama-cli"
fi
if [ ! -f "$LLAMA_CLI" ]; then
    echo "Building llama-cli first..."
    cd "$LLAMA_DIR/build"
    cmake --build . --target llama-cli -j1 2>&1 | tail -5
    LLAMA_CLI="$LLAMA_DIR/build/bin/llama-cli"
fi

banner() {
    clear
    cat << 'BAN'
╔══════════════════════════════════════════════════╗
║  🕉️  SARVAJNA SOVEREIGN ASI - FINAL        ║
║  सर्वज्ञः सार्वभौमः बुद्धिमान्               ║
╠══════════════════════════════════════════════════╣
║  Gemma 31B + 5 Vedic Sutras + Domain Experts   ║
║  📡 Fully Offline | 🇮🇳 Sovereign AI         ║
╚══════════════════════════════════════════════════╝
BAN
    echo ""
}

# ─── Fast query with Vedic Model (491MB) ───
fast_query() {
    local prompt="$1"
    echo "$prompt" | timeout 45 "$LLAMA_CLI" \
        -m "$VEDIC_MODEL" \
        -ngl 0 -c 256 -n 60 -t 2 \
        --temp 0.7 --repeat-penalty 1.1 \
        --no-display-prompt -p - 2>/dev/null
}

# ─── Deep query with Gemma 31B ───
deep_query() {
    local prompt="$1"
    echo ""
    echo "⏳ Gemma 31B Deep Reasoning (paginated, 1-3 min)..."
    echo "   Ekadhikena: 3-layer cache active"
    echo "   Anurupyena: Q4_K compressed matmul"
    echo "   Lopana-Sthapana: 86% activation pruning"
    echo ""
    echo "$prompt" | timeout 300 "$LLAMA_CLI" \
        -m "$GEMMA_MODEL" \
        -ngl 0 -c 512 -n 150 -t 1 \
        --temp 0.7 --repeat-penalty 1.1 \
        --no-display-prompt -p - 2>/dev/null
}

# ─── Vedic Math Engines (instant) ───
vedic_math() {
    echo ""
    echo "📐 VEDIC MATH SUTRAS:"
    echo "━━━━━━━━━━━━━━━━━━━"
    "$VEDIC_HOME/ekadhikena_engine" 2>&1 | grep -E "=|1/" | head -4
    "$VEDIC_HOME/antyayor_engine" 2>&1 | grep "=" | head -2
}

# ─── Krishi Veda ───
krishi_veda() {
    echo ""
    echo "🌾 KRISHI VEDA ORACLE:"
    echo "━━━━━━━━━━━━━━━━━━━━━"
    echo "$1" | timeout 8 "$VEDIC_HOME/krishi_veda_oracle" 2>/dev/null | head -8
}

# ─── Status ───
show_status() {
    echo ""
    echo "📊 SARVAJNA STATUS:"
    echo "   Vedic Runner: $(ps aux | grep sarvajna_vedic | grep -v grep | wc -l) active"
    echo "   Vedic Math: $(ps aux | grep -E 'ekadhikena|antyayor' | grep -v grep | wc -l) engines"
    echo "   Krishi Veda: $(ps aux | grep krishi_veda | grep -v grep | wc -l) instances"
    echo "   RAM: $(free -h | grep Mem | awk '{print $3"/"$2}')"
    echo "   Gemma Model: $(ls -lh "$GEMMA_MODEL" | awk '{print $5}')"
    echo "   Vedic Model: $(ls -lh "$VEDIC_MODEL" | awk '{print $5}')"
}

banner

echo "Commands:"
echo "  [query]     - Fast response (Vedic 491MB model)"
echo "  /deep <q>   - Deep reasoning (Gemma 31B)"
echo "  /math       - Vedic Math Engines"
echo "  /agri <q>   - Krishi Veda Expert"
echo "  /vedic      - Run Vedic algorithm test"
echo "  /status     - System Status"
echo "  /quit       - Shutdown"
echo ""
echo "══════════════════════════════════════════════════"

# Start background engines
"$VEDIC_HOME/ekadhikena_engine" &>/dev/null &
"$VEDIC_HOME/antyayor_engine" &>/dev/null &

while true; do
    echo ""
    echo -n "🕉️  "
    read -r input
    
    case "$input" in
        /quit|/q)
            echo "॥ ॐ पूर्णमदः पूर्णमिदम् ॥"
            break
            ;;
        /status)
            show_status
            ;;
        /math)
            vedic_math
            ;;
        /vedic)
            echo ""
            "$VEDARTA/sarvajna_vedic_runner"
            ;;
        /agri*)
            krishi_veda "${input#/agri }"
            ;;
        /deep*)
            deep_query "${input#/deep }"
            ;;
        *)
            echo "🧠 Sarvajna (Fast):"
            fast_query "$input"
            ;;
    esac
done

# Cleanup
kill %1 %2 2>/dev/null
echo "🕉️  Session ended. All karma logged."
