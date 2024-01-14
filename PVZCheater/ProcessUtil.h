#pragma once
#include "common.h"
#include "TlHelp32.h"
#include "filesystem"
//#include "typeinfo"

//https://github.com/Geedium/pvz-trainer/blob/master/PvZTrainer/debugger.h

typedef unsigned long long QWORD;

class ProcessUtil
{
public:
	/*
	get ProcessID By Process Title:
	Params:
	1. lpWindowName: The title of process window
	2. lpClassName:  The Class of process window(default NULL)
	Return: if not exists return 0, otherwise return real process ID.
	*/
	static DWORD GetProcessIDByTitle(LPCSTR lpWindowName, LPCSTR lpClassName = nullptr);

	/*
	Get process handle by the name of process.
	Return: if not exists return 0, else return real process ID.
	*/
	static DWORD GetProcessIDByExeFile(const char* processName);

	/*
	Get the module infomation in a process.
	Return a pair info of process, the 1st entry is base address and the second is the size of module.
	*/
	static std::pair<QWORD, QWORD> GetModule(DWORD processId, const char* moduleName);

	/*
	Read value from specific address of process handle
	*/
	template<typename T>
	static T Read(HANDLE pHandle, QWORD address) {
		T _read;
		ReadProcessMemory(pHandle, (LPVOID)address, &_read, sizeof(T), NULL);
		return _read;
	}

	template<typename T>
	static T* ReadBytes(HANDLE pHandle, QWORD address) {
		size_t len = sizeof(T);
		BYTE* _read = new BYTE[len];
		ReadProcessMemory(pHandle, (LPVOID)address, _read, len, NULL);
		return (T*)_read;
	}


	/*
	Read string value from specific address of process handle
	param:
	1. HANDLE pHandle: the process handle;
	2. address: the start address
	3. len: if len = 65535, then meeting the '\0' will stop; else read specific len of characters.
	*/
	static std::string ReadString(HANDLE pHandle, QWORD address, size_t len = 65535);

	/*
	Write value T to address in process handle
	*/
	template<typename T>
	static BOOL Write(HANDLE pHandle, QWORD address, T value) {
		return WriteProcessMemory(pHandle, (LPVOID)address, &value, sizeof(T), NULL);
	}

	/*
	Get the result address from multiple level pointers.
	*/
	template<typename T>
	static T ReadMultiLevelPointer(HANDLE pHandle, T base, std::vector<T> offsets);

	/*
	Return true if inject DLL successfully.
	*/
	static BOOL InjectDLL(HANDLE pHandle, const char *dllPath);

	/*
	Unload DLL.
	*/
	static void UnloadDLL(HANDLE pHandle, const char* moduleName);

	/*
	Virtual Alloc and write in another process
	*/
	static LPVOID AllocAndWrite(HANDLE h, void* data, DWORD size);

	/*
	This will wait the thread to end and free the memory allocated by `VirtualAllocEx`
	params:
	1. HANDLE threadHandle, the handle created by `CreateRemoteThread`
	2. std::vector<LPVOID> addrList, the address list of memories allocated by `VirtualAllocEx`
	3. async: true to create a new thread to breave resource.
	*/
	static void WaitToFree(HANDLE threadHandle, std::vector<LPVOID> addrList, bool async = true);

	/*
	Call the remote dll function and it will be free the allocated memory automatically.
	params:
	1. HANDLE h: remote process handle
	2. HMODULE dllModule: the HMODULE of dll file opened by LoadLibrary.
	3. const char* funcName: the export function in the dllModule
	4. addrList: the parameters allocate by `VirtualAllocEx` function, the length MUST be greate than 0, the first item is the function param, the others is used to free in latter progress.
	5. async: set false if you want to wait the thread exit.
	return: the boolean result of execution.
	*/
	static bool RemoteCallDllFunc(const HANDLE h, const HMODULE dllModule, const char* funcName, std::vector<LPVOID> addrList, bool async = true);
};

template<typename T>
inline T ProcessUtil::ReadMultiLevelPointer(HANDLE pHandle, T base, std::vector<T> offsets)
{
	int len = offsets.size();
	for (int i = 0; i < len; i++) {
		if (i < len - 1) base = Read<T>(pHandle, base + offsets[i]);
		else base = base + offsets[i];
	}
	return base;
}
