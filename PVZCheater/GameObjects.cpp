#include "GameObjects.h"

Zombie* parseZombie(DWORD startAddr, ZombieType* raw)
{
    Zombie* z = new Zombie(startAddr);
    memcpy_s(z, sizeof(ZombieType), raw, sizeof(ZombieType));
    return z;
}

Plant* parsePlant(DWORD startAddr, PlantType* raw)
{
    Plant* p = new Plant(startAddr);
    memcpy_s(p, sizeof(PlantType), raw, sizeof(PlantType));
    return p;
}

Bullet* parseBullet(DWORD startAddr, BulletType* raw)
{
    Bullet* b = new Bullet(startAddr);
    memcpy_s(b, sizeof(BulletType), raw, sizeof(BulletType));
    return b;
}
