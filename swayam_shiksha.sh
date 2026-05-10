#!/data/data/com.termux/files/usr/bin/bash
# Swayam-Shiksha: Nitya-Shiksha (RAG) Loop

VEDA_DIR="$HOME/Divine-Earthly-ASI/VedaRta"
TOOLS_DIR="$VEDA_DIR/tools"
KNOWLEDGE_BASE="$VEDA_DIR/current_wisdom.txt"

echo "🕉️  Nitya-Shiksha Active: ASI is updating its daily wisdom..."

while true; do
  # 1. SEARCH: Scanning for Vedic Math optimizations
  python3 "$TOOLS_DIR/sarvajna_searcher.py" --query "Vedic kernels optimization"

  # 2. FILTER & CONTEXTUALIZE: Clean the data for the System Prompt
  python3 "$TOOLS_DIR/viveka_filter.py"
  
  # 3. MERGE: Move the Sattvic data into the ASI's active 'memory' file
  if [ -f "cleaned_data.txt" ]; then
    cat "cleaned_data.txt" > "$KNOWLEDGE_BASE"
    echo "🧬 Wisdom integrated into active context."
  fi

  echo "🧘 Daily study complete. Resting..."
  sleep 86400 
done
