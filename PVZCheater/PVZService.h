#pragma once
#include "pch.h"
#include "ProcessUtil.h"
#include "logger.h"
#include "dllParam.h"

class PVZService
{
public:
	char const* const exeFile = "PlantsVsZombies.exe";
	char const* const dllFile = "pvzdll.dll";
	char* MyDefineDLLPath = new char[256];
private:
	bool isGameRunning = false;
	bool isInjectedDLL = false;

	DWORD pid = 0;
	HANDLE pHandle = nullptr;
	LPRECT wndRect = nullptr;
	LPRECT cliRect = nullptr;
	const std::vector<DWORD> baseOffset = { 0x329670, 0x868 };
	const std::vector<DWORD> sunOffset = {0x329670, 0x868, 0x5578};
public:
	PVZService();
	~PVZService();
	inline bool IsGameRunning() { return isGameRunning; };
	inline DWORD GetPid() { return pid; };
	inline HANDLE GetHandle() { return pHandle; };
	inline LPRECT GetCliRect() { return cliRect; };
	LPRECT GetWndRect();

	int GetSunCount();
	void SetSunCount(int count);
	/*
	Sun Not Decrease
	*/
	void ToggleSunNotDecrease(bool flag);

	/*
	Auto Collect
	*/
	void ToggleAutoCollect(bool flag);

	/*
	Plant without CD, chopmer no CD
	*/
	void TogglePlantNoCD(bool flag);

	/*
	Plant casually
	*/
	void TogglePlantAnywhere(bool flag);

	/*
	No Pause
	*/
	void ToggleNoPause(bool flag);

	/*
	Zombie Freeze
	*/
	void ToggleZombieFreeze(bool flag);

	/*
	ToggleSeckillBullet
	*/
	void ToggleSeckillBullet(bool flag);

	void AddPlant(DWORD row, DWORD col, DWORD code);

	void AddZombie(DWORD row, DWORD code);

	bool isInBattle();

	void FreezeAllZombie();
	void KillAllZombie();

private:
	BOOL injectDLL();
};
