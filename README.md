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

紫卡判断是否可用关键 call：0040FDE0

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
* +48：是否为发射类型
* +58：技能CD
* +90：发射类植物的发射CD

冰冻菇：1045F0C6

植物技能函数：0046A110

* 原子菇：eax = F

植物定时：

* 阳光：00466E03
* 射手：00466E10

### 3. Zombie

Add Zombie: 00420B87

Zombie Walk: 0053B443

Zombie Array Gap: 0x168

Zombie Count decrese / add : 41E9E6

Zombie Structure:

* +8: XPos(int)	+2C (float)
* +C: YPos(int)   +30(float)
* +34：减速速度，为负数可以击退僵尸
* +B8:是否魅惑
* +b9: 是否被吹
* +C8: 当前血量
* +CC：总血量
* +D0: 当前护盾值
* +D4: 总护盾值

Zombie blood decrease: 

* 扣减血量: 00541CDA，00541CE4
* 扣减护盾值：005419FA

Zombie Pos:

* blow: 0053B68D， label bollonzombie: 0046A07D

### 4. 是否暂停游戏

0061607F

## 0x2. Reference

* https://plantsvszombies.fandom.com/wiki/Hacking_guide#Method_1