#include <Application/UApplication.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <SDL2/SDL.h>

#include <cstdio>

#include <Log/ULog.hpp> 
#include <Window/UWindow.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_sdlrenderer2.h>

static bool running = true;

float counter = 0;

static bool open = true;

static void Render(UWindow *uWindow)
{
    ULog::Info("Render");

    SDL_SetRenderDrawColor(uWindow->SdlRenderer, 0, 0, 255, 255); // Blue background
    SDL_RenderClear(uWindow->SdlRenderer);

    SDL_SetRenderDrawColor(uWindow->SdlRenderer, 255, 255, 255, 255); // White rectangles

    SDL_Rect rect = { 10, 10, 100, 100 };

    SDL_RenderFillRect(uWindow->SdlRenderer, &rect);

    // Start the ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();

    static bool use_work_area = false;
    static ImGuiWindowFlags window_flags = 
        ImGuiWindowFlags_AlwaysAutoResize |
        //ImGuiWindowFlags_DockNodeHost |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoMouseInputs |
        ImGuiWindowFlags_NoDecoration  | 
        ImGuiWindowFlags_NoBackground  | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoSavedSettings;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Example: Fullscreen window", &open, window_flags);
    
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id);

        ImGui::Begin("Project Explorer");
        ImGui::Text("File 1");
        ImGui::Text("File 2");
        ImGui::End();
    
    ImGui::End();

    ImGui::PopStyleVar(3);

    // Rendering
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), uWindow->SdlRenderer);

    SDL_RenderPresent(uWindow->SdlRenderer);
}

static void EventLoop(void *window)
{
    SDL_Event event;

    UWindow *uWindow = (UWindow*)window;


    
    while (SDL_PollEvent(&event)) {

        
     if (ImGui_ImplSDL2_ProcessEvent(&event)) {
        //return 0; // Event handled by ImGui
        Render(uWindow);
    }

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

    /*if () {
        //return 0; // Event handled by ImGui
        Render(uWindow);
    }*/


    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        printf("Window size changed to %d x %d\n", event->window.data1, event->window.data2);
        SDL_RenderSetViewport(uWindow->SdlRenderer, nullptr);
        ImGui_ImplSDL2_ProcessEvent(event);
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

     // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
ImGuiStyle& style = ImGui::GetStyle();
style.WindowRounding = 5.0f;
style.FrameRounding = 4.0f;
// Customize colors as needed, e.g.:
style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

    ImGui_ImplSDL2_InitForSDLRenderer(window->SdlWindow, window->SdlRenderer);
    ImGui_ImplSDLRenderer2_Init(window->SdlRenderer);

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

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    delegate->ApplicationWillTerminate(this);

    delete window;
    SDL_Quit();

    return 0;
}
