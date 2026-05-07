#pragma once
#ifdef __PS2__

#include <gsKit.h>
#include <gsToolkit.h>

struct PS2Platform {
    static GSGLOBAL *gsGlobal;

    static void Init();
    static void Shutdown();
    static void SwapBuffers();
    static u32  GetTicks();
    static void Delay(u32 ms);
};

#endif // __PS2__