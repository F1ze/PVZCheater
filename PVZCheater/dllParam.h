#pragma once
#include "pch.h"

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