#pragma once

#include <cmath>
#include "math.hpp"

class PlayerStruct
{
public:
	void* vfTable; //0x0000
	Vector3 pos; //0x0004
	char pad_0010[48]; //0x0010
	Vector2 viewAngle; //0x0040
	char pad_0048[40]; //0x0048
	bool stand; //0x0070
	bool scoping; //0x0071
	char pad_0072[14]; //0x0072
	int8_t forwardBackward; //0x0080
	int8_t leftRight; //0x0081
	bool dead; //0x0082
	char pad_0083[117]; //0x0083
	uint32_t healt; //0x00F8
	uint32_t armor; //0x00FC
	char pad_0100[4]; //0x0100
	int32_t selecWeapon; //0x0104
	char pad_0108[12]; //0x0108
	uint32_t secAmmo; //0x0114
	char pad_0118[16]; //0x0118
	uint32_t primAmmo; //0x0128
	char pad_012C[16]; //0x012C
	uint32_t secMag; //0x013C
	char pad_0140[16]; //0x0140
	uint32_t primMag; //0x0150
	char pad_0154[4]; //0x0154
	uint32_t grenade; //0x0158
	char pad_015C[148]; //0x015C
	int32_t ping; //0x01F0
	uint32_t clientNum; //0x01F4
	char pad_01F8[308]; //0x01F8
	int32_t team; //0x032C
	char pad_0330[8]; //0x0330
	int32_t spec; //0x0338
	char pad_033C[265]; //0x033C
}; //Size: 0x0445