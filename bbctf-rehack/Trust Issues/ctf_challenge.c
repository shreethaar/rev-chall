#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdint.h>

#define MAX_LENGTH 100  // Increased length to accommodate for longer flag

int buffer[15 * 1024 *1024] = {1337};

// Function declarations
void banner(void);
void anti_debug(void);
void proc_input(void);
void yet_another_main(void);
void maybe_is_this_main(void);
void check_flag(const char *input);
void hex_to_string(const char *hex, char *output);
void decode_base64(const char* encoded, char* decoded);

// Obfuscated hex string (even indices hold the flag)
const char *hexString = "4ea3305c749f5fd241b7314a31885ff633e46e7a74c9723b79915f55530d74a63462722e743973be5f11465a728930ce6d6f5f374d3e3471311c4e";

// Custom entry point that properly initializes and calls the main function
void _start(void) {
    asm("xorl %ebp, %ebp");  // Clear the base pointer
    //anti_debug();  // Prevent debugging
    //banner();
    //proc_input();
    yet_another_main();
    exit(0);
}


// custom "main()" 
void yet_another_main(void) {
    anti_debug();  // Prevent debugging
    //printf("[+] Calling from yet_another_main\n");
    banner();
    proc_input();
    exit(0);
}


void decode_base64(const char* encoded, char* decoded) {
    // Custom base64 alphabet
    const char *b64 = "ZYXABCDEFGHIJKLMNOPQRSTUVWzyxabcdefghijklmnopqrstuvw0123456789+/";
    
    // Build lookup table
    int lookup[256];
    memset(lookup, -1, sizeof(lookup));
    for(int i = 0; i < 64; i++) {
        lookup[(unsigned char)b64[i]] = i;
    }
    
    size_t in_len = strlen(encoded);
    size_t j = 0;
    
    // Process input in blocks of 4 characters
    for (size_t i = 0; i < in_len;) {
        int val[4] = {0};
        int padding = 0;
        
        // Get values for a 4-character chunk
        for (int k = 0; k < 4; k++, i++) {
            if (i >= in_len) {
                padding++;
                continue;
            }
            
            unsigned char c = encoded[i];
            if (c == '=') {
                padding++;
                continue;
            }
            
            val[k] = lookup[c];
            if (val[k] == -1) {
                //printf("Invalid character in input: %c\n", c);
                return;
            }
        }
        
        // Combine 4x6 bits into 24 bits
        uint32_t triple = (val[0] << 18) | (val[1] << 12) | (val[2] << 6) | val[3];
        
        // Extract 3 bytes, accounting for padding
        for (int k = 0; k < 3 - padding; k++) {
            decoded[j++] = (triple >> (16 - (k * 8))) & 0xFF;
        }
    }
    
    decoded[j] = '\0';  // Null-terminate the result
}

void another_proc_input(void) {
    char input[MAX_LENGTH];
    printf("Enter the flag: ");
    fgets(input, MAX_LENGTH, stdin);
    input[strcspn(input, "\n")] = 0;  // Remove newline
    
    const char* test_encoded = "VjGgaDW7OhCIOS9DQBCEU1KPTSiWcN==";
    size_t decoded_len = strlen(test_encoded) * 3 / 4 + 1;
    char* decoded = (char*)malloc(decoded_len);
    if (!decoded) {
        //printf("Memory allocation failed\n");
        exit(1);
    }
    
    // Decode the test string
    decode_base64(test_encoded, decoded);
    
    // Print the decoded result
    //printf("Encoded: %s\n", test_encoded);
    //printf("Decoded hex: ");
    //for (size_t i = 0; i < strlen(decoded); i++) {
    //    printf("%02X ", (unsigned char)decoded[i]);
    //}
    //printf("\n");
    //printf("Decoded text: %s\n", decoded);
    
    if(strcmp(input, decoded) != 0) {
        printf("Wrong flag!\n");
    }
    else {
        printf("Correct flag!\n");
    }
    
    free(decoded);
    exit(0);
}


void maybe_is_this_main(void) {
    anti_debug();
    banner();
    another_proc_input();
    exit(0);
}

// Fake main() to mislead reverse engineers
int main(int argc, char **argv) {
    //printf("[+] Calling from main\n");
    banner();
    //proc_input();
    printf("Incorrect flag. Try again.\n");
    exit(0);
}

// Anti-debugging function using ptrace
void anti_debug(void) {
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
        //printf("No debugging allowed!\n");
        exit(1);
    }
}

// Process user input
void proc_input(void) {
    char input[MAX_LENGTH];
    //printf("Enter the flag > ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;  // Remove newline
    check_flag(input);
}

// Convert the even-positioned hex characters into a string
// Fixed to properly extract even-positioned bytes
void hex_to_string(const char *hex, char *output) {
    size_t len = strlen(hex);
    size_t j = 0;
    
    // Ensure we don't overflow output buffer
    size_t max_output_len = MAX_LENGTH - 1;
    
    for (size_t i = 0; i < len; i += 2) {
        // Process only even-positioned bytes (0, 4, 8...)
        if (i % 4 == 0) {
            // Ensure we have enough characters left to form a byte
            if (i + 1 < len && j < max_output_len) {
                char hexByte[3] = {hex[i], hex[i + 1], '\0'};
                output[j++] = (char)strtol(hexByte, NULL, 16);
            }
        }
    }
    output[j] = '\0';  // Null terminate
}

void check_flag(const char *input) {
    char decodedFlag[MAX_LENGTH];  
    hex_to_string(hexString, decodedFlag);
    
    // Remove debug print in production
    //printf("DEBUG: Extracted flag -> %s\n", decodedFlag);
    
    if (strcmp(input, decodedFlag) == 0) {
        printf("Correct flag! Well done!\n");
        printf("bbctf{%s}\n",input);
    } else {
        printf("Incorrect flag. Try again.\n");
        //printf("bbctf{%s}\n",input);
    }
}

// Banner function to display ASCII art
void banner() {
    const char *hex = "e2948ce294ace29490e294ace29480e29490e294ac20e294ace2948ce29480e29490e2948ce294ace294902020e294ace2948ce29480e29490e2948ce29480e29490e294ac20e294ace2948ce29480e29490e2948ce29480e294900a20e2948220e2949ce294ace29498e2948220e29482e29494e29480e2949020e29482202020e29482e29494e29480e29490e29494e29480e29490e2948220e29482e2949ce294a420e29494e29480e294900a20e294b420e294b4e29494e29480e29494e29480e29498e29494e29480e2949820e294b4202020e294b4e29494e29480e29498e29494e29480e29498e29494e29480e29498e29494e29480e29498e29494e29480e29498";
    
    size_t len = strlen(hex);
    if (len % 2 != 0) {
        printf("Invalid hex string.\n");
        return;
    }
    
    size_t byte_len = len / 2;
    unsigned char *utf8_str = malloc(byte_len + 1);
    if (!utf8_str) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    for (size_t i = 0; i < byte_len; i++) {
        sscanf(hex + (i * 2), "%2hhx", &utf8_str[i]);
    }
    utf8_str[byte_len] = '\0';
    
    printf("\n%s\n\n", utf8_str);
    free(utf8_str);
    printf("\n");
    printf("     (¬_¬)           (ಥ﹏ಥ)  \n");
    printf("      /│\\             /│\\  \n");
    printf("      / \\             / \\  \n");
    printf("  \"Why don't       \"I checked,    \n");
    printf("   you trust me?\"   main always return false!\"  \n\n\n");
    printf("Proof them wrong!\n");
    printf("Enter flag > ");
}
