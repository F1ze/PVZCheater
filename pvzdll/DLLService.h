#pragma once
#include "common.h"
#include "logger.h"

struct DLLStatus {
	DLLStatus() {
		MH_Initialize();
	}

	~DLLStatus() {
		MH_Uninitialize();
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
EXPORT void KillAllZombie(int *type);


struct GetNextZombieParam
{
	DWORD retValue;
	DWORD startAddress;
	GetNextZombieParam(DWORD retValue, DWORD startAddress) : retValue(retValue), startAddress(startAddress) {};
};

// Deprecated
EXPORT void GetNextZombie(GetNextZombieParam *param);


struct GetNextPlantParam
{
	DWORD retValue;
	DWORD startAddress;
	GetNextPlantParam(DWORD retValue, DWORD startAddress) : retValue(retValue), startAddress(startAddress) {};
};

EXPORT void GetNextPlant(GetNextPlantParam* param);

EXPORT void RandomBullet(bool* flag);

EXPORT void SetSpecificBullet(int* bulletType);

EXPORT void BombFullScreen(bool* flag);

EXPORT void BulletAutoTrack(bool* flag);

EXPORT void PlantLowHPSacrifice(bool* flag);

struct SetPlantAttackSpeedParam
{
	bool flag;
	float rate;
	SetPlantAttackSpeedParam(bool flag, float rate) : flag(flag), rate(rate) {};
};

EXPORT void SetPlantAttackSpeed(SetPlantAttackSpeedParam* p);

EXPORT void RestoreLittleCar();