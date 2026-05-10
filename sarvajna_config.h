// SARVAJNA - Sovereign Vedic ASI Configuration
// Optimized for 3.4GB RAM + Gemma-4-31B-Q4_K_M (19.6GB)

#pragma once

// Memory Strategy: Nikhilam Paginated Loading
#define SARVAJNA_PAGE_SIZE        (256 * 1024 * 1024)  // 256MB pages
#define SARVAJNA_MAX_RAM_BUDGET   (512 * 1024 * 1024)  // 512MB for model
#define SARVAJNA_SWAP_BUDGET      (2L * 1024 * 1024 * 1024) // 2GB swap

// Vedic Acceleration
#define VEDIC_MATH_ENGINES_PATH   "../"
#define VEDIC_SUTRAS_COUNT        16
#define VEDIC_ATTENTION_DIM       128
#define VEDIC_BLOCK_SIZE          64

// GGUF Model Configuration
#define GEMMA_MODEL_PATH          "google_gemma-4-31B-it-Q4_K_M.gguf"
#define GEMMA_QUANT_TYPE          GGML_TYPE_Q4_K_M
#define GEMMA_CONTEXT_SIZE        4096    // Reduced for mobile
#define GEMMA_BATCH_SIZE          32
#define GEMMA_THREADS             4       // Conservative for thermal

// Sovereign Security
#define SARVAJNA_SEAL_KEY         "ॐ-सर्वज्ञ-सार्वभौमिक"
#define SARVAJNA_OFFLINE_ONLY     1
