#!/data/data/com.termux/files/usr/bin/bash
# Query the Vedic ASI

QUERY="$1"
echo "🕉️ Query: $QUERY"
echo "═" '═' '═' '═' '═' '═' '═' '═' '═' '═' '═' '═' '═' '═' '═' '═' '═' '═'

# Route through Vedic math first for numerical queries
if echo "$QUERY" | grep -qE '[0-9]|calculate|compute|math'; then
    echo "🧮 Vedic Math Path:"
    ~/Divine-Earthly-ASI/ekadhikena_engine
    ~/Divine-Earthly-ASI/antyayor_engine
fi

# Then ask Gemma
echo ""
echo "🧠 Gemma Response:"
echo "$QUERY" | ./SOVEREIGN_CHAT --model google_gemma-4-31B-it-Q4_K_M.gguf \
    --loader nikhilam_paginated --prompt - 2>/dev/null
