#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define MAX_LEN 30

void checker(const char* flag);
bool checkFlag1(const char* flag1);
bool checkFlag2(const char* flag2);
bool checkFlag3(const char* flag3);
char* rotate(const char* src);
char* base64_encode(const unsigned char* data, size_t input_length);

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("./chall.exe <flag>\n");
        return EXIT_FAILURE;
    }
    char* flag = argv[1];
    checker(flag);  
    
    return EXIT_SUCCESS;
}

// ANTI-UPX-UNP4CK3R_1337
// flag1: ANTI-UPX-    --> reverse then rot13
// flag2: UNP4CK3R_    --> memcpy XOR value and key and ROL 3
// flag3: 1337         --> get current year and minus 688
void checker(const char* flag) {
    //printf("%s\n",flag);
    if(strlen(flag) < 22 || strlen(flag) > 22) {
        printf("Incorrect Flag\n");
        return;
    }
    char flag1[10] = {0}; 
    char flag2[10] = {0}; 
    char flag3[5] = {0};
    
    strncpy(flag1, flag, 9);
    flag1[9] = '\0';
    
    strncpy(flag2, flag + 9, 9); 
    flag2[9] = '\0';
    
    strncpy(flag3, flag + 18, 4); 
    flag3[4] = '\0';
    
    //printf("Part 1: %s\n", flag1);
    //printf("Part 2: %s\n", flag2);
    //printf("Part 3: %s\n", flag3);
    bool valid = checkFlag1(flag1) && checkFlag2(flag2) && checkFlag3(flag3);
    
    if (valid) {
        printf("Congrats! Correct Flag!\n");
        printf("FLAG{%s}\n",flag);
    } else {
        printf("Incorrect Flag\n");
    }
}

// Implement flag checking logic based on comments:
// "flag1: ANTI-UPX-    --> reverse then rot13"
bool checkFlag1(const char* flag1) {
    if(strlen(flag1) != 9) {
        return false;
    }
    char reversed[10];
    for(int i = 0; i < 9; i++) {
        reversed[i] = flag1[8-i];
    }
    reversed[9] = '\0';
    char* rot13_result = rotate(reversed);
    //printf("Rot13 result: %s\n", rot13_result);
    if (rot13_result == NULL) {
        return false;
    }
    bool result = (strncmp(rot13_result, "-KCH-VGAN", 9) == 0);
    free(rot13_result);
    return result;
}

// Base64 encoding table
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Encode a string to base64
char* base64_encode(const unsigned char* data, size_t input_length) {
    size_t output_length = 4 * ((input_length + 2) / 3);
    char* encoded_data = malloc(output_length + 1);
    if (encoded_data == NULL) return NULL;
    
    size_t i, j;
    for (i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;
        
        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;
        
        encoded_data[j++] = base64_table[(triple >> 18) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 12) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 6) & 0x3F];
        encoded_data[j++] = base64_table[triple & 0x3F];
    }
    
    // Add padding if necessary
    if (input_length % 3 == 1) {
        encoded_data[output_length - 1] = '=';
        encoded_data[output_length - 2] = '=';
    } else if (input_length % 3 == 2) {
        encoded_data[output_length - 1] = '=';
    }
    
    encoded_data[output_length] = '\0';
    return encoded_data;
}

// "flag2: UNP4CK3R_    --> memcpy XOR value and key and ROL 3"
bool checkFlag2(const char* flag2) {
    if (strlen(flag2) != 9) {
        return false;
    }
    
    // Base64 encode the input flag
    char* encoded = base64_encode((const unsigned char*)flag2, 9);
    if (encoded == NULL) {
        return false;
    }
    
    //printf("Encoded flag2: %s\n", encoded);
    
    // Compare with expected base64 value - "UNP4CK3R_" encoded in base64
    bool result = (strncmp(encoded, "VU5QNENLM1Jf", 12) == 0);
    
    free(encoded);
    return result;
}

// "flag3: 1337         --> get current year and minus 688"
bool checkFlag3(const char* flag3) {
    // Get current year
    time_t t = time(NULL);
    struct tm* timeinfo = localtime(&t);
    int current_year = timeinfo->tm_year + 1900;  // tm_year is years since 1900
    
    // Calculate the expected value (current year - 688)
    int expected_value = current_year - 688;
    
    // Convert the expected value to string
    char expected_str[5];
    sprintf(expected_str, "%d", expected_value);
    //printf("Expected flag3: %s\n", expected_str);
    
    // Compare with the input flag
    return strncmp(flag3, expected_str, 4) == 0;
}

char* rotate(const char *src) {
    if(src == NULL){
        return NULL;
    }
  
    char* result = malloc(strlen(src) + 1);
    
    if(result != NULL){
        strcpy(result, src);
        char* current_char = result;
        
        while(*current_char != '\0'){
            //Only increment alphabet characters
            if((*current_char >= 97 && *current_char <= 122) || (*current_char >= 65 && *current_char <= 90)){
                if(*current_char > 109 || (*current_char > 77 && *current_char < 91)){
                    //Characters that wrap around to the start of the alphabet
                    *current_char -= 13;
                }else{
                    //Characters that can be safely incremented
                    *current_char += 13;
                }
            }
            current_char++;
        }
    }
    return result;
}
