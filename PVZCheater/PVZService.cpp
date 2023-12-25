#include "PVZService.h"

PVZService::PVZService()
{
	wndRect = new RECT;
	cliRect = new RECT;

	// Get DLL path
	GetModuleFileNameA(NULL, MyDefineDLLPath, 256);
	auto path = std::string(MyDefineDLLPath);
	auto idx = path.find_last_of("\\");
	path = path.substr(0, idx + 1) + std::string(dllFile);
	strcpy(MyDefineDLLPath, path.c_str());

	// new thread
	auto fn = [this]() {
		while (true) {
			auto pid = ProcessUtil::GetProcessIDByExeFile(this->exeFile);
			if (pid == 0) {
				this->isGameRunning = false;
				this->pid = 0;
				this->pHandle = nullptr;
				this->isInjectedDLL = false;
			}
			else if (this->pid != pid) {
				CloseHandle(this->GetHandle());
				this->pid = pid;
				this->pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->pid);
				this->isGameRunning = true;
				this->injectDLL();
			}
			else {
				// 正常运作的常规定时操作
				// get window rect
				if (!this->isInjectedDLL) this->injectDLL();
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		};
	std::thread t(fn);
	t.detach();
}

PVZService::~PVZService()
{
	if (this->isInjectedDLL) ProcessUtil::UnloadDLL(this->pHandle, this->dllFile);
	CloseHandle(this->GetHandle());
}

LPRECT PVZService::GetWndRect()
{
	auto hwnd = FindWindowA("MainWindow", "Plants vs. Zombies");
	if (hwnd) {
		GetWindowRect(hwnd, this->wndRect);
		GetClientRect(hwnd, this->cliRect);
		this->wndRect->top += (this->wndRect->bottom - this->wndRect->top) - (this->cliRect->bottom - this->cliRect->top);
		return this->wndRect;
	}
	else return nullptr;
}

int PVZService::GetSunCount()
{
	auto base = (DWORD)ProcessUtil::GetModule(this->pid, this->exeFile).first;
	auto addr = ProcessUtil::ReadMultiLevelPointer<DWORD>(this->pHandle, base, sunOffset);
	return ProcessUtil::Read<int>(this->pHandle, addr);
}

void PVZService::SetSunCount(int count)
{
	auto base = (DWORD)ProcessUtil::GetModule(this->pid, this->exeFile).first;
	auto addr = ProcessUtil::ReadMultiLevelPointer<DWORD>(this->pHandle, base, sunOffset);
	ProcessUtil::Write(this->pHandle, addr, count);
}

int PVZService::GetSlotCount()
{
	if (isInBattle()) {
		auto base = (DWORD)ProcessUtil::GetModule(this->pid, this->exeFile).first;
		auto addr = ProcessUtil::ReadMultiLevelPointer<DWORD>(this->pHandle, base, slotCountOffset);
		return ProcessUtil::Read<DWORD>(this->pHandle, addr);
	}
	return 0;
}

int PVZService::GetSlotCodeByIdx(int idx)
{
	auto base = (DWORD)ProcessUtil::GetModule(this->pid, this->exeFile).first;
	auto addr = ProcessUtil::ReadMultiLevelPointer<DWORD>(this->pHandle, base, slotOffset);
	addr += idx * 0x50;
	return ProcessUtil::Read<DWORD>(this->pHandle, addr);
}

void PVZService::SetSlotCodeByIdx(int idx, int code)
{
	auto base = (DWORD)ProcessUtil::GetModule(this->pid, this->exeFile).first;
	auto addr = ProcessUtil::ReadMultiLevelPointer<DWORD>(this->pHandle, base, slotOffset);
	addr += idx * 0x50;
	ProcessUtil::Write<DWORD>(this->pHandle, addr, code);
}

void PVZService::ToggleSunNotDecrease(bool flag)
{
	if (flag) ProcessUtil::Write<WORD>(this->pHandle, 0x41E844, 0x9090);
	else ProcessUtil::Write<WORD>(this->pHandle, 0x41E844, 0xF32B);
}

void PVZService::ToggleAutoCollect(bool flag)
{
	//004342F2
	if (flag) ProcessUtil::Write<BYTE>(this->pHandle, 0x4342F2, 0xEB);
	else ProcessUtil::Write<BYTE>(this->pHandle, 0x4342F2, 0x75);
}

void PVZService::ToggleCardNoCD(bool flag)
{

	if (flag)
	{
		ProcessUtil::Write<WORD>(this->pHandle, 0x491E55, 0x9090);
		
		// normal purple
		//0040FDF4
		//ProcessUtil::Write<WORD>(this->pHandle, 0x40FDF4, 0x9090);
		ProcessUtil::Write<WORD>(this->pHandle, 0x40FDFD, 0x9090);
		// corn galon
		// 0040FFC7
		//ProcessUtil::Write<WORD>(this->pHandle, 0x40FFC7, 0x9090);
		ProcessUtil::Write<WORD>(this->pHandle, 0x40FFD8, 0x9090);
		ProcessUtil::Write<BYTE>(this->pHandle, 0x40FFEB, 0xEB);
		// cat grass
		// 0040FE3D
		//ProcessUtil::Write<QWORD>(this->pHandle, 0x40FE3D, 0x448B909090909090);
		ProcessUtil::Write<WORD>(this->pHandle, 0x40FE4D, 0x6FEB);

		// purple card
		// 1. 0040FDFD 90 90 / 75 01
		// 2. 0040FFD8 90 90 / 75 13		0040FFEB EB / 75
		// 3. 0040FE4D EB 6F / 75 70
	}
	else
	{
		ProcessUtil::Write<WORD>(this->pHandle, 0x491E55, 0x147E);

		//ProcessUtil::Write<WORD>(this->pHandle, 0x40FDF4, 0x1774);
		ProcessUtil::Write<WORD>(this->pHandle, 0x40FDFD, 0x0175);
		// corn galon
		//ProcessUtil::Write<WORD>(this->pHandle, 0x40FFC7, 0x3374);
		ProcessUtil::Write<WORD>(this->pHandle, 0x40FFD8, 0x1375);
		ProcessUtil::Write<BYTE>(this->pHandle, 0x40FFEB, 0x75);

		//ProcessUtil::Write<QWORD>(this->pHandle, 0x40FE3D, 0x448B0000008D840F);
		ProcessUtil::Write<WORD>(this->pHandle, 0x40FE4D, 0x7075);
	}
}

void PVZService::TogglePlantAnywhere(bool flag)
{
	// 004127EF
	if (flag) ProcessUtil::Write<DWORD>(this->pHandle, 0x4127EF, 0x000921E9);
	else ProcessUtil::Write<DWORD>(this->pHandle, 0x4127EF, 0x0920840F);
}

void PVZService::ToggleNoPause(bool flag)
{
	// 0045272A
	if (flag) ProcessUtil::Write<BYTE>(this->pHandle, 0x45272A, 0xEB);
	else ProcessUtil::Write<BYTE>(this->pHandle, 0x45272A, 0x75);
}

void PVZService::ToggleZombieFreeze(bool flag)
{
	// 0053B432
	if (flag) ProcessUtil::Write<DWORD>(this->pHandle, 0x53B432, 0x90909090);
	else ProcessUtil::Write<DWORD>(this->pHandle, 0x53B432, 0x082464D8);
}

void PVZService::ToggleSeckillBullet(bool flag)
{
	
	if (IsGameRunning()) {
		BYTE ori[] = { 0x8B, 0x6C, 0x24, 0x0C, 0x56 };
		BYTE hop[] = { 0xe9, 0x00, 0x00, 0x00, 0x00 };
		BYTE hook[] = {
			0xBD, 0xFF, 0xFF, 0x00, 0x00,	// mov ebp,0000FFFF
			0x56,							// push esi
			0xE9, 0x00, 0x00, 0x00, 0x00	// jmp PlantsVsZombies.exe+142187
		};

		DWORD startAddr = 0x542182;
		DWORD retnAddr = 0x542187;

		if (flag) {
			auto addr = VirtualAllocEx(this->pHandle, NULL, sizeof(hook), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			// hop
			auto hopOffset = (DWORD)addr - startAddr - 5;
			*(DWORD*)(hop + 1) = hopOffset;
			WriteProcessMemory(this->pHandle, (LPVOID)startAddr, hop, sizeof(hop), NULL);
			// hook padding
			auto retnOffset = retnAddr - (DWORD)addr - 11;
			*(DWORD*)(hook + 7) = retnOffset;
			WriteProcessMemory(this->pHandle, addr, hook, sizeof(hook), NULL);
		}
		else {
			WriteProcessMemory(this->pHandle, (LPVOID)startAddr, ori, sizeof(ori), NULL);
		}
	}
}

void PVZService::TogglePlantNoCD(bool flag)
{
	BYTE arr[] = {
	0x0F, 0x8E, 0, 0, 0, 0,				// jng 00466C6C
	0x83, 0xF8, 0x20,					// cmp eax, 20
	0x0F, 0x8E, 0x05, 0x00, 0x00, 0x00, // jng 5
	0xB8, 0x20, 0x00, 0x00, 0x00,		// mov eax,20
	0x48,								// dec eax
	0x89, 0x47, 0x54,					// mov [edi+54],eax
	0xE9, 0, 0, 0, 0					// jmp 00466C6C
	};

	BYTE original[] = {
		0x7E, 0x04, // jle 00466C6C
		0x48,		// dec eax
		0x89, 0x47, 0x54,					// mov [edi+54],eax
	};

	BYTE hop[] = {
		0xE9, 0x0, 0, 0, 0, // jmp xxx
		0x90				// nop
	};

	DWORD startAddr = 0x466C66;
	DWORD retnAddr = 0x466C6C;

	if (flag)
	{
		//ProcessUtil::Write<WORD>(this->pHandle, 0x491E55, 0x9090);

		// purple card
		// 1. 0040FDFD 90 90 / 75 01
		// 2. 0040FFD8 90 90 / 75 13		0040FFEB EB / 75
		// 3. 0040FE4D EB 6F / 75 70

		// Chomper CD, potato CD
		auto addr = VirtualAllocEx(this->pHandle, NULL, sizeof(arr), MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		DWORD retnOffset = retnAddr - (DWORD)addr - 6;
		*(DWORD*)(arr + 2) = retnOffset;

		retnOffset = retnAddr - (DWORD)addr - sizeof(arr);
		*(DWORD*)(arr + 25) = retnOffset;

		WriteProcessMemory(this->pHandle, addr, arr, sizeof(arr), NULL);

		DWORD jmpOffset = (DWORD)addr - startAddr - 5;
		*(DWORD*)(hop + 1) = jmpOffset;
		WriteProcessMemory(this->pHandle, (LPVOID)startAddr, hop, sizeof(hop), NULL);
	}
	else
	{
		//ProcessUtil::Write<WORD>(this->pHandle, 0x491E55, 0x147E);
		WriteProcessMemory(this->pHandle, (LPVOID)startAddr, original, sizeof(original), NULL);
	}
}

void PVZService::TogglePlantNoSleep(bool flag)
{
	// 0046219F
	if (flag) ProcessUtil::Write<QWORD>(this->pHandle, 0x46219F, 0x4F8B9000000292E9);
	else ProcessUtil::Write<QWORD>(this->pHandle, 0x46219F, 0x4F8B00000291840F);
}

void PVZService::TogglePlantRandomBullet(bool flag)
{
	auto paddr = ProcessUtil::AllocAndWrite(this->pHandle, &flag, sizeof(bool));
	HMODULE h = LoadLibrary(MyDefineDLLPath);
	auto funcAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(h, "RandomBullet");
	HANDLE threadHandle = CreateRemoteThread(this->pHandle, nullptr, 0, funcAddr, paddr, 0, NULL);
}

void PVZService::AddPlant(DWORD row, DWORD col, DWORD code)
{
	if (this->IsGameRunning() && this->isInjectedDLL && isInBattle()) {
		AddPlantParam* param = new AddPlantParam(row, col, code);
		auto paddr = ProcessUtil::AllocAndWrite(this->pHandle, param, sizeof(AddPlantParam));
		HMODULE h = LoadLibrary(MyDefineDLLPath);
		auto funcAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(h, "AddPlant");
		HANDLE threadHandle = CreateRemoteThread(this->pHandle, nullptr, 0, funcAddr, paddr, 0, NULL);
		// Release memory
		ProcessUtil::WaitToFree(threadHandle, { paddr });
	}
}

void PVZService::AddZombie(DWORD row, DWORD code)
{
	if (this->IsGameRunning() && this->isInjectedDLL && isInBattle() && row < 5) {
		AddZombieParam* param = new AddZombieParam(row, code);
		auto paddr = ProcessUtil::AllocAndWrite(this->pHandle, param, sizeof(AddZombieParam));
		HMODULE h = LoadLibrary(MyDefineDLLPath);
		auto funcAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(h, "AddZombie");
		HANDLE threadHandle = CreateRemoteThread(this->pHandle, nullptr, 0, funcAddr, paddr, 0, NULL);
		// Release memory
		ProcessUtil::WaitToFree(threadHandle, { paddr });
	}
}

bool PVZService::isInBattle()
{
	if (IsGameRunning()) {
		auto base = (DWORD)ProcessUtil::GetModule(this->pid, this->exeFile).first;
		auto addr = ProcessUtil::ReadMultiLevelPointer<DWORD>(this->pHandle, base, baseOffset);
		auto value = ProcessUtil::Read<DWORD>(this->pHandle, addr);
		return value != 0;
	}
	return false;
}

void PVZService::FreezeAllZombie()
{
	if (this->IsGameRunning() && this->isInjectedDLL && isInBattle()) {
		HMODULE h = LoadLibrary(MyDefineDLLPath);
		auto funcAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(h, "FreezeAllZombie");
		HANDLE threadHandle = CreateRemoteThread(this->pHandle, nullptr, 0, funcAddr, nullptr, 0, NULL);
	}
}

void PVZService::KillAllZombie()
{
	if (this->IsGameRunning() && this->isInjectedDLL && isInBattle()) {
		HMODULE h = LoadLibrary(MyDefineDLLPath);
		auto funcAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(h, "KillAllZombie");
		HANDLE threadHandle = CreateRemoteThread(this->pHandle, nullptr, 0, funcAddr, nullptr, 0, NULL);
	}
}

void PVZService::BlowAllZombie()
{
	auto arr = this->EnumerateZombie();
	for (auto item : arr) {
		ProcessUtil::Write<BYTE>(this->pHandle, item + 0xb9, 1);
	}
}

std::vector<DWORD> PVZService::EnumerateZombie()
{
	DWORD startAddress = 0;
	std::vector<DWORD> arr;
	if (isInBattle()) {
		while (true) {
			startAddress = GetNextZombie(startAddress);
			if (startAddress == 0) break;
			arr.push_back(startAddress);
			BOOST_LOG_TRIVIAL(info) << "Zombie: " << startAddress;
		}
	}
	return arr;
}

DWORD PVZService::GetNextZombie(DWORD startAddress)
{
	if (this->isInjectedDLL && isInBattle()) {

		GetNextZombieParam* param = new GetNextZombieParam(0, startAddress);
		auto paddr = ProcessUtil::AllocAndWrite(this->pHandle, param, sizeof(GetNextZombieParam));

		HMODULE h = LoadLibrary(MyDefineDLLPath);
		auto funcAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(h, "GetNextZombie");
		HANDLE threadHandle = CreateRemoteThread(this->pHandle, nullptr, 0, funcAddr, paddr, 0, NULL);
		WaitForSingleObject(threadHandle, INFINITE);
		auto ret = ProcessUtil::Read<GetNextZombieParam>(this->pHandle, (QWORD)paddr);
		if (ret.retValue == 0) return 0;
		else return ret.startAddress;
	}
	else return 0;
}

BOOL PVZService::injectDLL()
{
	if (IsGameRunning()) {
		this->isInjectedDLL = ProcessUtil::InjectDLL(this->pHandle, MyDefineDLLPath);
		if (!this->isInjectedDLL) logger::getLastError();
		return this->isInjectedDLL;
	}
	else return FALSE;
}
