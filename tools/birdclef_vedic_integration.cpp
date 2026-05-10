#include <cstdio>
#include <cmath>
#include <cstring>

#define PHI 1.618033988749895f

struct VedicSwara {
    const char* name;
    float freq_hz;
    const char* emotion;
    const char* healing;
};

const VedicSwara SWARAS[] = {
    {"Sa (Shadja)",     261.63f, "Grounding",       "Root chakra"},
    {"Re (Rishabha)",   293.66f, "Awakening",       "Sacral chakra"},
    {"Ga (Gandhara)",   329.63f, "Joy",              "Solar plexus"},
    {"Ma (Madhyama)",   349.23f, "Love",             "Heart chakra"},
    {"Pa (Panchama)",   392.00f, "Communication",    "Throat chakra"},
    {"Dha (Dhaivata)",  440.00f, "Intuition",        "Third eye"},
    {"Ni (Nishada)",    493.88f, "Transcendence",    "Crown chakra"},
    {nullptr, 0, nullptr, nullptr}
};

const VedicSwara* match_swara(float freq) {
    while(freq > 500.0f) freq /= 2.0f;
    while(freq < 200.0f) freq *= 2.0f;
    const VedicSwara* best = &SWARAS[0];
    float best_diff = 999;
    for(int i=0; SWARAS[i].name; i++) {
        float d = fabsf(freq - SWARAS[i].freq_hz);
        if(d < best_diff) { best_diff = d; best = &SWARAS[i]; }
    }
    return best;
}

int main() {
    printf("🕉️  BIRDCLEF-VEDIC — Samaveda + 100 Sutras\n");
    printf("  ========================================\n\n");
    
    struct { const char* species; float freq; } birds[] = {
        {"Hyacinth Macaw",  4200}, {"Jabiru Stork",    1800},
        {"Toco Toucan",     2800}, {"Rufous Hornero",  2200},
        {"Pantanal Snipe",  5500}, {"Great Egret",     3100},
        {"Sunbittern",      3800}, {nullptr, 0}
    };
    
    for(int i=0; birds[i].species; i++) {
        const VedicSwara* s = match_swara(birds[i].freq);
        printf("  🐦 %-22s → %-18s (%.0f Hz) | %s | %s\n",
               birds[i].species, s->name, birds[i].freq, s->emotion, s->healing);
    }
    
    printf("\n  🕉️  Every bird song is a Vedic mantra. Every forest is a temple.\n");
    return 0;
}
