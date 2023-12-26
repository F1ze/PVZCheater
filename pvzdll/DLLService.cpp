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
	//alloc(cz, 50)
	//createThread(cz)
	//define(base, PlantsVsZombies.exe + 329670)
	//cz:
	//mov eax, 1
	//push eax
	//mov esi, 13
	//push esi
	//mov edi, [base]
	//mov edi, [edi + 868]
	//mov eax, edi
	//call 410700
	//retn
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
		mov edi, ds: [edi + 0x868]
		mov eax, edi
		call func
	}
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
		mov ebx, ds: [ebx + 0x868]
		mov ecx, ebx
		mov ebx, ds: [ebx + 0xc4]
		mov edi, ebx

		mov eax, base
		mov ds: [edi], eax
		mov ds: [edi + 4], ecx
		call func
	}
}

void KillAllZombie()
{

	HANDLE h = GetCurrentProcess();

	HMODULE hMod = GetModuleHandle(NULL);
	DWORD base = (DWORD)hMod + 0x329670;

	void* func = (void*)0x420670;
	
	DWORD a = 0x7D809090;

	WriteProcessMemory(h, (LPVOID)0x4206F3, &a, sizeof(DWORD), NULL);

	__asm {
		mov ecx, 0x7f
		mov edi, 0xdc
		mov esi, 0x50
		mov edx, 0x3
		mov eax, base
		mov eax, ds: [eax]
		mov eax, ds: [eax + 0x868]


		push ecx
		push 0x1
		push 0x3
		push 0xFA
		push edi
		push esi
		push edx
		push eax
		call func
	}
	a = 0x7D803D74;
	WriteProcessMemory(h, (LPVOID)0x4206F3, &a, sizeof(DWORD), NULL);
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
		labelB:
		mov retV , 1
		labelC:
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
		mov edx, ds: [edx + 0x868]
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

// ===============================   Random Bullet Part start ===================================

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

typedef void (*sub_470E29)();

sub_470E29 fn;

// https://stackoverflow.com/questions/4099026/how-to-hook-usercall-userpurge-spoils-functions
__declspec(naked) void func_hook()
{
	__asm 
	{
		pushad
		call rand
		cdq
		mov ecx,13
		idiv ecx
		mov eax,edx
		mov ss: [esp+0x1C], eax
		popad
		jmp fn
	}
}


void RandomBullet(bool* flag)
{
	auto func = (LPVOID)0x470DF0;

	static bool hasCreate = false;


	if (*flag)
	{
		fn = (sub_470E29)0x470E29;
		fn = (sub_470E29)TramHook32((BYTE*)fn, (BYTE*)func_hook, 5);
	}
	else {
		void* start = (void*)0x470E29;
		BYTE ori[] = {0x89, 0x45, 0x5C, 0x8B, 0xC6};
		DWORD prt;
		VirtualProtect(start, 5, PAGE_EXECUTE_READWRITE, &prt);
		memcpy_s(start, sizeof(ori), ori, sizeof(ori));
		VirtualProtect(start, 5, prt, &prt);
	}
}
