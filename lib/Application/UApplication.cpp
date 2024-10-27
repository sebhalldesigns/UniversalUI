#include <Application/UApplication.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <SDL2/SDL.h>

#include <cstdio>

#include <Log/ULog.hpp> 
#include <Window/UWindow.hpp>

static int running = 0;

static void EventLoop(void *window)
{
    SDL_Event event;

    UWindow *uWindow = (UWindow*)window;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            ULog::Info("Window resized to %d x %d", event.window.data1, event.window.data2);
            
            SDL_SetWindowSize(uWindow->SdlWindow, event.window.data1, event.window.data2);
            SDL_RenderSetViewport(uWindow->SdlRenderer, nullptr);

            SDL_SetRenderDrawColor(uWindow->SdlRenderer, 0, 0, 255, 255); // Blue background
            SDL_RenderClear(uWindow->SdlRenderer);

            SDL_SetRenderDrawColor(uWindow->SdlRenderer, 255, 255, 255, 255); // White rectangles

            SDL_Rect rect = { 10, 10, 100, 100 };

            SDL_RenderFillRect(uWindow->SdlRenderer, &rect);

            SDL_RenderPresent(uWindow->SdlRenderer);

        }
    }
   
}

static int EventFilter(void *app, SDL_Event *event) 
{
    //printf("Event %d\n", event->type);
    return 1;
}




int UApplication::Run(UApplicationDelegate *delegate)
{

    ULog::Info("UApplication starting...");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ULog::Error("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }
    

    UWindow* window = new UWindow();

    if (!window->SdlWindow || !window->SdlRenderer) {
        ULog::Error("Failed to create window or renderer: %s", SDL_GetError());
        delete window;
        SDL_Quit();
        return -1;
    }

    delegate->ApplicationDidFinishLaunching(this, window);


    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(EventLoop, window, -1, true);
    #else

        SDL_SetEventFilter(EventFilter, this);
        SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
    
        while (running) {
            EventLoop(this);
        }
    #endif



    delegate->ApplicationWillTerminate(this);

    delete window;
    SDL_Quit();

    return 0;
}
