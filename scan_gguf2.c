/*
 * GGUF Scanner v2 - Uses syscall directly, proper error handling
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

// Safe read that retries on partial reads
ssize_t full_read(int fd, void *buf, size_t count) {
    size_t total = 0;
    while (total < count) {
        ssize_t n = read(fd, (char*)buf + total, count - total);
        if (n <= 0) return n;
        total += n;
    }
    return total;
}

int main() {
    int fd = open("google_gemma-4-31B-it-Q4_K_M.gguf", O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }
    
    // Read and verify magic
    char magic_buf[5] = {0};
    if (full_read(fd, magic_buf, 4) != 4) {
        fprintf(stderr, "Failed to read magic\n");
        return 1;
    }
    printf("Magic: %.4s\n", magic_buf);
    
    uint32_t version;
    uint64_t n_tensors, n_metadata;
    
    full_read(fd, &version, 4);
    full_read(fd, &n_tensors, 8);
    full_read(fd, &n_metadata, 8);
    
    printf("GGUF v%u\n", version);
    printf("Tensors: %llu\n", (unsigned long long)n_tensors);
    printf("Metadata: %llu\n\n", (unsigned long long)n_metadata);
    
    printf("=== Architecture ===\n");
    
    char key[256];
    uint64_t key_len;
    uint32_t val_type;
    
    for (uint64_t i = 0; i < n_metadata && i < 100; i++) {
        full_read(fd, &key_len, 8);
        if (key_len > 250) {
            fprintf(stderr, "Key too long: %llu at meta %llu\n", 
                    (unsigned long long)key_len, (unsigned long long)i);
            break;
        }
        full_read(fd, key, key_len);
        key[key_len] = 0;
        full_read(fd, &val_type, 4);
        
        // Only print relevant keys
        int show = (strstr(key, "block_count") || 
                   strstr(key, "architecture") ||
                   strstr(key, "context") ||
                   strstr(key, "embedding") ||
                   strstr(key, "head_count") ||
                   strstr(key, "feed_forward"));
        
        if (val_type == 4) {
            uint32_t v;
            full_read(fd, &v, 4);
            if (show) printf("  %s: %u\n", key, v);
        } else if (val_type == 6) {
            uint64_t v;
            full_read(fd, &v, 8);
            if (show) printf("  %s: %llu\n", key, (unsigned long long)v);
        } else if (val_type == 8) {
            uint64_t vlen;
            full_read(fd, &vlen, 8);
            char val[512] = {0};
            if (vlen > 500) vlen = 500;
            full_read(fd, val, vlen);
            if (show) printf("  %s: %s\n", key, val);
        } else if (val_type == 12) {
            double v;
            full_read(fd, &v, 8);
            if (show) printf("  %s: %f\n", key, v);
        } else {
            int sizes[] = {1,1,2,2,4,4,8,8,8,4,8,2,8};
            int sz = (val_type < 13) ? sizes[val_type] : 8;
            lseek(fd, sz, SEEK_CUR);
        }
    }
    
    close(fd);
    return 0;
}
