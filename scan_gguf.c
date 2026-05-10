/*
 * GGUF Scanner - Zero allocation, reads directly from file
 * Extracts architecture info for Vedic inference config
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("google_gemma-4-31B-it-Q4_K_M.gguf", O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }
    
    uint32_t magic, version;
    uint64_t n_tensors, n_metadata;
    
    read(fd, &magic, 4);
    read(fd, &version, 4);
    read(fd, &n_tensors, 8);
    read(fd, &n_metadata, 8);
    
    printf("GGUF v%u | %llu tensors | %llu metadata\n\n", version, 
           (unsigned long long)n_tensors, (unsigned long long)n_metadata);
    
    char key[256];
    uint64_t key_len;
    uint32_t val_type;
    
    printf("=== Architecture Metadata ===\n");
    
    for (uint64_t i = 0; i < n_metadata; i++) {
        read(fd, &key_len, 8);
        if (key_len > 255) key_len = 255;
        read(fd, key, key_len);
        key[key_len] = 0;
        read(fd, &val_type, 4);
        
        // Only print architecture-related keys
        int print = (strstr(key, "block_count") || 
                    strstr(key, "architecture") ||
                    strstr(key, "context") ||
                    strstr(key, "embedding") ||
                    strstr(key, "head_count") ||
                    strstr(key, "feed_forward"));
        
        if (val_type == 4) {  // uint32
            uint32_t v;
            read(fd, &v, 4);
            if (print) printf("  %s: %u\n", key, v);
        } else if (val_type == 6) {  // uint64
            uint64_t v;
            read(fd, &v, 8);
            if (print) printf("  %s: %llu\n", key, (unsigned long long)v);
        } else if (val_type == 8) {  // string
            uint64_t vlen;
            read(fd, &vlen, 8);
            char val[1024];
            if (vlen > 1023) vlen = 1023;
            read(fd, val, vlen);
            val[vlen] = 0;
            if (print) printf("  %s: %s\n", key, val);
        } else if (val_type == 12) {  // float64
            double v;
            read(fd, &v, 8);
            if (print) printf("  %s: %f\n", key, v);
        } else {
            // Skip unknown types
            int sizes[] = {1,1,2,2,4,4,8,8,8,4,8,2,8};
            int sz = (val_type < 13) ? sizes[val_type] : 8;
            lseek(fd, sz, SEEK_CUR);
        }
    }
    
    // First 20 tensor names
    off_t tensor_start = lseek(fd, 0, SEEK_CUR);
    printf("\n=== First 20 Tensors ===\n");
    printf("Tensor data at offset: %lld\n", (long long)tensor_start);
    
    for (int i = 0; i < 20 && i < (int)n_tensors; i++) {
        read(fd, &key_len, 8);
        if (key_len > 255) key_len = 255;
        read(fd, key, key_len);
        key[key_len] = 0;
        
        uint32_t n_dims;
        read(fd, &n_dims, 4);
        
        uint64_t dims[8];
        for (uint32_t d = 0; d < n_dims && d < 8; d++) {
            read(fd, &dims[d], 8);
        }
        
        uint32_t ttype;
        uint64_t offset;
        read(fd, &ttype, 4);
        read(fd, &offset, 8);
        
        printf("  [%2d] %-45s dims=", i, key);
        for (uint32_t d = 0; d < n_dims; d++) {
            printf("%llu%s", (unsigned long long)dims[d], d < n_dims-1 ? "x" : "");
        }
        printf(" offset=%llu\n", (unsigned long long)offset);
    }
    
    close(fd);
    return 0;
}
