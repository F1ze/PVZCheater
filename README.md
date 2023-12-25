# PVZ Cheater

[TOC]

## 0x0. Introduce

## 0x1. Cheat Notes

| Name | Offset |      |
| ---- | ------ | ---- |
|      |        |      |
|      |        |      |
|      |        |      |

### 1. Sun

Add Sun: 0041E6E0

Minus Sun: 0041E846

Sun Count = [[PlantsVsZombies.exe+329670] + 868] + 5578

### 2. Plant

add plant count: 00420C37

是否可以随意种植判断：004127EF

植物结构：是个数组，每个元素大小为 0x14C

元素结构：

* +8：XPos
* +C：YPos
* +24：植物代码

* +40：当前血量
* +44：总血量
* +148：是否种植，0 表示已种植，1 表示未种植。
* +14A：是否已种植，0表示未种植，非零表示已种植

冰冻菇：1045F0C6

植物技能函数：0046A110

* 原子菇：eax = F
* 

### 3. Zombie

Add Zombie: 00420B87

Zombie Walk: 0053B443

Zombie Array Gap: 0x168

Zombie Structure:

* +2C: XPos(int)	+50 (float)
* +28: YPos(int)     +54(float)
* +58：减速速度，为负数可以击退僵尸
* +C8: 当前血量
* +CC：总血量
* +D0: 当前护盾值
* +D4: 总护盾值
* +EC: 是否死亡  1 表示死亡

Zombie blood decrease: 

* 扣减血量: 00541CDA，00541CE4
* 扣减护盾值：005419FA

### 4. 是否暂停游戏

0061607F

## 0x2. Reference

* https://plantsvszombies.fandom.com/wiki/Hacking_guide#Method_1