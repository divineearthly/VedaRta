#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char** argv) {
    const char* model = argc > 1 ? argv[1] : "gemma-2b-q4.gguf";
    
    int fd = open(model, O_RDONLY);
    if (fd < 0) {
        printf("❌ Cannot open: %s\n", model);
        return 1;
    }
    
    uint32_t magic;
    read(fd, &magic, 4);
    printf("🕉️  GGUF Scanner\n");
    printf("   Model: %s\n", model);
    printf("   Magic: 0x%08X %s\n", magic, 
           magic == 0x46554747 ? "✅ Valid GGUF" : "❌ Invalid");
    
    off_t size = lseek(fd, 0, SEEK_END);
    printf("   Size: %.1f MB\n", size / (1024.0 * 1024.0));
    close(fd);
    return 0;
}
