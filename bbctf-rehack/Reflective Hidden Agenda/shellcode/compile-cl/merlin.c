#include <windows.h>
#include <stdio.h>
#include "resource.h"
// RC4 Decryption Function 
//void SpotEnumerateFiles 


// Export VoidFunc for reflective injection
__declspec(dllexport) void VoidFunc() {
    MessageBoxA(
        NULL, 
        "Click okay to delete file", 
        "Flag is not here, but somewhere else", 
        MB_OK
    );
}

// Export sHoWfLaG for hint message 
__declspec(dllexport) void sHoWfLaG() {
    MessageBoxA(
            NULL,
            "Patience... i am trying to EXTRACT the flag",
            "Bazinga FLAG!!!",
            MB_OK
        );
    // Extract the encrypted shellcode from the resource section
    /*HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_SHELLCODE), RT_RCDATA);
    HGLOBAL hLoaded = LoadResource(NULL, hResource);
    LPVOID pEncryptedShellcode = LockResource(hLoaded);
    DWORD dwSize = SizeofResource(NULL, hResource);
    */
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_SHELLCODE), RT_RCDATA);
    if (hResource == NULL) {
        MessageBoxA(NULL, "FindResource failed!", "Error", MB_ICONERROR);
        return;
    }

    // Step 2: Load the resource
    HGLOBAL hLoaded = LoadResource(NULL, hResource);
    if (hLoaded == NULL) {
        MessageBoxA(NULL, "LoadResource failed!", "Error", MB_ICONERROR);
        return;
    }

    // Step 3: Lock the resource
    LPVOID pEncryptedShellcode = LockResource(hLoaded);
    if (pEncryptedShellcode == NULL) {
        MessageBoxA(NULL, "LockResource failed!", "Error", MB_ICONERROR);
        return;
    }

    // Step 4: Get the size
    DWORD dwSize = SizeofResource(NULL, hResource);
    if (dwSize == 0) {
        MessageBoxA(NULL, "SizeofResource failed!", "Error", MB_ICONERROR);
        return;
    }

}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  
    DWORD fdwReason,     
    LPVOID lpReserved )  
{
    switch( fdwReason )
    {
         case DLL_PROCESS_ATTACH:
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
