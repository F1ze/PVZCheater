// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "DLLService.h"

static DLLStatus status;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    static std::atomic<bool> isInitMH = false;
    
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        bool expect = false;
        if (isInitMH.compare_exchange_strong(expect, true)) {
            MH_Initialize();
        }
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    {
        bool expect = true;
        if (isInitMH.compare_exchange_strong(expect, false)) {
            MH_Uninitialize();
        }
        break;
    }
    }
    return TRUE;
}