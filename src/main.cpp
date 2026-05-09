#ifdef __PS2__
#include <stdio.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <sbv_patches.h>
#include "platform/ps2/PS2Platform.hpp"
static void ps2_init_stdio() {
    SifInitRpc(0);
    freopen("host:stdout.txt", "w", stdout);
    freopen("host:stderr.txt", "w", stderr);
    setvbuf(stdout, NULL, _IONBF, 0);
}
#endif
#include <SDL.h>

#include "AnmManager.hpp"
#include "Chain.hpp"
#include "Controller.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "GamePaths.hpp"
#include "GameWindow.hpp"
#include "MidiOutput.hpp"
#include "SoundPlayer.hpp"
#include "Stage.hpp"
#include "Supervisor.hpp"
#include "TextHelper.hpp"
#include "ZunResult.hpp"
#include "i18n.hpp"
#include "utils.hpp"
#include <iostream>
void dlog(std::string msg){
    std::cout<<msg<<std::endl;
}

#ifdef __PS2__
extern "C" int SDL_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    //dlog("Starting");
    i32 renderResult = 0;
#ifdef __PS2__
    extern void init_scr();
    extern int printf(const char*, ...);
    printf("SDL_main entered\n");
    printf("GamePaths::Init...\n");
#endif
#ifdef __ANDROID__
    // On Android, SDL must be initialized before GamePaths::Init()
    // because SDL_AndroidGetInternalStoragePath() requires SDL_Init.
    if (SDL_Init(0) < 0)
    {
        return 1;
    }
#endif

    //dlog("Init Gamepath");
    GamePaths::Init();

    // if (utils::CheckForRunningGameInstance())
    // {
    //     g_GameErrorContext.Flush();

    //     return 1;
    // }

    //dlog("Load CONF File");
#ifdef __PS2__
    printf("LoadConfig...\n");
#endif
    if (g_Supervisor.LoadConfig(TH_CONFIG_FILE) != ZUN_SUCCESS)
    {
#ifdef __PS2__
        printf("LoadConfig FAILED, continuing\n");
#else
        g_GameErrorContext.Flush();
        return -1;
#endif
    }

    // if (GameWindow::InitD3dInterface())
    // {
    //     g_GameErrorContext.Flush();
    //     return 1;
    // }
    //dlog("Start the game");

restart:
    //dlog("Create game window");
#ifdef __PS2__
    printf("CreateGameWindow...\n");
    // PS2Platform::Init();
#endif
    GameWindow::CreateGameWindow();

    //dlog("new AnmManager");
#ifdef __PS2__
    printf("new AnmManager...\n");
#endif
    g_AnmManager = new AnmManager();

    //dlog("InitD3dRendering");
#ifdef __PS2__
    printf("AnmManager done\n");
    printf("InitD3dRendering...\n");
#endif
    if (GameWindow::InitD3dRendering())
    {
        g_GameErrorContext.Flush();
        return 1;
    }

    //dlog("InitializeDSound");
#ifdef __PS2__
    printf("InitializeDSound...\n");
#endif
    g_SoundPlayer.InitializeDSound();
    //dlog("GetJoystickCaps");
#ifdef __PS2__
    printf("GetJoystickCaps...\n");
#endif
    Controller::GetJoystickCaps();
    //dlog("ResetKeyboard");
#ifdef __PS2__
    printf("ResetKeyboard...\n");
#endif
    Controller::ResetKeyboard();

    //dlog("Supervisor::RegisterChain");
#ifdef __PS2__
    printf("RegisterChain...\n");
#endif
    if (Supervisor::RegisterChain() != ZUN_SUCCESS)
    {
        goto stop;
    }
#ifdef __PS2__
    printf("RegisterChain done\n");
    printf("entering game loop\n");
#endif
    if (!g_Supervisor.cfg.windowed)
    {
        SDL_ShowCursor(SDL_DISABLE);
    }

    g_GameWindow.curFrame = 0;

    //dlog("Into loop game event");
#ifdef __PS2__
    printf("Into loop game event...\n");
#endif
    while (true)
    {
        SDL_Event e;

        //dlog("Into poolevent loop");
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                goto stop;
            }
        }

        //dlog("g_GameWindow.Render");
        renderResult = g_GameWindow.Render();
        if (renderResult != 0)
        {
            break;
        }

        //        SDL_Delay(1000.0f / 60.0f);

        //        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        //        {
        //            TranslateMessage(&msg);
        //            DispatchMessage(&msg);
        //        }
        //        else
        //        {
        //            testCoopLevelRes = g_Supervisor.d3dDevice->TestCooperativeLevel();
        //            if (testCoopLevelRes == D3D_OK)
        //            {
        //                renderResult = g_GameWindow.Render();
        //                if (renderResult != 0)
        //                {
        //                    goto stop;
        //                }
        //            }
        //            else if (testCoopLevelRes == D3DERR_DEVICENOTRESET)
        //            {
        //                g_AnmManager->ReleaseSurfaces();
        //                testResetRes = g_Supervisor.d3dDevice->Reset(&g_Supervisor.presentParameters);
        //                if (testResetRes != 0)
        //                {
        //                    goto stop;
        //                }
        //                GameWindow::InitD3dDevice();
        //                g_Supervisor.unk198 = 3;
        //            }
        //        }
    }


stop:
    //dlog("stop the game");
    g_Chain.Release();
    g_SoundPlayer.Release();

    delete g_AnmManager;
    g_AnmManager = NULL;

    // Clean up GL resources while the context is still valid.
    // THPrac::THPracGuiShutdown();
    // {
    //     SDL_GLContext ctx = g_Renderer ? g_Renderer->glContext : nullptr;
    //     if (g_Renderer)
    //         g_Renderer->Release();
    //     if (ctx)
    //         SDL_GL_DeleteContext(ctx);
    // }

    SDL_DestroyWindow(g_GameWindow.window);
    SDL_GL_DeleteContext(g_GameWindow.glContext);

    if (renderResult == 2)
    {
        // Clean up resources that leak across restart cycles.
        // We cannot call Supervisor::DeletedCallback() here because
        // ReleasePbg3() has a built-in double-free (calls Release() then
        // delete which calls Release() again) that crashes on modern heaps.
        // PBG3 archives are re-released internally by LoadPbg3() on reload,
        // so only these three resources actually leak:
        if (g_Supervisor.midiOutput != NULL)
        {
            g_Supervisor.midiOutput->StopPlayback();
            delete g_Supervisor.midiOutput;
            g_Supervisor.midiOutput = NULL;
        }
        TextHelper::ReleaseTextBuffer();
        // Controller::CloseSDLController();

        g_GameErrorContext.ResetContext();

        GameErrorContext::Log(&g_GameErrorContext, TH_ERR_OPTION_CHANGED_RESTART);

        if (!g_Supervisor.cfg.windowed)
        {
            SDL_ShowCursor(SDL_ENABLE);
        }
        goto restart;
    }

    FileSystem::WriteDataToFile(TH_CONFIG_FILE, &g_Supervisor.cfg, sizeof(g_Supervisor.cfg));

    SDL_ShowCursor(SDL_ENABLE);
    g_GameErrorContext.Flush();
    SDL_Quit();
    return 0;
}
