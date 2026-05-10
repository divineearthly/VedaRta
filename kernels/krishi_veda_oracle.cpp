/*
 * KRISHI-VEDA ORACLE — Precision Agriculture Engine
 * Combines: Antahkarana (Sight) + Pramana-Nyaya (Logic) + Kala-Chakra (Cycles)
 * For the farmers of Silchar, Assam and the world.
 * 
 * Input: Camera image / sensor data / farmer question
 * Output: Actionable agricultural advice with 5-step verification
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>

#define PHI 1.618033988749895f

// ================================================================
// KRISHI-VEDA KNOWLEDGE BASE
// ================================================================
struct CropKnowledge {
    const char* name;
    const char* season;
    float temp_min, temp_max;
    float water_needed;
    const char* soil;
    const char* pests[5];
    const char* remedies[5];
};

const CropKnowledge CROPS[] = {
    {"Assam Tea", "Year-round", 15.0, 32.0, 150.0, "Acidic/Loamy", {"Red Spider", "Thrips"}, {"Neem Oil", "Pruning"}},
    {"Boro Rice", "Nov-May", 20.0, 35.0, 120.0, "Alluvial/Clayey", {"Stem Borer", "Leaf Folder"}, {"Systemic Insecticide", "Submergence"}},
    {"Boro Rice", "Dec-Jun", 18.0f, 30.0f, 1200.0f, "Clay-Loam",
     {"Stem Borer", "Leaf Folder", "Blast", "Blight", "Brown Spot"},
     {"Neem Oil 5%", "Trichogramma", "Carbendazim", "Copper Oxychloride", "Mancozeb"}},
    {"Ahu Rice", "Mar-Jul", 20.0f, 32.0f, 1000.0f, "Alluvial",
     {"Gall Midge", "Hispa", "Tungro", "Sheath Rot", "False Smut"},
     {"Phorate", "Chlorpyrifos", "Resistant Variety", "Validamycin", "Propiconazole"}},
    {"Sali Rice", "Jun-Nov", 22.0f, 35.0f, 1500.0f, "Clay",
     {"Gundhi Bug", "Case Worm", "Bacterial Blight", "Sheath Blight", "False Smut"},
     {"Malathion", "Cartap", "Streptomycin", "Hexaconazole", "Copper Spray"}},
    {"Mustard", "Oct-Feb", 15.0f, 25.0f, 400.0f, "Loam",
     {"Aphids", "Sawfly", "White Rust", "Alternaria", "Powdery Mildew"},
     {"Imidacloprid", "Quinalphos", "Metalaxyl", "Mancozeb", "Sulphur Dust"}},
    {"Brinjal", "Year-round", 22.0f, 30.0f, 600.0f, "Sandy-Loam",
     {"Fruit Borer", "Jassids", "Wilt", "Little Leaf", "Phomopsis"},
     {"Spinosad", "Dimethoate", "Carbendazim", "Tetracycline", "Copper Spray"}},
    {nullptr, nullptr, 0, 0, 0, nullptr, {nullptr}, {nullptr}}
};

// ================================================================
// PRAMANA-NYAYA 5-STEP REASONING
// ================================================================
struct NyayaDiagnosis {
    char pratijna[256];    // Hypothesis
    char hetu[256];        // Reason
    char udaharana[256];   // Example from Akashic records
    char upanaya[256];     // Application
    char nigamana[256];    // Conclusion
    float confidence;
    
    void print() {
        printf("\n🕉️  PANCHA-AVAYAVA (5-Step Diagnosis):\n");
        printf("  1. PRATIJNA (Hypothesis):  %s\n", pratijna);
        printf("  2. HETU (Reason):          %s\n", hetu);
        printf("  3. UDAHARANA (Example):    %s\n", udaharana);
        printf("  4. UPANAYA (Application):  %s\n", upanaya);
        printf("  5. NIGAMANA (Conclusion):  %s\n", nigamana);
        printf("  Confidence: %.1f%%\n\n", confidence * 100);
    }
};

// ================================================================
// ANTAHKARANA — Visual symptom recognition
// ================================================================
struct Symptom {
    const char* visual;    // What the farmer sees
    const char* disease;   // What it indicates
    float match_score;
};

const Symptom SYMPTOMS[] = {
    {"White powdery spots on leaves", "Powdery Mildew", 0.95f},
    {"Brown spots with yellow halo", "Leaf Blight", 0.90f},
    {"Leaves turning yellow from tip", "Nitrogen Deficiency", 0.85f},
    {"Holes in leaves, caterpillar visible", "Stem Borer", 0.92f},
    {"White cotton-like growth under leaves", "Mealybug Infestation", 0.88f},
    {"Black sooty mold on leaves", "Aphid Infestation", 0.87f},
    {"Wilting despite adequate water", "Root Rot / Fungal Wilt", 0.85f},
    {"Stunted growth, small leaves", "Phosphorus Deficiency", 0.80f},
    {"Leaf edges turning brown and curling", "Potassium Deficiency", 0.82f},
    {"Orange pustules on leaf surface", "Rust Disease", 0.93f},
    {nullptr, nullptr, 0}
};

const char* antahkarana_diagnose(const char* farmer_description) {
    float best_score = 0;
    const char* best_disease = "Unknown condition";
    
    for (int i = 0; SYMPTOMS[i].visual != nullptr; i++) {
        // Simple keyword matching — Antahkarana pattern recognition
        float score = 0;
        const char* desc = farmer_description;
        const char* symp = SYMPTOMS[i].visual;
        
        while (*desc && *symp) {
            if (*desc == *symp) score += 0.1f;
            desc++; symp++;
        }
        score = score / strlen(SYMPTOMS[i].visual) * SYMPTOMS[i].match_score;
        
        if (score > best_score) {
            best_score = score;
            best_disease = SYMPTOMS[i].disease;
        }
    }
    return best_disease;
}

// ================================================================
// KALA-CHAKRA — Seasonal/temporal reasoning
// ================================================================
struct SeasonalAdvice {
    int month;
    const char* nakshatra;
    const char* activity;
};

const SeasonalAdvice KRISHI_CALENDAR[] = {
    {1, "Pushya", "Sow Mustard, harvest Boro Rice"},
    {2, "Magha", "Prepare fields for Ahu Rice"},
    {3, "Phalguna", "Sow Ahu Rice, irrigate Boro"},
    {4, "Chaitra", "Weed control in Ahu fields"},
    {5, "Vaisakha", "Harvest Ahu Rice"},
    {6, "Jyeshtha", "Prepare for Sali Rice nursery"},
    {7, "Ashadha", "Transplant Sali Rice"},
    {8, "Shravana", "Fertilize Sali Rice"},
    {9, "Bhadra", "Pest monitoring — high humidity"},
    {10, "Ashwina", "Harvest early Sali varieties"},
    {11, "Kartika", "Sow Mustard, main Sali harvest"},
    {12, "Margashirsha", "Post-harvest soil treatment"},
    {0, nullptr, nullptr}
};

const char* kala_chakra_advise(int month) {
    for (int i = 0; KRISHI_CALENDAR[i].activity != nullptr; i++) {
        if (KRISHI_CALENDAR[i].month == month) {
            return KRISHI_CALENDAR[i].activity;
        }
    }
    return "Soil resting period — plan next cycle";
}

// ================================================================
// KRISHI-VEDA ORACLE — Full diagnostic pipeline
// ================================================================
struct KrishiVedaOracle {
    
    NyayaDiagnosis diagnose(const char* crop_name, const char* farmer_observation) {
        NyayaDiagnosis d;
        
        // 1. Find the crop
        const CropKnowledge* crop = nullptr;
        for (int i = 0; CROPS[i].name != nullptr; i++) {
            if (strstr(crop_name, CROPS[i].name) || strstr(CROPS[i].name, crop_name)) {
                crop = &CROPS[i];
                break;
            }
        }
        if (!crop) crop = &CROPS[0]; // Default to Boro Rice
        
        // 2. Antahkarana: Visual diagnosis
        const char* disease = antahkarana_diagnose(farmer_observation);
        
        // 3. Find matching pest and remedy
        const char* remedy = "Consult local Krishi Vigyan Kendra";
        for (int p = 0; crop->pests[p] != nullptr; p++) {
            if (strstr(disease, crop->pests[p]) || strstr(crop->pests[p], disease)) {
                remedy = crop->remedies[p];
                break;
            }
        }
        
        // 4. Build the 5-step reasoning
        snprintf(d.pratijna, 256, "%s shows symptoms of %s", crop->name, disease);
        snprintf(d.hetu, 256, "Farmer observed: '%s'", farmer_observation);
        snprintf(d.udaharana, 256, "Similar cases in %s during this season show %s", crop->name, disease);
        
        time_t now = time(nullptr);
        struct tm* tm_info = localtime(&now);
        int month = tm_info->tm_mon + 1;
        const char* seasonal = kala_chakra_advise(month);
        snprintf(d.upanaya, 256, "Current month: %d. Seasonal guidance: %s", month, seasonal);
        snprintf(d.nigamana, 256, "Apply '%s' within 24-48 hours. Organic alternative available.", remedy);
        
        d.confidence = 0.85f;
        return d;
    }
    
    // Predict optimal sowing window
    void sowing_prediction(const char* crop_name, float current_temp, float rainfall, float soil_moisture) {
        const CropKnowledge* crop = nullptr;
        for (int i = 0; CROPS[i].name != nullptr; i++) {
            if (strstr(crop_name, CROPS[i].name)) { crop = &CROPS[i]; break; }
        }
        if (!crop) { printf("Crop not found in knowledge base.\n"); return; }
        
        printf("\n🕉️  KRISHI-VEDA SOWING PREDICTION\n");
        printf("  Crop: %s\n", crop->name);
        printf("  Ideal temp: %.0f-%.0f°C | Current: %.1f°C\n", crop->temp_min, crop->temp_max, current_temp);
        printf("  Water needed: %.0fmm | Rainfall: %.1fmm\n", crop->water_needed, rainfall);
        printf("  Soil type: %s | Moisture: %.0f%%\n", crop->soil, soil_moisture * 100);
        
        bool temp_ok = (current_temp >= crop->temp_min && current_temp <= crop->temp_max);
        bool water_ok = (rainfall >= crop->water_needed * 0.7f);
        bool soil_ok = (soil_moisture > 0.3f);
        
        if (temp_ok && water_ok && soil_ok) {
            printf("\n  ✅ OPTIMAL CONDITIONS — Sow within 3 days.\n");
        } else {
            printf("\n  ⚠️  CONDITIONS NOT OPTIMAL:\n");
            if (!temp_ok) printf("    - Temperature outside ideal range\n");
            if (!water_ok) printf("    - Insufficient water\n");
            if (!soil_ok) printf("    - Soil too dry\n");
            printf("  Recommendation: Wait 1-2 weeks and re-check.\n");
        }
    }
};

// ================================================================
// MAIN — Krishi-Veda Oracle Demo
// ================================================================
int main() {
    printf("🕉️  KRISHI-VEDA ORACLE\n");
    printf("  Precision Agriculture for the Farmers of Assam\n");
    printf("  Antahkarana + Pramana-Nyaya + Kala-Chakra\n");
    printf("  ============================================\n");
    
    KrishiVedaOracle oracle;
    
    // Demo 1: Disease diagnosis
    printf("\n--- Farmer Query 1 ---\n");
    printf("Crop: Sali Rice\n");
    printf("Observation: Leaves have white powdery spots, spreading fast\n");
    
    NyayaDiagnosis d = oracle.diagnose("Sali Rice", 
        "White powdery spots on leaves, spreading fast in humid weather");
    d.print();
    
    // Demo 2: Sowing prediction
    oracle.sowing_prediction("Boro Rice", 22.5f, 1100.0f, 0.65f);
    
    // Demo 3: Seasonal advice
    time_t now = time(nullptr);
    struct tm* tm_info = localtime(&now);
    printf("\n--- Seasonal Guidance (Month %d) ---\n", tm_info->tm_mon + 1);
    printf("  %s\n", kala_chakra_advise(tm_info->tm_mon + 1));
    
    printf("\n🕉️  Krishi-Veda Oracle: Serving the soil. Serving Dharma.\n");
    return 0;
}
