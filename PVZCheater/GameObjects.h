#pragma once
#include "pch.h"

#define GetInt(base, offset) (*(int*)((BYTE*)base + offset))
#define GetFloat(base, offset) (*(float*)((BYTE*)base + offset))
#define GetByte(base, offset) (*(BYTE*)((BYTE*)base + offset))
// Get offset in a obj
#define GetOffset(obj, attr) ((uintptr_t)((BYTE*)(&attr) - (BYTE*)(obj)))
#define GetMemAddr(pObj, attr) ((uintptr_t)((pObj->addr)+GetOffset(pObj, attr)))

typedef BYTE(ZombieType)[0x168];

struct Zombie {
private:
	BYTE _r1[8];
public:
	// +8
	int xPosI;
	// +C
	int yPosI;
private:
	BYTE _r2[0xC];
public:
	// +1C
	int row;
private:
	BYTE _r3[0x4];
public:
	// +0x24
	int code;
	// +0x28 1->fade away, 2->be black 3->head execution
	int behaviorType;
	// +0x2C
	float xPosF;
	// +0x30
	float yPosF;
	// +0x34
	float slowDownSpeed;
private:
	BYTE _r4[0x80];
public:
	// +0xB8
	BYTE isCharm;
	// +0xB9
	BYTE isBlowToDie;
	// +0xBA
	BYTE isIgnorePlant;
private:
	BYTE _r5;
	BYTE _r6;
public:
	// +0xBD
	BYTE isJumpWater;
private:
	// 0xBE
	BYTE _r7;
public:
	// +0xBF
	BYTE isEatOnion;
private:
	BYTE _r8[8];
public:
	// +0xC8
	int curBlood;
	// +0xCC
	int fullBlood;
	// +0xD0
	int curShield;
	// +0xD4
	int fullShield;
private:
	BYTE _r9[0x14];
public:
	// +0xEC
	BYTE isDead;
private:
	BYTE _r10[0x7B];
public:
	DWORD addr;
public:
	Zombie(DWORD addr) : addr(addr) {};
};


typedef BYTE(PlantType)[0x14C];

struct Plant {
private:
	BYTE _r1[0x8];
public:
	// +8
	int xPos;
	// +C
	int yPos;
private:
	BYTE _r2[0x14];
public:
	// +24
	int code;
private:
	BYTE _r3[0x18];
public:
	// +40
	int curBlood;
	// +44
	int fullBlood;
	// +48
	int isAttackType;
private:
	BYTE _r4[0xC];
public:
	// +58
	int curCD;
	// +5C
	int fullCD;
private:
	BYTE _r5[0xE1];
public:
	// 0x141
	BYTE isDead;
private:
	BYTE _r6[0xA];
public:
	DWORD addr;

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
	BYTE isDead;
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