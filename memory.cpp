#include "pch.h"
#include "memory.hpp"
#include "signatures.h"

Memory::Memory() {
    
    this->baseModule = (uintptr_t)GetModuleHandle(NULL);
    this->moduleSize = GetModuleSize();
}



// retourne la taille du module
uintptr_t Memory::GetModuleSize() {
    MODULEINFO modInfo = { 0 };
    if (this->baseModule) {
        // On remplit la struct complète
        GetModuleInformation(GetCurrentProcess(), (HMODULE)this->baseModule, &modInfo, sizeof(modInfo));
    }
    return (uintptr_t)modInfo.SizeOfImage;
}


// Signature Scanning
uintptr_t Memory::PatternScan(const unsigned char* pattern, const char* mask) {


    uintptr_t patternLength = strlen(mask);

    // boucle principale (i) qui parcourt la mémoire du process
    for (uintptr_t i = 0; i < (this->moduleSize - patternLength); i++)
    {
        bool find{ true };

        // Boucle secondaire (j) qui compare la signature
        for (uintptr_t j = 0; j < patternLength; j++)
        {

            // test logique avec le masque 
            if (mask[j] != '?' && ((unsigned char*)this->baseModule)[i + j] != pattern[j])
            {
                find = false;
                break;
            }
        }
        if (find)
        {
            return (uintptr_t)(this->baseModule + i);
        }
    }
    return 0;
}

bool Memory::Initialize() {
    // création de la structure de pattern / mask + relier à globales
    ScanTask tasks[] =
    {
         { "Local Player", Signatures::LocalPlayer, Signatures::maskLocalPlayer, 1, &this->localPlayerPtr},
         { "Entity Count", Signatures::EntityCount, Signatures::maskEntityCount, 3, &this->entityCountPtr},
         {"Entity List", Signatures::EntityList, Signatures::maskEntityList, 1, &this->entityListPtr}
    };

    uintptr_t instructionAdresse{ 0 };
    uintptr_t ptrTemp{ 0 };

    size_t taskSize = std::size(tasks);
    // boucle pour automatiser l'init des addresse mémoire dynamique

    for (size_t i = 0; i < taskSize; i++)
    {
        instructionAdresse = PatternScan(tasks[i].pattern, tasks[i].mask);

        if (instructionAdresse != 0)
        {
            ptrTemp = Read<uintptr_t>(instructionAdresse + tasks[i].offset);

            // check si c'est entity count dans ce cas simple déréférencement 
            if (strcmp(tasks[i].name, "Entity Count") == 0)
            {
                *tasks[i].resultAdress = (uintptr_t)ptrTemp;
            }
            else
            {
                *tasks[i].resultAdress = Read<uintptr_t>(ptrTemp);
            }
            
            std::cout << "[+] " << tasks[i].name << " initialise avec succes." << std::endl;
        }

        else
        {
            std::cout << "[!] " << tasks[i].name << " Pattern introuvable !" << std::endl;
            Sleep(1000);
            return false;
        }
    }

    return true;
}