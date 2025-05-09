#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// CRC32 table for faster computation
uint32_t crc32_table[256];

// Initialize the CRC32 table
void init_crc32_table() {
    uint32_t polynomial = 0xEDB88320;
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (size_t j = 0; j < 8; j++) {
            if (c & 1) {
                c = polynomial ^ (c >> 1);
            } else {
                c >>= 1;
            }
        }
        crc32_table[i] = c;
    }
}

// Calculate CRC32 for a single byte
uint32_t crc32_byte(uint8_t byte) {
    uint32_t crc = 0xFFFFFFFF;
    crc = crc32_table[(crc ^ byte) & 0xFF] ^ (crc >> 8);
    return ~crc;
}

// Encrypt shellcode using CRC32
void encrypt_shellcode(uint8_t* shellcode, size_t size, uint8_t* encrypted, uint32_t* key) {
    // Generate random key if not provided
    if (*key == 0) {
        srand(time(NULL));
        *key = rand();
    }
    
    uint32_t current_key = *key;
    
    for (size_t i = 0; i < size; i++) {
        // XOR byte with lowest byte of key
        uint8_t xored = shellcode[i] ^ (current_key & 0xFF);
        
        // Calculate CRC32 and take lowest byte
        encrypted[i] = crc32_byte(xored) & 0xFF;
        
        // Rotate key for next byte
        current_key = ((current_key << 1) | (current_key >> 31)) & 0xFFFFFFFF;
    }
}

// Decrypt shellcode
void decrypt_shellcode(uint8_t* encrypted, size_t size, uint8_t* decrypted, uint32_t key) {
    uint32_t current_key = key;
    
    for (size_t i = 0; i < size; i++) {
        uint8_t found = 0;
        
        // Try all possible bytes
        for (uint16_t test = 0; test < 256; test++) {
            uint8_t xored = test ^ (current_key & 0xFF);
            if ((crc32_byte(xored) & 0xFF) == encrypted[i]) {
                decrypted[i] = test;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            printf("Failed to decrypt byte at position %zu\n", i);
        }
        
        // Rotate key for next byte
        current_key = ((current_key << 1) | (current_key >> 31)) & 0xFFFFFFFF;
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

int main() {
    // Initialize CRC32 table
    init_crc32_table();
    
    // Example shellcode (using NOPs for demonstration)
    uint8_t shellcode[] = {0x90, 0x90, 0x90, 0x90, 0x90};
    size_t shellcode_size = sizeof(shellcode);
    
    // Buffers for encrypted and decrypted data
    uint8_t encrypted[shellcode_size];
    uint8_t decrypted[shellcode_size];
    
    // Encryption key
    uint32_t key = 0;  // Will be randomly generated
    
    // Encrypt shellcode
    encrypt_shellcode(shellcode, shellcode_size, encrypted, &key);
    
    // Print results
    print_bytes("Original shellcode", shellcode, shellcode_size);
    print_bytes("Encrypted shellcode", encrypted, shellcode_size);
    printf("Encryption key: 0x%08x\n", key);
    
    // Decrypt shellcode
    decrypt_shellcode(encrypted, shellcode_size, decrypted, key);
    print_bytes("Decrypted shellcode", decrypted, shellcode_size);
    
    // Verify decryption
    if (memcmp(shellcode, decrypted, shellcode_size) == 0) {
        printf("Decryption successful - output matches original shellcode\n");
    } else {
        printf("Decryption failed!\n");
    }
    
    return 0;
}
