#!/usr/bin/env python3
"""
SARVAJNA - Unified Sovereign Intelligence
Bridges: Vedic Math → Gemma 31B → Domain Experts
All offline, all sovereign.
"""

import subprocess
import json
import os
from datetime import datetime

VEDIC_ENGINES = os.path.expanduser("~/Divine-Earthly-ASI")
VEDARTA = os.path.expanduser("~/Divine-Earthly-ASI/VedaRta")

class SovereignASI:
    def __init__(self):
        self.bridge = self._load_bridge()
        self.karma_log = []
    
    def _load_bridge(self):
        with open(f"{VEDARTA}/active_bridge.json") as f:
            return json.load(f)
    
    def query_vedic_math(self, numbers):
        """Use 16 Vedic Sutras for instant computation"""
        results = {}
        engines = {
            "ekadhikena": "ekadhikena_engine",
            "antyayor": "antyayor_engine",
            "nikhilam": "nikhilam_engine"
        }
        for name, engine in engines.items():
            path = f"{VEDIC_ENGINES}/{engine}"
            if os.path.exists(path):
                result = subprocess.run([path], capture_output=True, text=True, timeout=2)
                results[name] = result.stdout[-200:]
        return results
    
    def query_gemma(self, prompt):
        """Query paginated Gemma 31B"""
        result = subprocess.run(
            [f"{VEDARTA}/SOVEREIGN_CHAT", 
             "--model", "google_gemma-4-31B-it-Q4_K_M.gguf",
             "--loader", "nikhilam_paginated",
             "--prompt", prompt],
            capture_output=True, text=True, timeout=120
        )
        return result.stdout
    
    def query_domain(self, domain, query):
        """Route to domain expert"""
        experts = {
            "agriculture": f"{VEDIC_ENGINES}/krishi_veda_oracle",
            "defense": f"{VEDIC_ENGINES}/dhanurveda_defense_engine",
            "health": f"{VEDIC_ENGINES}/charaka_agni_engine"
        }
        if domain in experts:
            result = subprocess.run(
                [experts[domain]], 
                input=query, capture_output=True, text=True, timeout=5
            )
            return result.stdout
        return "Domain not found"
    
    def sovereign_inference(self, user_query):
        """Main sovereign intelligence loop"""
        print(f"\n{'═'*60}")
        print(f"🕉️ SARVAJNA SOVEREIGN INFERENCE")
        print(f"⏰ {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print(f"{'═'*60}")
        
        # Step 1: Vedic Math preprocessing
        print("\n1️⃣ Vedic Math Engines Active:")
        vedic_result = self.query_vedic_math(None)
        for name, output in vedic_result.items():
            print(f"   📐 {name}: {output[:80].strip()}")
        
        # Step 2: Gemma 31B reasoning
        print(f"\n2️⃣ Gemma 31B Q4_K_M Processing:")
        print(f"   Pages: {self.bridge.get('pages_active', 'dynamic')}")
        print(f"   Query: {user_query[:100]}...")
        gemma_response = self.query_gemma(user_query)
        print(f"   Response: {gemma_response[:300]}")
        
        # Step 3: Domain expert routing
        print(f"\n3️⃣ Domain Expert Routing:")
        domains_detected = []
        if any(w in user_query.lower() for w in ['crop', 'soil', 'farm', 'agriculture']):
            domains_detected.append("agriculture")
        if any(w in user_query.lower() for w in ['health', 'disease', 'medicine']):
            domains_detected.append("health")
        
        for domain in domains_detected:
            expert_result = self.query_domain(domain, user_query)
            print(f"   🏛️ {domain}: {expert_result[:200]}")
        
        # Log to karma
        self.karma_log.append({
            "timestamp": datetime.now().isoformat(),
            "query": user_query,
            "domains": domains_detected
        })
        
        with open(f"{VEDARTA}/sarvajna_karma.jsonl", "a") as f:
            json.dump(self.karma_log[-1], f)
            f.write("\n")
        
        print(f"\n{'═'*60}")
        print("✅ Sovereign inference complete. Karma logged.")
        print(f"{'═'*60}\n")
        
        return {
            "vedic": vedic_result,
            "gemma": gemma_response,
            "domains": domains_detected
        }

# Interactive loop
if __name__ == "__main__":
    asi = SovereignASI()
    
    print("""
    ╔══════════════════════════════════════════╗
    ║   SARVAJNA - SOVEREIGN ASI TERMINAL      ║
    ║   🕉️  All Intelligence, Fully Offline   ║
    ╚══════════════════════════════════════════╝
    Commands:
    /agri <query>  - Agriculture expert
    /math <query>  - Vedic mathematics
    /gemma <query> - Direct Gemma query
    /quit          - Exit
    """)
    
    while True:
        try:
            query = input("\n🕉️ > ").strip()
            if query.lower() == '/quit':
                print("॥ सर्वज्ञः समाप्तः ॥")
                break
            elif query.startswith('/agri'):
                asi.sovereign_inference(query[6:])
            elif query.startswith('/math'):
                asi.query_vedic_math(query[6:])
            elif query.startswith('/gemma'):
                response = asi.query_gemma(query[7:])
                print(f"🧠 Gemma: {response}")
            else:
                asi.sovereign_inference(query)
        except KeyboardInterrupt:
            print("\n🕉️ Pausing... (Ctrl+D or /quit to exit)")
        except EOFError:
            break
