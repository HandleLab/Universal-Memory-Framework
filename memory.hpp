#pragma once
#include <psapi.h>
#include <Windows.h>
#include <iostream>


class Memory
{
private:
	uintptr_t baseModule;
	uintptr_t moduleSize;

	
	uintptr_t localPlayerPtr{ 0 };
	uintptr_t entityListPtr{ 0 };
	uintptr_t entityCountPtr{ 0 };

	struct ScanTask
	{
		const char* name;
		const unsigned char* pattern;
		const char* mask;
		int offset;
		uintptr_t* resultAdress;
	};

public:

	Memory();

	// --- GETTERS ---
	uintptr_t GetBase() const { return baseModule; }
	uintptr_t GetSize() const { return moduleSize; }

	uintptr_t GetLocalPlayer() const { return localPlayerPtr; }
	uintptr_t GetEntityCount() const { return entityCountPtr; }
	uintptr_t GetEntityList() const { return entityListPtr; }


	// --- LE COEUR DU SDK INTERNE ---

	// cast de l'adresse en pointeur de type T et déréférencement 
	template <typename T>
	static T Read(uintptr_t address) {
		if (address == 0) return T{}; // Renvoie la valeur par défaut du type de l'argument
		return *(T*)address;
	}

	template <typename T>
	static void Write(uintptr_t adress, T value) {
		if (adress == 0) return;
		*(T*)adress = value;
	}

	// fonction pour patching d'octet
	static void Patch(uintptr_t adress, const unsigned char* bytes, size_t size) {
		DWORD oldProtect;
		VirtualProtect((void*) adress, size, PAGE_EXECUTE_READWRITE, &oldProtect);

		memcpy((void*)adress, bytes, size);


		VirtualProtect((void*)adress, size, oldProtect, &oldProtect);
	}

	uintptr_t PatternScan(const unsigned char* pattern, const char* mask);

	uintptr_t GetModuleSize();

	bool Initialize();
};
