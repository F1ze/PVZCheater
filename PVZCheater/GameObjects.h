#pragma once
#include "pch.h"

typedef BYTE(ZombieType)[0x168];

struct Zombie {
	DWORD addr;
	// +8
	int xPosI;
	// +C
	int yPosI;
	// +0x24
	int code;
	// +0x2C
	float xPosF;
	// +0x30
	float yPosF;
	// +0x34
	float slowDownSpeed;
	// +0xB8
	BYTE isCharm;
	// +0xB9
	BYTE isBlowToDie;
	// +0xBD
	BYTE isJumpWater;
	// +0xBF
	BYTE isEatOnion;
	// +0xC8
	int curBlood;
	// +0xCC
	int fullBlood;
	// +0xD0
	int curShield;
	// +0xD4
	int fullShield;

	const DWORD xPosIOffset = 0x8;
	const DWORD yPosIOffset = 0xC;
	const DWORD codeOffset = 0x24;
	const DWORD xPosFOffset = 0x2C;
	const DWORD yPosFOffset = 0x30;
	const DWORD slowDownSpeedOffset = 0x34;
	const DWORD isCharmOffset = 0xB8;
	const DWORD isBlowToDieOffset = 0xB9;
	const DWORD isJumpWaterOffset = 0xBD;
	const DWORD isEatOnionOffset = 0xBF;
	const DWORD curBloodOffset = 0xC8;
	const DWORD fullBloodOffset = 0xCC;
	const DWORD curShieldOffset = 0xD0;
	const DWORD fullShieldOffset = 0xD4;

	Zombie(DWORD addr) : addr(addr) {};
};


typedef BYTE(PlantType)[0x14C];

struct Plant {
	DWORD addr;
	// +8
	int xPos;
	// +C
	int yPos;
	// +24
	int code;
	// +40
	int curBlood;
	// +44
	int fullBlood;
	// +48
	int isAttackType;
	// +58
	int curCD;
	// +5C
	int fullCD;

	const DWORD xPosOffset = 0x8;
	const DWORD yPosOffset = 0xC;
	const DWORD codeOffset = 0x24;
	const DWORD curBloodOffset = 0x40;
	const DWORD fullBloodOffset = 0x44;
	const DWORD isAttackTypeOffset = 0x48;
	const DWORD curCDOffset = 0x58;
	const DWORD fullCDOffset = 0x5C;

	Plant(DWORD addr) : addr(addr) {};
};

typedef BYTE(BulletType)[0x94];

struct Bullet {
private:
	BYTE _r1[8];
public:
	// 0x8
	int xPos;
	int yPos;
private:
	BYTE _r2[12];
public:
	// 0x1C
	int row;
private:
	BYTE _r3[16];
public:
	// 0x30
	float xPosF;
	// 0x34
	float yPosF;
private:
	BYTE _r4[24];
public:
	// 0x50
	BYTE isValid;
private:
	BYTE _r5[8+3];
public:
	int code;
private:
	BYTE _r6[4*13];
public:
	DWORD addr;
	Bullet(DWORD addr) : addr(addr) {};
};

// 将得到的数组转化为结构体
Zombie* parseZombie(DWORD startAddr, ZombieType* raw);
Plant* parsePlant(DWORD startAddr, PlantType* raw);
Bullet* parseBullet(DWORD startAddr, BulletType* raw);