#include <Windows.h>
#include <iostream> 
#include "memory.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    uintptr_t localPlayer{};
    uintptr_t clientState{};
    uintptr_t entityList{};
    
    localPlayer = FindPattern("client.dll", "\x7d\x00\x8b\xc1\xc1\xe0", "x?xxxx", 0x9, 0x10);
    entityList = FindPattern("client.dll", "\x7f\x00\x81\xfe\x00\x00\x00\x00\x7d\x00\x8b\xc6", "x?xx????x?xx", 0x11, 0x10);
    clientState = FindPattern("engine.dll", "\xd9\x5d\x00\x8b\x0d\x00\x00\x00\x00\x8d\x49", "xx?xx????xx", 5, 0);
    
    while (!GetAsyncKeyState(VK_END))
    {
        
        //Print LocalEntity Health
        std::cout << std::dec << "LocalPlayer Health: " << *(int*)(FindDMAAddy(localPlayer, { 0x100 })) << std::hex << std::endl;

        //Print clientState
        std::cout << "ClientState" << clientState << std::endl;

        //Print ViewAngles
        std::cout << "ViewAngles X:" << *(float*)(FindDMAAddy(clientState, { 0x4D90 })) << std::endl;
        std::cout << "ViewAngles Y:" << *(float*)(FindDMAAddy(clientState, { 0x4D94 })) << std::endl;
       
        //Print EntityList Address
        std::cout << "EntityList: " << entityList << std::endl;
        
        Sleep(1);
        //Clear Console
        system("cls");
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

