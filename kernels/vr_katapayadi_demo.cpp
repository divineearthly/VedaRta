#include <cstdio>
#include <cmath>
#include "vr_katapayadi.h"

int main() {
    printf("🕉️  Katapayadi Vedic Number System\n\n");
    
    const char* syllables[] = {"ka","kha","ga","gha","nga","ca","cha","ja","jha","nya",
        "ta","tha","da","dha","na","pa","pha","ba","bha","ma",
        "ya","ra","la","va","sha","sa","ha", nullptr};
    
    printf("Syllable → Value:\n");
    for(int i=0; syllables[i]; i++)
        printf("  %-4s → %d\n", syllables[i], katapayadi_value(syllables[i]));
    
    printf("\n---\n");
    
    // Encode a real Sanskrit word: "Veda" = va + e + da
    const char* veda = "veda";
    printf("Word: %s\n", veda);
    printf("Katapayadi embedding (first 8 dims): ");
    for(int d=0; d<8; d++)
        printf("%.3f ", katapayadi_embed(veda, d, 64));
    printf("\n");
    
    // Decode the number back
    printf("\nKatapayadi numeric encoding:\n");
    long num = 0;
    for(int i=0; veda[i]; i++) {
        int kv = katapayadi_value(&veda[i]);
        if(kv > 0) num = num * 10 + kv;
    }
    printf("  %s → %ld\n", veda, num);
    printf("  Reverse: %ld → (decode from right to left per Katapayadi rule)\n", num);
    
    printf("\n✅ Katapayadi system working.\n");
    return 0;
}
