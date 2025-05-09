#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Encrypted flag parts (LUHN_GOT_EPBAOCMQ)
const unsigned char encryptedFirstPart[] = { 
    0x9B, 0xFF, 0x26, 0x93, 0xD7, 0xB7, 0x97, 0xFB, 0xD7 // LU#N_GOT_
};

const unsigned char encryptedSecondPart[] = {
    0xBF, 0xEB, 0xA3, 0xAF, 0x97, 0xA7, 0x9F, 0xEF // EPBAOCMQ
};

void decryptFlagPart(const unsigned char *encrypted, int len, unsigned char *output) {
    for (int i = 0; i < len; i++) {
        unsigned char c = encrypted[i];
        c ^= 0xAA;          // XOR first with key
        c = (c >> 2) | (c << 6);  // Rotate right by 2 bits
        output[i] = c;
    }
    output[len] = '\0';
}

int luhnCheck(const char *number) {
    int len = strlen(number);
    int sum = 0;
    int isSecond = 0;

    for (int i = len - 1; i >= 0; i--) {
        int digit = number[i] - '0';
        if (isSecond) {
            digit *= 2;
            digit = (digit / 10) + (digit % 10);
        }
        sum += digit;
        isSecond = !isSecond;
    }
    return (sum % 10) == 0;
}

int checkSecondPart(const char *number, unsigned char *decrypted) {
    unsigned char transformed[9];
    for (int i = 0; i < 8; i++) {
        int pair = (number[i*2] - '0') * 10 + (number[i*2+1] - '0');
        unsigned char c = (unsigned char)pair;
        c ^= 0xAA;
        c = (c << 2) | (c >> 6); // Rotate left by 2
        transformed[i] = c;
    }
    
    if (memcmp(transformed, encryptedSecondPart, 8) == 0) {
        decryptFlagPart(encryptedSecondPart, 8, decrypted);
        return 1;
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            CreateWindowA("EDIT", "", WS_VISIBLE|WS_CHILD|WS_BORDER|ES_AUTOHSCROLL,
                          10, 10, 250, 25, hwnd, (HMENU)1, NULL, NULL);
            CreateWindowA("BUTTON", "CRACK IT", WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
                          270, 10, 80, 25, hwnd, (HMENU)2, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == 2) {
                char input[17];
                GetWindowTextA(GetDlgItem(hwnd, 1), input, 17);

                if (strlen(input) != 16) {
                    MessageBoxA(hwnd, "Sweet 16 darling", "Error", MB_OK);
                } 
                else if (luhnCheck(input)) {
                    unsigned char part1[20], part2[10];
                    char flag[50] = {0};
                    
                    // Decrypt and build flag
                    decryptFlagPart(encryptedFirstPart, 9, part1);
                    if (checkSecondPart(input, part2)) {
                        snprintf(flag, sizeof(flag), "bbctf{%s%s}\n... hey that is mine!!!", part1, part2);
                    } else {
                        snprintf(flag, sizeof(flag), "bbctf{%s\n... but is it mine thou ???", part1);
                    }
                    
                    MessageBoxA(hwnd, flag, "Flag Found!", MB_OK);
                } else {
                    MessageBoxA(hwnd, "Aint no Luhn's number that!", "Error", MB_OK);
                }
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "FlagWindowClass";
    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(0, "FlagWindowClass", "Luhn's Vault", 
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                               400, 100, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
