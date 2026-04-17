#pragma once
#include <psapi.h>
#include <windows.h>
#include <iostream>
#include <cmath> 

#include "entity.hpp"
#include "features.hpp"

class Hook
{
public:
	
	typedef void(__cdecl* tShot)(Vector3* origin, Vector3* target, PlayerStruct* owner);

	BYTE* Detour32(BYTE* src, BYTE* dst, const uintptr_t len);

	static void __cdecl hooked_sub_4613B0(Vector3* origin, Vector3* target, PlayerStruct* owner);

	static tShot oShot;

	void unHook();
private:
	
	
	BYTE* targetAddr{ nullptr };      // adresse du JMP
	BYTE originalBytes[10]{ 0 };     //sauvegarde des octets originaux
	uintptr_t hookLen{ 0 };         
};

