#ifdef __PS2__
#include <sifrpc.h>
#include <debug.h>
#include <stdio.h>
#include <sio.h>

extern "C" int SDL_main(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    sio_init(115200, 0, 0, 0, 0);
    init_scr();
    sio_printf("Hello from PCSX2!\\n");
    scr_printf("PS2 main() reached\n");
    SifInitRpc(0);
    scr_printf("SifInitRpc done\n");
    scr_printf("calling SDL_main...\n");
    int ret = SDL_main(argc, argv);
    scr_printf("SDL_main returned %d\n", ret);
    return ret;
}
#endif