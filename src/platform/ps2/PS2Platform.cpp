#ifdef __PS2__
#include "PS2Platform.hpp"
#include <kernel.h>
#include <timer.h>
#include <stdio.h>
#include <GL/ps2gl.h>
#include <GL/glut.h>
#include <gsKit.h>

__attribute__((constructor(101))) static void ps2_early_stdio_init() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    freopen("host:stdout.txt", "w", stdout);
    freopen("host:stderr.txt", "w", stderr);
}

GSGLOBAL *PS2Platform::gsGlobal = nullptr;

// GS VRAM layout (4MB):
// slot0: pages   0-31  → frame0  (640x448 CT32)
// slot1: pages  32-63  → frame1  (640x448 CT32)
// slotZ: pages  64-95  → depth   (640x448 Z16S)
// texs:  pages  96-191 → textures
#define FRAME_PAGES 32
#define DEPTH_PAGES 32
#define TEX_PAGES   96

static pgl_area_handle_t s_frame0, s_frame1, s_depth;

void PS2Platform::Init() {
    printf("glutInit...\n");
    int argc = 0;
    glutInit(&argc, nullptr);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 448);
    glutCreateWindow("TH06");

    // add texture slots from remaining GS VRAM (pages 210+ based on glutInit allocation output)
    // glutInit uses pages 0-209 for framebuffers; rest is free for textures
    pglAddGsMemSlot(212, 1,  GS_PSM_CT32);  // 1-page slots for small textures
    pglAddGsMemSlot(213, 1,  GS_PSM_CT32);
    pglAddGsMemSlot(214, 2,  GS_PSM_CT32);
    pglAddGsMemSlot(216, 2,  GS_PSM_CT32);
    pglAddGsMemSlot(218, 2,  GS_PSM_CT32);
    pglAddGsMemSlot(220, 2,  GS_PSM_CT32);
    pglAddGsMemSlot(222, 8,  GS_PSM_CT32);  // 8-page slots for medium textures
    pglAddGsMemSlot(230, 8,  GS_PSM_CT32);
    pglAddGsMemSlot(238, 32, GS_PSM_CT32);  // 32-page slots for large textures
    pglAddGsMemSlot(270, 32, GS_PSM_CT32);
    pglAddGsMemSlot(302, 64, GS_PSM_CT32);  // 64-page slots for very large textures
    pglAddGsMemSlot(366, 64, GS_PSM_CT32);
    pglAddGsMemSlot(210, 2,  GS_PSM_T8);    // 8-bit texture slots

    printf("glutInit done\n");
}

void PS2Platform::Shutdown() {
    pglFinish();
}

void PS2Platform::SwapBuffers() {
    printf("pglSwapBuffers...\n");
    pglSwapBuffers();
    printf("pglSwapBuffers done\n");
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