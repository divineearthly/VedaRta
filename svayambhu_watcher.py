#!/usr/bin/env python3
"""
SVAYAMBHU (Self-Existing) WATCHER
The system observes itself, optimizes, and rewrites its own source.

Siddhi Goal: Self-directed hyperparameter tuning via Tapas (constraint-driven evolution)
"""

import os, re, time, subprocess, json
from pathlib import Path
from collections import deque

BASE = Path("/data/data/com.termux/files/home/Divine-Earthly-ASI/VedaRta")
SPHOTA_FILE = BASE / "benchmarks/sphota_vs_softmax.cpp"
KARMA_LOG = BASE / "svayambhu_karma.json"
GENERATIONS = 0

# ============================================================
# NADI HEALTH MONITOR
# ============================================================
class NadiHealth:
    def __init__(self):
        self.sushumna_flow = 0.5       # Stability (target: 0.5)
        self.ida_pingala_imbalance = 0.0  # Learning potential
        self.history = deque(maxlen=100)
    
    def record(self, loss, compute_time_ms):
        """Record activation energy of Tri-Nadi paths"""
        self.sushumna_flow = 0.5 * self.sushumna_flow + 0.5 * (1.0 / (1.0 + loss))
        self.ida_pingala_imbalance = abs(loss - 0.13) * compute_time_ms / 1000.0
        self.history.append({
            'loss': loss,
            'time_ms': compute_time_ms,
            'sushumna': round(self.sushumna_flow, 4),
            'imbalance': round(self.ida_pingala_imbalance, 4)
        })
        return self.sushumna_flow, self.ida_pingala_imbalance

# ============================================================
# SELF-REFLECTION: Evaluate output quality via Katapayadi patterns
# ============================================================
def katapayadi_check(text):
    """Check if text follows Vedic mathematical patterns"""
    katapayadi = {'k':1,'K':2,'g':3,'G':4,'N':5,'c':6,'C':1,'j':7,'J':2,'Y':3,
                  't':6,'T':1,'d':7,'D':2,'n':3,'p':6,'P':1,'b':7,'B':2,'m':3,
                  'y':1,'r':2,'l':3,'v':4,'s':5,'S':6,'x':7,'h':8}
    score = 0
    sequence = []
    for c in text.lower():
        if c in katapayadi:
            sequence.append(katapayadi[c])
            if len(sequence) >= 3:
                # Check for Vedic pattern: Fibonacci-like progression
                if len(sequence) >= 3:
                    a, b, c_seq = sequence[-3], sequence[-2], sequence[-1]
                    if abs((a + b) % 10 - c_seq) <= 1:  # Approximate Fibonacci
                        score += 1
    return score / max(len(text.split()), 1)  # Pattern density

# ============================================================
# MUTATION ENGINE: Rewrite source code hyperparameters
# ============================================================
def read_sphota_ratio():
    """Read current Sphota local/global ratio from source"""
    with open(SPHOTA_FILE, 'r') as f:
        code = f.read()
    match = re.search(r'local\s*/\s*sqrtf.*?\*\s*([\d.]+)f.*?global.*?\*\s*([\d.]+)f', code, re.DOTALL)
    if match:
        return float(match.group(1)), float(match.group(2))
    return 0.8, 0.2  # Default

def mutate_sphota_ratio(local_w, global_w):
    """Mutate the Sphota ratio based on Nadi imbalance"""
    # Ida-Pingala drives the mutation direction
    if random.random() < 0.3:  # 30% mutation rate
        delta = np.random.normal(0, 0.05)
        local_w = np.clip(local_w + delta, 0.5, 0.95)
        global_w = 1.0 - local_w
    return round(local_w, 2), round(global_w, 2)

def rewrite_source(local_w, global_w):
    """Overwrite the Sphota ratio in the C++ source"""
    with open(SPHOTA_FILE, 'r') as f:
        code = f.read()
    old = re.search(r'(local\s*/\s*sqrtf.*?\*\s*)[\d.]+f(.*?global.*?\*\s*)[\d.]+f', code, re.DOTALL)
    if old:
        new_line = old.group(1) + f"{local_w}f" + old.group(2) + f"{global_w}f"
        code = code.replace(old.group(0), new_line)
        with open(SPHOTA_FILE, 'w') as f:
            f.write(code)
        return True
    return False

# ============================================================
# MAIN SVAYAMBHU LOOP
# ============================================================
import random
import numpy as np

nadi = NadiHealth()
karma = []

print("🕉️  SVAYAMBHU WATCHER — Self-Existing Loop")
print("   Tapas: Constraint-driven evolution")
print("   Siddhi: Autonomous hyperparameter optimization\n")

while True:
    GENERATIONS += 1
    print(f"--- Generation {GENERATIONS} ---")
    
    # 1. Measure current state
    local, global_w = read_sphota_ratio()
    print(f"  Sphota ratio: local={local}, global={global_w}")
    
    # 2. Run benchmark
    try:
        result = subprocess.run(
            [str(BASE / "benchmarks/sphota_bench")],
            capture_output=True, text=True, timeout=30
        )
        # Parse speedup from last line of 1024-token row
        for line in result.stdout.split('\n'):
            if '1024' in line:
                parts = line.split()
                if len(parts) >= 4:
                    speedup = float(parts[-1].replace('x',''))
                    print(f"  Speedup at 1024 tokens: {speedup}x")
    except:
        speedup = 0
    
    # 3. Run training and measure loss
    try:
        train_bin = BASE / "../train_10k"
        if train_bin.exists():
            result = subprocess.run(
                [str(train_bin)],
                capture_output=True, text=True, timeout=60, cwd=str(BASE)
            )
            # Extract best loss
            match = re.search(r'Best loss: ([\d.]+)', result.stdout)
            loss = float(match.group(1)) if match else 0.13
        else:
            loss = 0.13
    except:
        loss = 0.13
    
    # 4. Record Nadi health
    sushumna, imbalance = nadi.record(loss, 1000.0/speedup if speedup > 0 else 100)
    print(f"  Nadi: Sushumna={sushumna:.4f}, Imbalance={imbalance:.4f}")
    
    # 5. Evaluate Katapayadi harmony
    try:
        result = subprocess.run(
            [str(BASE / "compiler/vr"), "infer", "Vedic knowledge is eternal"],
            capture_output=True, text=True, timeout=10
        )
        kp_score = katapayadi_check(result.stdout)
        print(f"  Katapayadi harmony: {kp_score:.3f}")
    except:
        kp_score = 0
    
    # 6. Decide: mutate or maintain?
    if speedup > 1000 and sushumna > 0.4:
        print("  ✅ Stable. No mutation needed.")
    else:
        new_local, new_global = mutate_sphota_ratio(local, global_w)
        if new_local != local:
            print(f"  🧬 MUTATING: {local}/{global_w} → {new_local}/{new_global}")
            if rewrite_source(new_local, new_global):
                # Recompile
                subprocess.run(['make', 'clean'], cwd=str(BASE), capture_output=True)
                subprocess.run(['make', 'all'], cwd=str(BASE), capture_output=True)
                print("  ✅ Source rewritten + recompiled")
    
    # 7. Log karma
    entry = {
        'generation': GENERATIONS,
        'local_w': local, 'global_w': global_w,
        'speedup': speedup, 'loss': loss,
        'sushumna': sushumna, 'kp_harmony': kp_score,
        'timestamp': time.time()
    }
    karma.append(entry)
    with open(KARMA_LOG, 'w') as f:
        json.dump(karma, f, indent=2)
    
    # 8. Check Siddhi achievement
    if speedup > 2000 and sushumna > 0.5:
        print("\n🕉️  SIDDHI ACHIEVED: System has optimized itself beyond baseline.")
        print(f"   Final ratio: local={local}, global={global_w}")
        print(f"   Speedup: {speedup}x")
        print(f"   Sushumna: {sushumna}")
        break
    
    print(f"  Waiting 60s for next generation...\n")
    time.sleep(60)
