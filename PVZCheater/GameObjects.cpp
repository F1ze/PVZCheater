#include "GameObjects.h"

#define GetInt(base, offset) (*(int*)((BYTE*)base + offset))
#define GetFloat(base, offset) (*(float*)((BYTE*)base + offset))
#define GetByte(base, offset) (*(BYTE*)((BYTE*)base + offset))

Zombie* parseZombie(DWORD startAddr, ZombieType* raw)
{
    Zombie* z = new Zombie(startAddr);
    z->xPosI = GetInt(raw, z->xPosIOffset);
    z->yPosI = GetInt(raw, z->yPosIOffset);

    z->code = GetInt(raw, z->codeOffset);
    z->xPosF = GetFloat(raw, z->xPosFOffset);
    z->yPosF = GetFloat(raw, z->yPosFOffset);

    z->slowDownSpeed = GetFloat(raw, z->slowDownSpeedOffset);
    
    z->isCharm = GetByte(raw, z->isCharmOffset);
    z->isBlowToDie = GetByte(raw, z->isBlowToDieOffset);
    z->isJumpWater = GetByte(raw, z->isJumpWaterOffset);
    z->isEatOnion = GetByte(raw, z->isEatOnionOffset);

    z->curBlood = GetInt(raw, z->curBloodOffset);
    z->fullBlood = GetInt(raw, z->fullBloodOffset);
    z->curShield = GetInt(raw, z->curShieldOffset);
    z->fullShield = GetInt(raw, z->fullShieldOffset);
    return z;
}

Plant* parsePlant(DWORD startAddr, PlantType* raw)
{
    Plant* p = new Plant(startAddr);

    p->xPos = GetInt(raw, p->xPosOffset);
    p->yPos = GetInt(raw, p->yPosOffset);

    p->code = GetInt(raw, p->codeOffset);
    p->curBlood = GetInt(raw, p->curBloodOffset);
    p->fullBlood = GetInt(raw, p->fullBloodOffset);

    p->isAttackType = GetInt(raw, p->isAttackTypeOffset);
    p->curCD = GetInt(raw, p->curCDOffset);
    p->fullCD = GetInt(raw, p->fullCDOffset);

    return p;
}

Bullet* parseBullet(DWORD startAddr, BulletType* raw)
{
    Bullet* b = new Bullet(startAddr);
    memcpy_s(b, sizeof(BulletType), raw, sizeof(BulletType));
    return b;
}
