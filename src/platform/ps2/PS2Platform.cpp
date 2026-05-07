#ifdef __PS2__
#include "PS2Platform.hpp"
#include <kernel.h>
#include <timer.h>
#include <stdio.h>

// Jalan sebelum main() dan SDL2main
__attribute__((constructor(101))) static void ps2_early_stdio_init() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    freopen("host:stdout.txt", "w", stdout);
    freopen("host:stderr.txt", "w", stderr);
}

GSGLOBAL *PS2Platform::gsGlobal = nullptr;

void PS2Platform::Init() {
    gsGlobal = gsKit_init_global();
    gsGlobal->Mode       = GS_MODE_NTSC;
    gsGlobal->Width      = 640;
    gsGlobal->Height     = 448;
    gsGlobal->PSM        = GS_PSM_CT32;
    gsGlobal->PSMZ       = GS_PSMZ_16S;
    gsGlobal->ZBuffering = GS_SETTING_ON;
    gsGlobal->Interlace  = GS_INTERLACED;
    gsGlobal->Field      = GS_FIELD;
    gsKit_init_screen(gsGlobal);
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
}

void PS2Platform::Shutdown() {
    if (gsGlobal) gsKit_deinit_global(gsGlobal);
}

void PS2Platform::SwapBuffers() {
    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);
}

static u64 s_tickBase = 0;

u32 PS2Platform::GetTicks() {
    if (!s_tickBase) s_tickBase = GetTimerSystemTime();
    return (u32)((GetTimerSystemTime() - s_tickBase) / 147456);
}

void PS2Platform::Delay(u32 ms) {
    u32 start = GetTicks();
    while (GetTicks() - start < ms);
}

#endif // __PS2__
