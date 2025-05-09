#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to generate a random 256-bit (32-byte) key
void generate_key(unsigned char *key, size_t key_len) {
    srand((unsigned int)time(NULL)); // Seed the random number generator
    for (size_t i = 0; i < key_len; i++) {
        key[i] = (unsigned char)(rand() % 256); // Generate random bytes
    }
}

// RC4 Key Scheduling Algorithm (KSA)
void rc4_ksa(const unsigned char *key, size_t key_len, unsigned char *s) {
    for (int i = 0; i < 256; i++) {
        s[i] = (unsigned char)i;
    }

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + s[i] + key[i % key_len]) % 256;
        // Swap s[i] and s[j]
        unsigned char temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}

// RC4 Pseudo-Random Generation Algorithm (PRGA)
unsigned char rc4_prga(unsigned char *s, int *i, int *j) {
    *i = (*i + 1) % 256;
    *j = (*j + s[*i]) % 256;

    // Swap s[*i] and s[*j]
    unsigned char temp = s[*i];
    s[*i] = s[*j];
    s[*j] = temp;

    return s[(s[*i] + s[*j]) % 256];
}

// Function to perform RC4 encryption
void rc4_encrypt(const unsigned char *plaintext, size_t len, const unsigned char *key, size_t key_len, unsigned char *ciphertext) {
    unsigned char s[256]; // State array for RC4
    int i = 0, j = 0;

    // Key Scheduling Algorithm (KSA)
    rc4_ksa(key, key_len, s);

    // Encrypt the plaintext using the PRGA
    for (size_t k = 0; k < len; k++) {
        unsigned char keystream_byte = rc4_prga(s, &i, &j);
        ciphertext[k] = plaintext[k] ^ keystream_byte; // XOR plaintext with keystream
    }
}

int main() {
    const char *input_file = "shellcode.bin";
    const char *output_file = "encrypted.bin";
    const size_t key_len = 32; // 256-bit key (32 bytes)

    // Generate a random 256-bit key
    unsigned char key[key_len];
    generate_key(key, key_len);

    // Print the generated key
    printf("Generated Key (256-bit):\n");
    for (size_t i = 0; i < key_len; i++) {
        printf("%02x", key[i]);
    }
    printf("\n\n");

    // Open the input file
    FILE *file = fopen(input_file, "rb");
    if (!file) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the file into a buffer
    unsigned char *plaintext = (unsigned char *)malloc(file_size);
    if (!plaintext) {
        perror("Error allocating memory for plaintext");
        fclose(file);
        return EXIT_FAILURE;
    }
    fread(plaintext, 1, file_size, file);
    fclose(file);

    // Allocate memory for the ciphertext
    unsigned char *ciphertext = (unsigned char *)malloc(file_size);
    if (!ciphertext) {
        perror("Error allocating memory for ciphertext");
        free(plaintext);
        return EXIT_FAILURE;
    }

    // Perform RC4 encryption
    rc4_encrypt(plaintext, file_size, key, key_len, ciphertext);

    // Write the ciphertext to the output file
    FILE *out_file = fopen(output_file, "wb");
    if (!out_file) {
        perror("Error opening output file");
        free(plaintext);
        free(ciphertext);
        return EXIT_FAILURE;
    }
    fwrite(ciphertext, 1, file_size, out_file);
    fclose(out_file);

    // Print the ciphertext in hexadecimal format
    printf("Encrypted Output (Hex):\n");
    for (long i = 0; i < file_size; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    // Clean up
    free(plaintext);
    free(ciphertext);

    return EXIT_SUCCESS;
}
