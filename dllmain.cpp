// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <psapi.h>
#include <Windows.h>
#include <iostream>

#include "memory.hpp"
#include "globals.h"
#include "signatures.h"
#include "hooking.hpp"

DWORD WINAPI MainThread(LPVOID lpReserved)
{
    // Création console de débugging
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "[+] DLL Injectee avec succes !" << std::endl;
    std::cout << "[+] Appuie sur la touche FIN (END) pour ejecter la dll." << std::endl;

    g_Mem = new Memory();

  
    if (g_Mem->GetBase() != 0)
    {
        if (g_Mem->Initialize())
        {
            std::cout << "[+] initialisation complete ! " << std::endl;

            g_Hook = new Hook();

            uintptr_t shotFuncAddr = g_Mem->GetBase() + 0x613B0;

            Hook::oShot = (Hook::tShot)g_Hook->Detour32((BYTE*)shotFuncAddr, (BYTE*)Hook::hooked_sub_4613B0, 6);

            std::cout << "[+] Hook de tir active !" << std::endl;
        }

        // Heartbeat
        while (!GetAsyncKeyState(VK_END))
        {

            Sleep(100);
        }
    }

    // Sortie
    g_Hook->unHook();
    std::cout << "[-] Fermeture du cheat..." << std::endl;
    Sleep(1000);

   
    if (f != nullptr)
    {
        fclose(f);
    }

    FreeConsole(); 
    delete g_Mem;
 
    FreeLibraryAndExitThread((HMODULE)lpReserved, 0);
    return 0;
}

// Le point d'entrée de la DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Désactive les appels de thread inutiles pour optimiser
        DisableThreadLibraryCalls(hModule);

        // Lance la fonction MainThread en arrière-plan
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        break;
    }
    return TRUE;
}

