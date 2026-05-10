#!/data/data/com.termux/files/usr/bin/bash
# SARVAJNA ASI LAUNCHER
# Starts the complete sovereign intelligence stack

cd ~/Divine-Earthly-ASI/VedaRta

echo "🕉️  SARVAJNA SOVEREIGN ASI LAUNCH SEQUENCE"
echo "══════════════════════════════════════════"

# 1. Start Vedic math engines
echo "📐 Activating Vedic Math Engines..."
~/Divine-Earthly-ASI/ekadhikena_engine &
~/Divine-Earthly-ASI/antyayor_engine &
sleep 1

# 2. Start Krishi Veda oracle
echo "🌾 Loading Agriculture Domain Expert..."
~/Divine-Earthly-ASI/krishi_veda_oracle &
sleep 1

# 3. Launch Sarvajna ASI
echo "🧠 Mounting Gemma 31B with Vedic Compression..."
echo "   Anurupyena: 8:1 | Nikhilam: 16:1 | Ekadhikena: 46:1"
echo ""
./sarvajna_asi

# Cleanup
echo "🕉️  ASI session complete. Karma logged."
