#include <Application/UApplication.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <SDL2/SDL.h>

#include <cstdio>

#include <Log/ULog.hpp> 
#include <Window/UWindow.hpp>

static bool running = true;

static void Render(UWindow *uWindow)
{
    ULog::Info("Render");

    SDL_SetRenderDrawColor(uWindow->SdlRenderer, 0, 0, 255, 255); // Blue background
    SDL_RenderClear(uWindow->SdlRenderer);

    SDL_SetRenderDrawColor(uWindow->SdlRenderer, 255, 255, 255, 255); // White rectangles

    SDL_Rect rect = { 10, 10, 100, 100 };

    SDL_RenderFillRect(uWindow->SdlRenderer, &rect);

    SDL_RenderPresent(uWindow->SdlRenderer);
}

static void EventLoop(void *window)
{
    SDL_Event event;

    UWindow *uWindow = (UWindow*)window;
    
    while (SDL_PollEvent(&event)) {

        switch (event.type)
        {
            case SDL_QUIT:
            {
                running = false;
            } break;

            case SDL_WINDOWEVENT:
            {
                #ifdef __EMSCRIPTEN__
 
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        ULog::Info("Window resized to %d x %d", event.window.data1, event.window.data2);
                        SDL_SetWindowSize(uWindow->SdlWindow, event.window.data1, event.window.data2);
                        SDL_RenderSetViewport(uWindow->SdlRenderer, nullptr);
                        Render(uWindow);
                    }
                #endif

            } break;

        }
    }
   
}

static int EventFilter(void *window, SDL_Event *event) 
{
    UWindow *uWindow = (UWindow*)window;


    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        printf("Window size changed to %d x %d\n", event->window.data1, event->window.data2);
        SDL_RenderSetViewport(uWindow->SdlRenderer, nullptr);
        Render(uWindow);
    }

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

    Render(window);

    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(EventLoop, window, -1, true);
    #else

        SDL_SetEventFilter(EventFilter, window);
        SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
    
        while (running) {
            EventLoop(window);
        }
    #endif



    delegate->ApplicationWillTerminate(this);

    delete window;
    SDL_Quit();

    return 0;
}
