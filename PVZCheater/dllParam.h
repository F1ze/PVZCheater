#pragma once
#include "common.h"

struct AddPlantParam
{
	DWORD row;
	DWORD col;
	DWORD plantCode;
	AddPlantParam(DWORD row, DWORD col, DWORD plantCode) : row(row), col(col), plantCode(plantCode) {};
};


struct AddZombieParam
{
	DWORD row;
	DWORD zombieCode;
	AddZombieParam(DWORD row, DWORD zombieCode) : row(row), zombieCode(zombieCode) {};
};

struct GetNextZombieParam
{
	DWORD retValue = 0;
	DWORD startAddress = 0;
	GetNextZombieParam() {};
	GetNextZombieParam(DWORD retValue, DWORD startAddress) : retValue(retValue), startAddress(startAddress) {};
};


struct GetNextPlantParam
{
	DWORD retValue;
	DWORD startAddress;
	GetNextPlantParam() {};
	GetNextPlantParam(DWORD retValue, DWORD startAddress) : retValue(retValue), startAddress(startAddress) {};
};

struct SetPlantAttackSpeedParam
{
	bool flag;
	float rate;
	SetPlantAttackSpeedParam() {};
	SetPlantAttackSpeedParam(bool flag, float rate) : flag(flag), rate(rate) {};
};