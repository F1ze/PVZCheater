#pragma once
#include "common.h"

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
	// +10
	int xWidth;
	// +14
	int yWitdth;
	// +0x18
	BYTE isVisible;
private:
	BYTE _r2[0x3];
public:
	// +1C
	int row;
private:
	BYTE _r3[0x4];
public:
	// +0x24
	int code;
	// +0x28 1->fade away, 2->be black 3->head execution, 32->无视植物
	int behaviorType;
	// +0x2C
	float xPosF;
	// +0x30
	float yPosF;
	// +0x34
	float slowDownSpeed;
private:
	// +0x64 = 10, 会改变僵尸的行动轨迹
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
	// +164 = zbArrLen | ((*(int *)(*(*base868+A8) + 0x14))++ << 16);
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
	// +10
	int xWidth;
	// +14
	int yWitdth;
	// +0x18
	BYTE isVisible;
private:
	BYTE _r2_0[0x3];
public:
	// 0x1C
	int row;
private:
	BYTE _r2_1[0x4];
public:
	// +24
	int code;
	// +28
	int col;
private:
	// +3C 应该是和场景有关的，修改会造成植物移动 2 9  20
	BYTE _r3[0x14];
public:
	// +40
	int curBlood;
	// +44
	int fullBlood;
	// +48
	int isAttackType;
private:
	BYTE _r4[0x4];
public:
	// +50
	int bombCD;
	// +54
	int reloadCD;
	// +58
	int curCD;
	// +5C
	int fullCD;
private:
	BYTE _r5_1[0x28];
	// 0x88
	int row_2;
	BYTE _r5_1_1[0x4];
public:
	// 0x90
	int emitCD;
	// 0x94
	int uniqueID;
private:
	BYTE _r5_2[0xA9];
public:
	// 0x141
	BYTE isDead;
	// 0x142
	BYTE isDead_2;
	// 0x143
	BYTE isDisableAttack;

	// 0x145 是否高亮（铲子移动在植物上会高亮）
private:
	BYTE _r6[0x8];
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
	BYTE _r2[0xC];
public:
	// 0x1C
	int row;
private:
	BYTE _r3[0x10];
public:
	// 0x30
	float xPosF;
	// 0x34
	float yPosF;
private:
	BYTE _r4_1[4];
public:
	// 0x3C
	float xSpeed;
	// 0x40
	float ySpeed;
private:
	// all float
	BYTE _r4_2[0xC];
public:
	// 0x50
	BYTE isDead;
private:
	BYTE _r5_1[4];
public:
	// 0x58 子弹扔出去的方式，0-> 普通行, 1-> 投掷，2->三头其中的两发, 6 -> 倒发 ,9 -> 自动追踪
	int throwType;
public:
	// 0x5C
	int code;
private:
	// 0x74 PlantAttackType 直线型、爆炸型等
	BYTE _r6[4*13];
public:
	DWORD addr;
	Bullet(DWORD addr) : addr(addr) {};
};

enum BulletCode {
	// 0
	BEAN,
	// 1
	ICE_BEAN,
	// 2
	CABBAGE,
	// 3
	WATERMALON,
	// 4
	BUBBLE,
	// 5
	ICE_WATERMALON,
	// 6
	FIRE_BEAN,
	// 7
	STAR,
	// 8
	THORN,
	// 9
	BASKETBALL,
	// 0xA
	CORN_GRAIN,
	// 0xB
	CORN_BOMB,
	// 0xC, 12
	BUTTER,
	// 0xD, 13
	BEAN_1
};

typedef BYTE(LittleCarType)[0x48];

struct LittleCar {
private:
	BYTE _r1[8];
public:
	// 0x8
	float xPos;
	float yPos;
private:
	BYTE _r2[4];
public:
	// 0x14
	int row;
private:
	BYTE _r3[0x14];
public:
	// 0x2C, 行为方式，0-初始化车状态，1-就绪状态，2-碾压状态
	int behaviorType;
	// 0x30
	BYTE isDead;
private:
	BYTE _r4[3];
public:
	// 0x34, the type of little car
	int code;
private:
	BYTE _r5[0x14];
public:
	DWORD addr;
	LittleCar(DWORD addr) : addr(addr) {};
};

// 将得到的数组转化为结构体
Zombie* parseZombie(DWORD startAddr, ZombieType* raw);
Plant* parsePlant(DWORD startAddr, PlantType* raw);
Bullet* parseBullet(DWORD startAddr, BulletType* raw);
LittleCar* parseLittleCar(DWORD startAddr, LittleCarType* raw);