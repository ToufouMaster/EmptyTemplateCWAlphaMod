// dllmain.cpp : Defines the entry point for the application.

#include "dllmain.h"

// This function is called whenever the mod is loaded
DWORD WINAPI MainLoop() {
    cube::InitAPI();

    return 0;
}

// This function is the entry function called whenever the dll is loaded
extern "C" BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Call the MainLoop Function on an other Thread.
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainLoop, 0, 0, NULL);
        break;
    }
    return TRUE;
}