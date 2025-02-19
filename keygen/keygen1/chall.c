#include <stdio.h>
#include <string.h>

int validate_key(char *key) {
    if (strlen(key) != 16) return 0;
    int checksum = 0;
    for (int i = 0; i < 16; i++) {
        checksum += key[i];
    }
    if (checksum % 17 != 0) return 0;
    if (key[0] != 'A') return 0;
    return 1;
}

int main() {
    char key[17];
    printf("Enter key: ");
    scanf("%16s", key);
    if (validate_key(key)) {
        printf("Valid key!\n");
    } else {
        printf("Invalid key!\n");
    }
    return 0;
}
