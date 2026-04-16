#include "pch.h"
#include "hooking.hpp"

Hook::tShot Hook::oShot = nullptr;

BYTE* Hook::Detour32(BYTE* src, BYTE* dst, const uintptr_t len) {
    if (len < 5) return nullptr;

    // allocation de l'espace mémoire du trampoline (gateway)
    BYTE* gateway = (BYTE*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (gateway == nullptr) return nullptr;

    // copie des octets écraser par le hook 
    memcpy(gateway, src, len);

    // calcule et écriture du jmp pour revenir à la fonction originale 
    uintptr_t gateRelAddr = (uintptr_t)(src - gateway - 5);
    *(gateway + len) = 0xE9;
    *(uintptr_t*)(gateway + len + 1) = gateRelAddr;


    DWORD oldProtect{ 0 };
    //changement des permissions
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);

    // calcule de l'offset entre le hook et l'instruction suivante (le silent aim)
    uintptr_t relativeAdress = (uintptr_t)(dst - src - 5);

    // mise en place du hook 
    *src = 0xE9; // on écris l'oppcode du jmp 
    *(uintptr_t*)(src + 1) = relativeAdress; // on écrit l'offset du jmp 
    // restitution des permissions de base 
    VirtualProtect(src, len, oldProtect, &oldProtect);

    return gateway;
}


void __cdecl Hook::hooked_sub_4613B0(Vector3* origin, Vector3* target, PlayerStruct* owner)
{
    if ((uintptr_t)owner == g_Mem->GetLocalPlayer()) {
    
        PlayerStruct* bestTarget = Features::GetBestTarget(origin, 10.0f);

        if (bestTarget != nullptr) {
            *target = bestTarget->pos;
        }
    }


    // Appeler le Gateway quoi qu'il arrive
    Hook::oShot(origin, target, owner);
}


void Hook::unHook()
{
    // Sécurité
    if (targetAddr == nullptr) return;

    DWORD oldProtect;
   
    VirtualProtect(targetAddr, hookLen, PAGE_EXECUTE_READWRITE, &oldProtect);

    // copie des octets originaux par-dessus le JMP
    memcpy(targetAddr, originalBytes, hookLen);

    VirtualProtect(targetAddr, hookLen, oldProtect, &oldProtect);

    std::cout << "[+] Hook retire, memoire restauree." << std::endl;
}