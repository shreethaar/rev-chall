#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// RC4 context structure
typedef struct {
    uint8_t S[256];
    int i;
    int j;
} RC4_CTX;

// Initialize RC4 state with a key
void rc4_init(RC4_CTX *ctx, uint8_t *key, size_t key_len) {
    int i, j = 0;
    uint8_t temp;
    
    // Initialize S-box
    for (i = 0; i < 256; i++) {
        ctx->S[i] = i;
    }
    
    // Key scheduling algorithm (KSA)
    for (i = 0; i < 256; i++) {
        j = (j + ctx->S[i] + key[i % key_len]) & 0xFF;
        // Swap
        temp = ctx->S[i];
        ctx->S[i] = ctx->S[j];
        ctx->S[j] = temp;
    }
    
    ctx->i = 0;
    ctx->j = 0;
}

// Generate RC4 keystream and XOR with input
void rc4_crypt(RC4_CTX *ctx, uint8_t *data, size_t data_len) {
    uint8_t temp;
    int i = ctx->i;
    int j = ctx->j;
    
    for (size_t k = 0; k < data_len; k++) {
        // Update i and j
        i = (i + 1) & 0xFF;
        j = (j + ctx->S[i]) & 0xFF;
        
        // Swap values
        temp = ctx->S[i];
        ctx->S[i] = ctx->S[j];
        ctx->S[j] = temp;
        
        // Generate keystream byte and XOR with data
        uint8_t keystream = ctx->S[(ctx->S[i] + ctx->S[j]) & 0xFF];
        data[k] ^= keystream;
    }
    
    ctx->i = i;
    ctx->j = j;
}

// Generate random key
void generate_key(uint8_t *key, size_t key_len) {
    srand(time(NULL));
    for (size_t i = 0; i < key_len; i++) {
        key[i] = rand() & 0xFF;
    }
}

// Helper function to print bytes in hex
void print_bytes(const char* label, uint8_t* data, size_t size) {
    printf("%s: ", label);
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

// Helper function to print key in hex
void print_key(const char* label, uint8_t* key, size_t key_len) {
    printf("%s: ", label);
    for (size_t i = 0; i < key_len; i++) {
        printf("%02x", key[i]);
    }
    printf("\n");
}

int main() {
    // Example shellcode (using NOPs for demonstration)
    uint8_t shellcode[] = {0x90, 0x90, 0x90, 0x90, 0x90};
    size_t shellcode_size = sizeof(shellcode);
    
    // Create buffers for encrypted and decrypted data
    uint8_t encrypted[shellcode_size];
    uint8_t decrypted[shellcode_size];
    
    // Copy original shellcode to encrypted buffer (will be encrypted in place)
    memcpy(encrypted, shellcode, shellcode_size);
    
    // Generate random key (16 bytes for this example)
    const size_t key_len = 16;
    uint8_t key[key_len];
    generate_key(key, key_len);
    
    // Print original data and key
    print_bytes("Original shellcode", shellcode, shellcode_size);
    print_key("Encryption key", key, key_len);
    
    // Encrypt
    RC4_CTX encrypt_ctx;
    rc4_init(&encrypt_ctx, key, key_len);
    rc4_crypt(&encrypt_ctx, encrypted, shellcode_size);
    print_bytes("Encrypted shellcode", encrypted, shellcode_size);
    
    // Copy encrypted data to decrypted buffer
    memcpy(decrypted, encrypted, shellcode_size);
    
    // Decrypt
    RC4_CTX decrypt_ctx;
    rc4_init(&decrypt_ctx, key, key_len);
    rc4_crypt(&decrypt_ctx, decrypted, shellcode_size);
    print_bytes("Decrypted shellcode", decrypted, shellcode_size);
    
    // Verify decryption
    if (memcmp(shellcode, decrypted, shellcode_size) == 0) {
        printf("Decryption successful - output matches original shellcode\n");
    } else {
        printf("Decryption failed!\n");
    }
    
    return 0;
}
