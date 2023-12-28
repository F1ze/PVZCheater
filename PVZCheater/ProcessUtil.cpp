#include "ProcessUtil.h"

DWORD ProcessUtil::GetProcessIDByTitle(LPCSTR lpWindowName, LPCSTR lpClassName)
{
	HWND w = FindWindowA(lpClassName, lpWindowName);
	if (w == nullptr) return 0;
	else {
		DWORD processId = 0;
		GetWindowThreadProcessId(w, &processId);
		return processId;
	}
}

DWORD ProcessUtil::GetProcessIDByExeFile(const char* processName)
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);
	while (Process32Next(handle, &entry)) {
		if (!strcmp(entry.szExeFile, processName)) {
			CloseHandle(handle);
			return entry.th32ProcessID;
		}
	}
	return 0;
}

std::pair<QWORD, QWORD> ProcessUtil::GetModule(DWORD processId, const char* moduleName)
{
	// If game is 32-bit program, so the cheater must be 32-bit.
	HANDLE mod = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	while (Module32Next(mod, &mEntry)) {
		if (!strcmp(mEntry.szModule, moduleName)) 
			return { (QWORD)mEntry.hModule, mEntry.modBaseSize };
		//getLogger()->AddLog("Handle:%llx, ModuleName: %s(%x), exeFile:%s\n", (QWORD)mEntry.hModule, mEntry.szModule, mEntry.modBaseAddr, mEntry.szExePath);
	} 
	return { 0, 0 };
}

std::string ProcessUtil::ReadString(HANDLE pHandle, QWORD address, size_t len)
{
	std::string s;
	for (size_t i = 0; i < len; i++)
	{
		char c;
		ReadProcessMemory(pHandle, (LPVOID)address, &c, sizeof(char), NULL);
		s.push_back(c);
		if (c == 0 && len == 65535) break;
		address = (QWORD)((BYTE*)address) + 1;
	}
	return s;
}

BOOL ProcessUtil::InjectDLL(HANDLE pHandle, const char* dllPath)
{
	if (std::filesystem::exists(dllPath) && pHandle != nullptr) {
		LPVOID pAddr = VirtualAllocEx(pHandle, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
		if (pAddr == nullptr) return FALSE;
		WriteProcessMemory(pHandle, pAddr, dllPath, strlen(dllPath) + 1, NULL);
		HMODULE hModule = LoadLibrary("KERNEL32.DLL");
		if (hModule == nullptr) return FALSE;
		LPTHREAD_START_ROUTINE func = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryA");
		if (func == nullptr) return FALSE;
		return CreateRemoteThread(pHandle, NULL, 0, func, pAddr, 0, NULL) != nullptr;
	}
	return FALSE;
}

void ProcessUtil::UnloadDLL(HANDLE pHandle, const char* moduleName)
{
	DWORD pid = GetProcessId(pHandle);
	LPVOID paramAddr = (LPVOID)GetModule(pid, moduleName).first;
	if (paramAddr != nullptr) {
		HMODULE hModule = LoadLibrary("KERNEL32.DLL");
		LPTHREAD_START_ROUTINE func = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "FreeLibrary");
		CreateRemoteThread(pHandle, NULL, 0, func, paramAddr, 0, NULL);
	}
}

LPVOID ProcessUtil::AllocAndWrite(HANDLE h, void* data, DWORD size)
{
	LPVOID dataAddr = VirtualAllocEx(h, NULL, size, MEM_COMMIT, PAGE_READWRITE);
	if (dataAddr == nullptr) return nullptr;
	if (!WriteProcessMemory(h, dataAddr, data, size, NULL)) return nullptr;
	return dataAddr;
}

void ProcessUtil::WaitToFree(HANDLE threadHandle, std::vector<LPVOID> addrList, bool async)
{
	auto func = [threadHandle, addrList] {
		WaitForSingleObject(threadHandle, INFINITE);
		// https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualfreeex
		for (auto item : addrList)
			VirtualFreeEx(threadHandle, item, 0, MEM_RELEASE);
		CloseHandle(threadHandle);
	};
	
	if (async) {
		std::thread t(func);
		t.detach();
	}
	else func();
}

bool ProcessUtil::RemoteCallDllFunc(const HANDLE h, const HMODULE dllModule, const char* funcName, std::vector<LPVOID> addrList, bool async)
{
	if (h == nullptr || dllModule == nullptr || addrList.empty()) return false;
	auto funcAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(dllModule, funcName);
	if (!funcAddr) return false;
	HANDLE threadHandle = CreateRemoteThread(h, nullptr, 0, funcAddr, addrList[0], 0, NULL);
	if (!threadHandle) return false;
	WaitToFree(threadHandle, addrList, async);
	return true;
}
