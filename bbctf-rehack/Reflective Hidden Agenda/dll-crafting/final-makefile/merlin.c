#include <windows.h>
#include <stdio.h>
#include "resource.h"

static HINSTANCE g_hinstDLL=NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

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

// RC4 Decryption Function
void rc4_decrypt(const unsigned char *pEncryptedShellcode, DWORD dwSize, const unsigned char *key, size_t key_len, unsigned char *pDecryptedShellcode) {
    unsigned char s[256]; // State array for RC4
    int i = 0, j = 0;

    // Key Scheduling Algorithm (KSA)
    rc4_ksa(key, key_len, s);

    // Decrypt the ciphertext using the PRGA
    for (DWORD k = 0; k < dwSize; k++) {
        unsigned char keystream_byte = rc4_prga(s, &i, &j);
        pDecryptedShellcode[k] = pEncryptedShellcode[k] ^ keystream_byte; // XOR ciphertext with keystream
    }
}

// Export VoidFunc for reflective injection
__declspec(dllexport) void Run() {
    MessageBoxA(
        NULL, 
        "Click okay to delete file", 
        "Flag is not here, but somewhere else", 
        MB_OK
    );
}

// Export sHoWfLaG for hint message 
__declspec(dllexport) void VoidFunc() {
    MessageBoxA(
            NULL,
            "Patience... i am trying to EXTRACT the flag",
            "Bazinga FLAG!!!",
            MB_OK
        );
    // Extract the encrypted shellcode from the resource section
    HRSRC hResource = FindResource(g_hinstDLL, MAKEINTRESOURCE(IDR_SHELLCODE), RT_RCDATA);
    HGLOBAL hLoaded = LoadResource(g_hinstDLL, hResource);
    LPVOID pEncryptedShellcode = LockResource(hLoaded);
    DWORD dwSize = SizeofResource(g_hinstDLL, hResource);


    
    const unsigned char key[] = {
        0x47, 0xe1, 0x9b, 0xca, 0xd9, 0x87, 0x21, 0x40,
        0xa5, 0x9d, 0x19, 0x14, 0x6c, 0x24, 0xc7, 0x6d,
        0x8a, 0x7e, 0x61, 0x83, 0x05, 0xa2, 0x9e, 0x4d,
        0x0f, 0x7f, 0x7a, 0x66, 0xc2, 0x43, 0x30, 0x09
    };

    const size_t key_len = 32; // 256-bit key (32 bytes)
    unsigned char *pDecryptedShellcode = (unsigned char *)malloc(dwSize);
     
    rc4_decrypt(pEncryptedShellcode, dwSize, key, key_len, pDecryptedShellcode);
    RtlSecureZeroMemory(pDecryptedShellcode, dwSize);
    free(pDecryptedShellcode);


}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  
    DWORD fdwReason,     
    LPVOID lpReserved )  
{
    switch( fdwReason )
    {
         case DLL_PROCESS_ATTACH:
            g_hinstDLL=hinstDLL;
             {
                STARTUPINFO si;
                PROCESS_INFORMATION pi;
                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);
                ZeroMemory(&pi, sizeof(pi));

                // CreateProcess requires all 10 parameters
                if(CreateProcessA(
                    "C:\\Windows\\System32\\notepad.exe",  // Application name
                    NULL,                                  // Command line args
                    NULL,                                  // Process security attributes
                    NULL,                                  // Thread security attributes
                    FALSE,                                 // Inherit handles
                    0,                                     // Creation flags
                    NULL,                                  // Environment
                    NULL,                                  // Current directory
                    &si,                                   // Startup info
                    &pi                                    // Process info
                )) {
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                }
            }
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
