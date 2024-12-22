#include <windows.h>
#include <iostream>
#include <SDK.h>
#include <thread>
#include "Context.h"


int main()
{
    ctx::memory.Initialize("cs2.exe");
    ctx::renderer.Initialize();
    sdk::Initialize();

    /*
    * 
    * Update thread - every tick (update player data)
    * Render thread - (render everything)
    * 
    */

    std::thread updateThread(sdk::Update);

    // Main loop / render thread
    while (ctx::running)
    {
        if (GetAsyncKeyState(VK_END))
            ctx::running = false;

        ctx::renderer.Render();
        Sleep(1);
    }

    // Cleanup
    updateThread.detach();
    ctx::renderer.Destroy();
    //FreeLibraryAndExitThread(inst, 0);
}

/*BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        if (HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, 0); thread != nullptr)
            CloseHandle(thread);
    }

    return TRUE;
}*/