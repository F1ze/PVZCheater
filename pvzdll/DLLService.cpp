#include "pch.h"
#include "DLLService.h"

EXPORT void AddPlant(AddPlantParam* param)
{
	if (param->row > 4 || param->col > 8 || param->plantCode > 52) return;

	DWORD row = param->row;
	DWORD col = param->col;
	DWORD plantCode = param->plantCode;

	HMODULE hMod = GetModuleHandle(NULL);

	DWORD base = (DWORD)hMod + 0x329670;

	void* func = (void*)0x40FA10;

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

	HMODULE hMod = GetModuleHandle(NULL);
	DWORD base = (DWORD)hMod + 0x329670;

	void* func = (void*)0x420670;
	SHORT a = 0x9090;
	WriteProcessMemory(hMod, (LPVOID)0x4206F3, &a, 2, NULL);

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
	a = 0x3d74;
	WriteProcessMemory(hMod, (LPVOID)0x4206F3, &a, 2, NULL);
}
