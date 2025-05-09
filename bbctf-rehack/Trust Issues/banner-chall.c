#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void hex_to_utf8(const char *hex) {
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
    printf("%s\n", utf8_str);
    free(utf8_str);
}


int main() {
    printf("\n");
    const char *banner = "e2948ce294ace29490e294ace29480e29490e294ac20e294ace2948ce29480e29490e2948ce294ace294902020e294ace2948ce29480e29490e2948ce29480e29490e294ac20e294ace2948ce29480e29490e2948ce29480e294900a20e2948220e2949ce294ace29498e2948220e29482e29494e29480e2949020e29482202020e29482e29494e29480e29490e29494e29480e29490e2948220e29482e2949ce294a420e29494e29480e294900a20e294b420e294b4e29494e29480e29494e29480e29498e29494e29480e2949820e294b4202020e294b4e29494e29480e29498e29494e29480e29498e29494e29480e29498e29494e29480e29498e29494e29480e29498";  
    hex_to_utf8(banner);
    printf("\n");
    printf("     (¬_¬)           (ಥ﹏ಥ)  \n");
    printf("      /│\\             /│\\  \n");
    printf("      / \\             / \\  \n");
    printf("  \"Why don’t       \"I checked,    \n");
    printf("   you trust me?\"   main always return false!\"  \n");
    printf("\n\n");
    printf("Proof them wrong!\nEnter flag >\n");
}
