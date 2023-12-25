#pragma once
#include "pch.h"

struct DLLStatus {
	DLLStatus() {
		MH_Initialize();
	}
};

struct AddPlantParam
{
	DWORD row;
	DWORD col;
	DWORD plantCode;
	AddPlantParam(DWORD row, DWORD col, DWORD plantCode) : row(row), col(col), plantCode(plantCode) {};
};

EXPORT void AddPlant(AddPlantParam *param);

struct AddZombieParam
{
	DWORD row;
	DWORD zombieCode;
	AddZombieParam(DWORD row, DWORD zombieCode) : row(row), zombieCode(zombieCode) {};
};

EXPORT void AddZombie(AddZombieParam* param);


EXPORT void FreezeAllZombie();
EXPORT void KillAllZombie();


struct GetNextZombieParam
{
	DWORD retValue;
	DWORD startAddress;
	GetNextZombieParam(DWORD retValue, DWORD startAddress) : retValue(retValue), startAddress(startAddress) {};
};

EXPORT void GetNextZombie(GetNextZombieParam *param);

EXPORT void RandomBullet(bool* flag);