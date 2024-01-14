#include "pch.h"
#include "DLLService.h"

EXPORT void AddPlant(AddPlantParam* param)
{
	if (param->plantCode > 52) return;

	DWORD row = param->row;
	DWORD col = param->col;
	if (row > 4) row = -1;
	if (col > 8) col = -1;
	DWORD plantCode = param->plantCode;

	HMODULE hMod = GetModuleHandle(NULL);

	DWORD base = (DWORD)hMod + 0x329670;

	void* func = (void*)0x40FA10;

	if (row == -1 && col == -1) return;

	if (row == -1) {
		for (size_t i = 0; i < 5; i++)
		{
			row = i;
			__asm {
				mov edx, 0xffffffff
				push edx
				mov eax, plantCode
				push eax
				mov eax, row
				mov esi, col
				push esi
				mov edi, base
				mov edi, ds: [edi]
				mov edi, ds : [edi + 0x868]
				push edi
				call func
			}
		}
	}
	else if (col == -1) {
		for (size_t i = 0; i < 9; i++)
		{
			col = i;
			__asm {
				mov edx, 0xffffffff
				push edx
				mov eax, plantCode
				push eax
				mov eax, row
				mov esi, col
				push esi
				mov edi, base
				mov edi, ds: [edi]
				mov edi, ds : [edi + 0x868]
				push edi
				call func
			}
		}
	}
	else {
		__asm {
			mov edx, 0xffffffff
			push edx
			mov eax, plantCode
			push eax
			mov eax, row
			mov esi, col
			push esi
			mov edi, base
			mov edi, ds: [edi]
			mov edi, ds : [edi + 0x868]
			push edi
			call func
		}
	}




}

void AddZombie(AddZombieParam* param)
{
	DWORD row = param->row;
	DWORD code = param->zombieCode;
	if (row > 4 || code > 36) return;

	HMODULE hMod = GetModuleHandle(NULL);

	DWORD base = (DWORD)hMod + 0x329670;

	void* func = (void*)0x410700;

	__asm {
		mov eax, row
		push eax
		mov esi, code
		push esi
		mov edi, base
		mov edi, ds: [edi]
		mov edi, ds : [edi + 0x868]
		mov eax, edi
		call func
	}
}

std::vector<DWORD> getAllZombie()
{
	HMODULE hMod = GetModuleHandle(NULL);
	DWORD base = (DWORD)hMod + 0x329670;

	DWORD zbArr = 0;
	DWORD len = 0;

	__asm {
		pushad
		mov eax, base
		mov eax, ds: [eax]
		mov eax, ds : [eax + 0x868]
		test eax, eax
		je NotInGame
		mov ebx, ds : [eax + 0xA8]
		mov zbArr, ebx
		mov eax, ds : [eax + 0xAC]
		mov len, eax
		jmp Finish
		NotInGame :
		mov zbArr, 0
			Finish :
			popad
	}

	std::vector<DWORD> arr;
	if (zbArr != 0) {
		const int zbSz = 0x168;
		DWORD arrEnd = zbArr + len * zbSz;

		for (DWORD i = zbArr; i < arrEnd; i += zbSz)
		{
			BYTE sign = *(BYTE*)(i + 0xEC);
			int sign0x164 = *(int*)(i + 0x164);
			if (!sign && (sign0x164 & 0xFFFF0000) != 0) arr.push_back(i);
		}
	}
	return arr;

}


std::vector<DWORD> getAllPlant()
{
	HMODULE hMod = GetModuleHandle(NULL);
	DWORD base = (DWORD)hMod + 0x329670;

	DWORD pltArr = 0;
	DWORD len = 0;

	__asm {
		pushad
		mov eax, base
		mov eax, ds: [eax]
		mov eax, ds : [eax + 0x868]
		test eax, eax
		je NotInGame
		mov ebx, ds : [eax + 0xC4]
		mov pltArr, ebx
		mov eax, ds : [eax + 0xC8]
		mov len, eax
		jmp Finish
		NotInGame :
		mov pltArr, 0
			Finish :
			popad
	}

	std::vector<DWORD> arr;
	if (pltArr) {
		const int pltSz = 0x14C;
		DWORD arrEnd = pltArr + len * pltSz;

		for (DWORD i = pltArr; i < arrEnd; i += pltSz)
		{
			BYTE sign = *(BYTE*)(i + 0x141);
			int sign0x148 = *(int*)(i + 0x148);
			if (!sign && (sign0x148 & 0xFFFF0000) != 0) arr.push_back(i);
		}
	}
	return arr;

}

void FreezeAllZombie()
{
	HMODULE hMod = GetModuleHandle(NULL);
	DWORD base = (DWORD)hMod + 0x329670;

	void* func = (void*)0x469E90;
	__asm {

		//mov ebx, [base]
		//mov ebx, [ebx + 868]
		//mov ecx, ebx
		//mov ebx, [ebx + c4]
		//mov edi, ebx

		//mov[edi], base
		//mov[edi + 4], ecx

		//call 469E90

		mov ebx, base
		mov ebx, ds: [ebx]
		mov ebx, ds : [ebx + 0x868]
		mov ecx, ebx
		mov ebx, ds : [ebx + 0xc4]
		mov edi, ebx

		mov eax, base
		mov ds : [edi] , eax
		mov ds : [edi + 4] , ecx
		call func
	}
}

void KillAllZombie(int* type)
{

	HANDLE h = GetCurrentProcess();

	HMODULE hMod = GetModuleHandle(NULL);
	DWORD base = (DWORD)hMod + 0x329670;

	auto zbArr = getAllZombie();
	if (type == nullptr || *type == 0)
	{
		void* func = (void*)0x543540;

		for (auto item : zbArr) {
			__asm {
				pushad
				mov ecx, item
				call func
				popad
			}
		}
	}
	else if (*type == 1)
	{
		for (auto item : zbArr) {
			*(DWORD*)(item + 0x28) = 3;
		}
	}
	else {
		for (auto item : zbArr) {
			*(BYTE*)(item + 0xEC) = 1;
		}
	}
}

void GetNextZombie(GetNextZombieParam* param)
{
	DWORD startAddress = param->startAddress;

	HMODULE hMod = GetModuleHandle(NULL);
	DWORD base = (DWORD)hMod + 0x329670;

	DWORD p1 = 0;
	DWORD* p2 = &param->startAddress;
	DWORD retV = 0;
	void* func = (void*)0x41F6B0;
	__asm {
		mov eax, base
		mov eax, ds: [eax]
		mov eax, ds : [eax + 0x868]
		mov p1, eax
	}

	if (startAddress == 0) {
		__asm {
			mov eax, p1
			mov eax, ds: [eax + 0xA8]
			mov startAddress, eax
		}
		param->startAddress = startAddress;
	}

	__asm {
		mov edx, p1
		mov esi, p2
		call func
		test al, al
		jne labelB
		mov retV, 0
		jmp labelC
		labelB :
		mov retV, 1
			labelC :
	}
	param->retValue = retV;
}


void GetNextPlant(GetNextPlantParam* param)
{
	DWORD startAddress = param->startAddress;

	HMODULE hMod = GetModuleHandle(NULL);
	DWORD base = (DWORD)hMod + 0x329670;

	DWORD p1 = 0;
	DWORD* p2 = &param->startAddress;
	DWORD retV = 0;
	void* func = (void*)0x41F710;

	__asm
	{
		mov edx, base
		mov edx, ds: [edx]
		mov edx, ds : [edx + 0x868]
		mov esi, p2
		call func
		test al, al
		mov retV, 0
		je l1
		mov retV, 1
		l1:
	}

	param->retValue = retV;
}

// ===============================   Hook Part ===================================

bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5) return false;
	DWORD prt;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &prt);

	uintptr_t relativeOffset = dst - src - 5;
	*src = 0xE9;
	*(uintptr_t*)(src + 1) = relativeOffset;

	VirtualProtect(src, len, prt, &prt);
	return true;
}


BYTE* TramHook32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5) return 0;
	BYTE* gateway = (BYTE*)VirtualAlloc(nullptr, len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	memcpy_s(gateway, len, src, len);
	uintptr_t relativeOffset = src - gateway - 5;

	*(gateway + len) = 0xE9;
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = relativeOffset;

	Detour32(src, dst, len);
	return gateway;
}


//  =================  Bomb all screen hook  =====================

typedef int(WINAPI* CherryOrDoomrBombKill)(int base868, int originRow, int originXPos, int originYPos, int damagePixelRadius, int damagaRowRadius, char isToBlack, char a9_7F);
CherryOrDoomrBombKill oriCherryOrDoomrBombKill;

int WINAPI detourCherryOrDoomrBombKill(int base868, int originRow, int originXPos, int originYPos, int damagePixelRadius, int damagaRowRadius, char isToBlack, char a9_7F) {
	damagaRowRadius = 10;
	damagePixelRadius = 900;
	return oriCherryOrDoomrBombKill(base868, originRow, originXPos, originYPos, damagePixelRadius, damagaRowRadius, isToBlack, a9_7F);
}

void BombFullScreen(bool* flag)
{
	static std::atomic<bool> isCreateHook = false;
	static bool hasEnableHook = false;

	bool expect = false;

	LPVOID func = (LPVOID)0x420670;

	if (isCreateHook.compare_exchange_strong(expect, true)) {
		MH_CreateHook(func, &detourCherryOrDoomrBombKill, reinterpret_cast<LPVOID*>(&oriCherryOrDoomrBombKill));
	}

	if (*flag) {
		if (hasEnableHook == false && MH_EnableHook(func) == MH_OK) hasEnableHook = true;
	}
	else
	{
		//MH_DisableHook(func);
		if (hasEnableHook && MH_DisableHook(func) == MH_OK) hasEnableHook = false;
	}
}

// ===============================   Random Bullet Part start ===================================

// 470E29
typedef void (*RandomBulletFn)();

RandomBulletFn oriRandomBullet;

// https://stackoverflow.com/questions/4099026/how-to-hook-usercall-userpurge-spoils-functions
__declspec(naked) void detourRandomBullet()
{
	__asm
	{
		pushad
		call rand
		cdq
		mov ecx, 13
		idiv ecx
		mov eax, edx
		mov ss : [esp + 0x1C] , eax
		popad
		jmp oriRandomBullet
	}
}


void RandomBullet(bool* flag)
{
	auto func = (LPVOID)0x470DF0;

	if (*flag)
	{
		oriRandomBullet = (RandomBulletFn)0x470E29;
		oriRandomBullet = (RandomBulletFn)TramHook32((BYTE*)oriRandomBullet, (BYTE*)detourRandomBullet, 5);
	}
	else {
		void* start = (void*)0x470E29;
		BYTE ori[] = { 0x89, 0x45, 0x5C, 0x8B, 0xC6 };
		DWORD prt;
		VirtualProtect(start, 5, PAGE_EXECUTE_READWRITE, &prt);
		memcpy_s(start, sizeof(ori), ori, sizeof(ori));
		VirtualProtect(start, 5, prt, &prt);
	}
}


// ================== Bullet Auto Track hook ======================

typedef int(__stdcall* BulletMoveFn)(int bulletAddr);
BulletMoveFn oriBulletMoveFn;

int __stdcall detourBulletMoveFn(int bulletAddr) {

	int bulletMoveType = *(int*)(bulletAddr + 0x58);
	float& xSpeed = *(float*)(bulletAddr + 0x3C);
	// 7 -> star
	if (bulletMoveType <= 2 || bulletMoveType == 6 || bulletMoveType == 7) {
		auto arr = getAllZombie();
		int sz = arr.size();
		if (sz > 0) {
			// bullet move type
			*(int*)(bulletAddr + 0x58) = 9;
			int idx = rand() % (int)arr.size();
			// prior to attack zombies which xpos < 200.
			std::sort(arr.begin(), arr.end(), [](DWORD a, DWORD b) {
				int ax = *(int*)(a + 0x8);
				int bx = *(int*)(b + 0x8);
				return ax < bx;
				});
			if (*(int*)(arr[0] + 0x8) <= 200) idx = 0;
			int zb = arr[idx];
			*(int*)(bulletAddr + 0x88) = *(int*)(zb + 0x164);
		}
	}
	else if (bulletMoveType == 9) {
		if (xSpeed == 0) {
			*(int*)(bulletAddr + 0x58) = 0;
			*(int*)(bulletAddr + 0x88) = 0;
			xSpeed = 3;
		}
	}

	int yPos = *(int*)(bulletAddr + 0xC);
	if (yPos < -600 || yPos > 2000) *(BYTE*)(bulletAddr + 0x50) = 1;

	return oriBulletMoveFn(bulletAddr);
}

void BulletAutoTrack(bool* flag)
{
	static std::atomic<bool> isCreateHook = false;
	static bool hasEnableHook = false;

	bool expect = false;

	LPVOID func = (LPVOID)0x471F70;
	// Throw Bullet Movement -> Non-throw
	LPVOID throwBulletMovementTarget = (LPVOID)0x472380;
	BYTE throwBulletMovementPatch[] = { 0xEB, 0x07 };
	BYTE throwBulletMovementOrigin[] = { 0x75, 0x07 };

	// can attack zombies behind plants
	LPVOID attackBehindTarget = (LPVOID)0x46B29A;
	BYTE attackBehindPatch[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	BYTE attackBehindOrigin[] = { 0x0F, 0x8C, 0xA1, 0x00, 0x00, 0x00 };

	// Star Auto Track
	LPVOID starPatchTarget = (LPVOID)0x462E3B;
	BYTE starPatch[] = { 0xE9, 0xE6, 0x01, 0x0, 0x0, 0x90 };
	BYTE starPatchOrigin[] = { 0x0F, 0x84, 0xC1, 0x01, 0x0, 0x0 };


	if (isCreateHook.compare_exchange_strong(expect, true)) {
		MH_CreateHook(func, &detourBulletMoveFn, reinterpret_cast<LPVOID*>(&oriBulletMoveFn));
	}

	if (*flag) {
		if (hasEnableHook == false && MH_EnableHook(func) == MH_OK) hasEnableHook = true;
		DWORD old;
		VirtualProtect((LPVOID)0x46B143, 2, PAGE_EXECUTE_READWRITE, &old);
		*(WORD*)(0x46B143) = 0x43EB;
		VirtualProtect((LPVOID)0x46B143, 2, old, &old);
		//// Throw Bullet Movement -> Non-throw
		//VirtualProtect(throwBulletMovementTarget, 6, PAGE_EXECUTE_READWRITE, &old);
		//memcpy_s(throwBulletMovementTarget, sizeof(throwBulletMovementPatch), throwBulletMovementPatch, sizeof(throwBulletMovementPatch));
		//VirtualProtect(throwBulletMovementTarget, 6, old, &old);

		//// can attack zombies behind plants
		//VirtualProtect(attackBehindTarget, 6, PAGE_EXECUTE_READWRITE, &old);
		//memcpy_s(attackBehindTarget, sizeof(attackBehindPatch), attackBehindPatch, sizeof(attackBehindPatch));
		//VirtualProtect(attackBehindTarget, 6, old, &old);

		//// star auto track
		//VirtualProtect(starPatchTarget, 6, PAGE_EXECUTE_READWRITE, &old);
		//memcpy_s(starPatchTarget, sizeof(starPatch), starPatch, sizeof(starPatch));
		//VirtualProtect(starPatchTarget, 6, old, &old);
	}
	else
	{
		//MH_DisableHook(func);
		if (hasEnableHook && MH_DisableHook(func) == MH_OK) hasEnableHook = false;
		DWORD old;
		VirtualProtect((LPVOID)0x46B143, 2, PAGE_EXECUTE_READWRITE, &old);
		*(WORD*)(0x46B143) = 0x4374;
		VirtualProtect((LPVOID)0x46B143, 2, old, &old);
		//// Throw Bullet Movement -> Non-throw
		//VirtualProtect(throwBulletMovementTarget, 6, PAGE_EXECUTE_READWRITE, &old);
		//memcpy_s(throwBulletMovementTarget, sizeof(throwBulletMovementOrigin), throwBulletMovementOrigin, sizeof(throwBulletMovementOrigin));
		//VirtualProtect(throwBulletMovementTarget, 6, old, &old);
		//// can attack zombies behind plants
		//VirtualProtect(attackBehindTarget, 6, PAGE_EXECUTE_READWRITE, &old);
		//memcpy_s(attackBehindTarget, sizeof(attackBehindOrigin), attackBehindOrigin, sizeof(attackBehindOrigin));
		//VirtualProtect(attackBehindTarget, 6, old, &old);
		//// star auto track
		//VirtualProtect(starPatchTarget, 6, PAGE_EXECUTE_READWRITE, &old);
		//memcpy_s(starPatchTarget, sizeof(starPatchOrigin), starPatchOrigin, sizeof(starPatchOrigin));
		//VirtualProtect(starPatchTarget, 6, old, &old);
	}
}


// ==================== Plant low hp sacrifice ============================
typedef void(*ZombieEatPlant)();
ZombieEatPlant oriZombieEatPlant;

__declspec(naked) void detourZombieEatPlant() {
	__asm {
		pushad
		cmp dword ptr[ecx + 0x40], 0xF
		jg finish
		mov dword ptr[ecx + 0x24], 0x11
		finish:
		popad
			jmp oriZombieEatPlant
	}
}


void PlantLowHPSacrifice(bool* flag)
{

	if (*flag)
	{
		oriZombieEatPlant = (ZombieEatPlant)0x5404D0;
		oriZombieEatPlant = (ZombieEatPlant)TramHook32((BYTE*)oriZombieEatPlant, (BYTE*)detourZombieEatPlant, 7);
	}
	else {
		void* start = (void*)0x5404D0;
		BYTE ori[] = { 0x6A, 0xFF, 0x68, 0x48, 0xBC, 0x6C, 0x00 };
		DWORD prt;
		VirtualProtect(start, 7, PAGE_EXECUTE_READWRITE, &prt);
		memcpy_s(start, sizeof(ori), ori, sizeof(ori));
		VirtualProtect(start, 7, prt, &prt);
	}
}

// ==================== Plant Attack Speed ============================

float attackSpeedRate = 0.0;

// 普通卡豌豆射手类
typedef void(*NormalBeanPlantAttackSpeedFn)();
NormalBeanPlantAttackSpeedFn oriNormalBeanPlantAttackSpeedFn;

__declspec(naked) void detourSetNormalBeanPlantAttackSpeedFn() {
	__asm {
		pushad
		mov eax, 0x729670
		mov eax, ds: [eax]
		mov eax, ds : [eax + 0x868]
		mov eax, ds : [eax + 0xB8]
		test eax, eax	// 判断僵尸数量为0
		je Finish
		mov[esi + 0x90], 2
		Finish :
		push 140
		fild ds : [esp]
		fld attackSpeedRate
		fmulp   st(1), st(0)
		fistp ds : [esp]
		mov ebx, 150
		sub ebx, ds : [esp]
		mov[esi + 0x5C], ebx
		pop ecx
		popad
		jmp oriNormalBeanPlantAttackSpeedFn
	}
}

typedef void(*OtherPlantAttackSpeedFn)();
OtherPlantAttackSpeedFn oriOtherPlantAttackSpeedFn;

__declspec(naked) void detourSetOtherPlantAttackSpeedFn() {
	__asm {
		pushad
		mov eax, 0x729670
		mov eax, ds: [eax]
		mov eax, ds : [eax + 0x868]
		mov eax, ds : [eax + 0xB8]
		test eax, eax	// 判断僵尸数量为0
		je Finish
		mov[esi + 0x90], 2
		Finish :
		push 140
		fild ds : [esp]
		fld attackSpeedRate
		fmulp   st(1), st(0)
		fistp ds : [esp]
		mov ebx, 150
		sub ebx, ds : [esp]
		mov[esi + 0x5C], ebx
		pop ecx

		popad
		jmp oriOtherPlantAttackSpeedFn
	}
}

void SetPlantAttackSpeed(SetPlantAttackSpeedParam* p)
{
	static std::atomic<bool> isCreateHook = false;
	// Star, watermalen, corn, fume-shroom, puff, scaredy
	static std::vector<DWORD> otherPlant90 = { 0x46306E, 0x462BBC, 0x462B89, 0x462A86, 0x462ADB + 6, 0x462AF5 + 6,  0x4629EE + 1, 0x462DA2 + 6, /*splitpea*/0x46293A + 6 };
	static std::vector<DWORD> otherPlant90Origin = { 0x28, 0x24, 0x1E, 0x32, 0x1D, 0x19, 0x20, 0x23, 0x1A };

	static std::vector<DWORD> otherPlant90Byte = { /*仙人掌*/0x462BA4,  /*大喷菇*/0x4682E5+2, /*大大喷菇*/0x468327+2, 0x468360 +2, /*机枪射手*/0x468488 + 2, /*猫草*/0x4684B0 + 6, /*三巨头*/0x4684EA + 2};
	static std::vector<BYTE> otherPlant90OriginByte = { 0x23, 0xF, 0x6C, 0x7E, 0x12, 0x13, 0x12};

	bool expect = false;
	// 具体修改逻辑在函数462840中
	if (p->flag)
	{
		attackSpeedRate = p->rate;
		if (isCreateHook.compare_exchange_strong(expect, true)) {
			// 第一次创建hook

			// 普通卡豌豆射手类
			oriNormalBeanPlantAttackSpeedFn = (NormalBeanPlantAttackSpeedFn)0x462990;
			oriNormalBeanPlantAttackSpeedFn = (NormalBeanPlantAttackSpeedFn)TramHook32((BYTE*)oriNormalBeanPlantAttackSpeedFn, (BYTE*)detourSetNormalBeanPlantAttackSpeedFn, 10);
			oriNormalBeanPlantAttackSpeedFn = (NormalBeanPlantAttackSpeedFn)((DWORD)oriNormalBeanPlantAttackSpeedFn + 10);
			// nop掉其他多发的射手
			DWORD prt;
			auto je1 = (LPVOID)0x46299D;
			VirtualProtect(je1, 2, PAGE_EXECUTE_READWRITE, &prt);
			*(WORD*)je1 = 0x28EB;
			VirtualProtect(je1, 2, prt, &prt);

			// 其他植物类型
			oriOtherPlantAttackSpeedFn = (OtherPlantAttackSpeedFn)0x46322C;
			oriOtherPlantAttackSpeedFn = (NormalBeanPlantAttackSpeedFn)TramHook32((BYTE*)oriOtherPlantAttackSpeedFn, (BYTE*)detourSetOtherPlantAttackSpeedFn, 6);

			for (auto item : otherPlant90) {
				VirtualProtect((LPVOID)item, 4, PAGE_EXECUTE_READWRITE, &prt);
				*(DWORD*)item = 0x2;
				VirtualProtect((LPVOID)item, 4, prt, &prt);
			}

			for (auto item : otherPlant90Byte) {
				VirtualProtect((LPVOID)item, 1, PAGE_EXECUTE_READWRITE, &prt);
				*(BYTE*)item = 0x1;
				VirtualProtect((LPVOID)item, 1, prt, &prt);
			}


		}

	}
	else
	{
		expect = true;
		if (isCreateHook.compare_exchange_strong(expect, false)) {
			DWORD prt;

			// 还原现场

			// 普通卡豌豆射手类
			void* start = (void*)0x462990;
			BYTE ori[] = { 0xC7, 0x86, 0x90, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00 };
			VirtualProtect(start, 10, PAGE_EXECUTE_READWRITE, &prt);
			memcpy_s(start, sizeof(ori), ori, sizeof(ori));
			VirtualProtect(start, 10, prt, &prt);

			auto je1 = (LPVOID)0x46299D;
			VirtualProtect(je1, 2, PAGE_EXECUTE_READWRITE, &prt);
			*(WORD*)je1 = 0x3174;
			VirtualProtect(je1, 2, prt, &prt);

			// 其他植物类型
			start = (void*)0x46322C;
			BYTE otherPlantOri[] = { 0x89, 0x4E, 0x58, 0x83, 0xF8, 0x12 };
			VirtualProtect(start, 6, PAGE_EXECUTE_READWRITE, &prt);
			memcpy_s(start, sizeof(otherPlantOri), otherPlantOri, sizeof(otherPlantOri));
			VirtualProtect(start, 6, prt, &prt);

			for (size_t i = 0; i < otherPlant90.size(); i++) {
				auto item = (LPVOID)otherPlant90[i];
				VirtualProtect(item, 4, PAGE_EXECUTE_READWRITE, &prt);
				*(DWORD*)item = otherPlant90Origin[i];
				VirtualProtect(item, 4, prt, &prt);
			}

			for (size_t i = 0; i < otherPlant90Byte.size(); i++) {
				auto item = (LPVOID)otherPlant90Byte[i];
				VirtualProtect(item, 1, PAGE_EXECUTE_READWRITE, &prt);
				*(BYTE*)item = otherPlant90OriginByte[i];
				VirtualProtect(item, 1, prt, &prt);
			}

			// 还原所有原来的CD
			auto arr = getAllPlant();
			for (auto plt : arr) {
				int code = *(int*)(plt + 0x24);
				int isAttackType = *(int*)(plt + 0x48);
				if (isAttackType) {
					if (code == 32 || code == 34 || code == 39 || code == 44) {
						*(int*)(plt + 0x5C) = 300;
					}
					else {
						*(int*)(plt + 0x5C) = 150;
					}
				}

			}
		}

	}
}
