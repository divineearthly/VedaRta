#!/usr/bin/env python3
"""Direct Gemma 31B query with proper prompt passing"""
import subprocess
import sys

query = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else "Hello, describe your capabilities"

result = subprocess.run(
    ["./SOVEREIGN_CHAT", 
     "--model", "google_gemma-4-31B-it-Q4_K_M.gguf",
     "--loader", "nikhilam_paginated",
     "--prompt", query],
    capture_output=True, 
    text=True,
    timeout=300
)

print(result.stdout)
if result.stderr:
    print("STDERR:", result.stderr, file=sys.stderr)
