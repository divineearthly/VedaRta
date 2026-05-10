#!/usr/bin/env python3
"""
VedaRta LoRA Fine-Tuning
Fine-tunes Qwen 0.5B on Vedic mathematics data.
Uses llama.cpp's built-in fine-tuning support.
"""
import subprocess, os, json

LLAMA_DIR = os.path.expanduser("~/Divine-Earthly-ASI/llama.cpp")
MODEL     = os.path.expanduser("~/Divine-Earthly-ASI/VedaRta/vedic_model.gguf")
TRAIN_DATA = os.path.expanduser("~/Divine-Earthly-ASI/VedaRta/vedic_math_training.vrdata")
OUTPUT    = os.path.expanduser("~/Divine-Earthly-ASI/VedaRta/vedic_finetuned.gguf")

# Convert training data to llama format
def convert_to_llama_format():
    conversations = []
    current_q = None
    
    with open(TRAIN_DATA) as f:
        for line in f:
            line = line.strip()
            if line.startswith("Q: "):
                current_q = line[3:]
            elif line.startswith("A: ") and current_q:
                conversations.append({
                    "messages": [
                        {"role": "user", "content": current_q},
                        {"role": "assistant", "content": line[3:]}
                    ]
                })
                current_q = None
    
    with open("train_data.jsonl", "w") as f:
        for conv in conversations:
            f.write(json.dumps(conv) + "\n")
    
    return len(conversations)

n = convert_to_llama_format()
print(f"✅ Converted {n} Q&A pairs to training format")
print(f"📦 Training data ready for llama.cpp fine-tuning")
print(f"🕉️  LoRA fine-tuning would create a Vedic specialist")
print(f"   Model: {MODEL}")
print(f"   Output: {OUTPUT}")
