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
		NotInGame:
		mov zbArr, 0
		Finish:
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
		NotInGame:
		mov pltArr, 0
		Finish:
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
	if (bulletMoveType == 0 || bulletMoveType == 2 || bulletMoveType == 6) {
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

	return oriBulletMoveFn(bulletAddr);
}

void BulletAutoTrack(bool* flag)
{
	static std::atomic<bool> isCreateHook = false;
	static bool hasEnableHook = false;

	bool expect = false;

	LPVOID func = (LPVOID)0x471F70;

	if (isCreateHook.compare_exchange_strong(expect, true)) {
		MH_CreateHook(func, &detourBulletMoveFn, reinterpret_cast<LPVOID*>(&oriBulletMoveFn));
	}

	if (*flag) {
		if (hasEnableHook == false && MH_EnableHook(func) == MH_OK) hasEnableHook = true;
		DWORD old;
		VirtualProtect((LPVOID)0x46B143, 2, PAGE_EXECUTE_READWRITE, &old);
		*(WORD*)(0x46B143) = 0x43EB;
		VirtualProtect((LPVOID)0x46B143, 2, old, &old);
	}
	else
	{
		//MH_DisableHook(func);
		if (hasEnableHook && MH_DisableHook(func) == MH_OK) hasEnableHook = false;
		DWORD old;
		VirtualProtect((LPVOID)0x46B143, 2, PAGE_EXECUTE_READWRITE, &old);
		*(WORD*)(0x46B143) = 0x4374;
		VirtualProtect((LPVOID)0x46B143, 2, old, &old);
	}
}


// ==================== Plant low hp sacrifice ============================
typedef void(*ZombieEatPlant)();
ZombieEatPlant oriZombieEatPlant;

__declspec(naked) void detourZombieEatPlant() {
	__asm {
		pushad
		cmp dword ptr [ecx+0x40], 0xF
		jg finish
		mov dword ptr [ecx+0x24], 0x11
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
		mov[esi + 0x90], 2
		push 140
		fild ds: [esp]
		fld attackSpeedRate
		fmulp   st(1), st(0)
		fistp ds : [esp]
		mov ebx, 150
		sub ebx, ds : [esp]
		mov [esi+0x5C], ebx
		pop ecx
		popad
		jmp oriNormalBeanPlantAttackSpeedFn
	}
}

void SetPlantAttackSpeed(SetPlantAttackSpeedParam* p)
{
	static std::atomic<bool> isCreateHook = false;
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

			DWORD prt;
			auto je1 = (LPVOID)0x46299D;
			VirtualProtect(je1, 2, PAGE_EXECUTE_READWRITE, &prt);
			*(WORD*)je1 = 0x28EB;
			VirtualProtect(je1, 2, prt, &prt);
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

			// 还原所有原来的CD
			auto arr = getAllPlant();
			for (auto plt : arr) {
				int code = *(int*)(plt + 0x24);
				if (code == 0 || code == 5 || code == 7 || code == 28 || code == 52) {
					*(int*)(plt + 0x5C) = 150;
				}
			}
		}

	}
}
