#include <windows.h>
#include <iostream>
#include <SDK.h>
#include <thread>
#include "Context.h"
#include <Gui.h>

/*
* Handle keybinds
* TODO: add keybinding
*/
void KeyHandling()
{
    while (ctx::running)
    {
        if (GetAsyncKeyState(VK_END))
            ctx::running = false;

        if (GetAsyncKeyState(VK_INSERT) & 0x0001)
            gui::open = !gui::open;

        Sleep(100);
    }
}

int main()
{
    // Set to highest priority.
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

    ctx::memory.Initialize("cs2.exe");
    ctx::renderer.Initialize();
    sdk::Initialize();

    /*
    * 
    * Update thread         - every tick (update player data)
    * Render thread         - render everything
    * KeyHandling thread    - check for keypresses
    * 
    */

    std::thread updateThread(sdk::Update);
    std::thread keyHandlingThread(KeyHandling);

    // Main loop / render thread
    while (ctx::running)
    {  
        ctx::renderer.Render();
        Sleep(1);
    }

    // Cleanup
    updateThread.detach();
    keyHandlingThread.detach();
    ctx::renderer.Destroy();
}