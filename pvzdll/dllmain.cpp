// dllmain.cpp : Defines the entry point for the DLL application.
#include "common.h"
#include "DLLService.h"
#include "logger.h"

static DLLStatus* status = new DLLStatus;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{

    logger::init();

    static std::atomic<bool> isInitMH = false;

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        delete status;
    }
    return TRUE;
}