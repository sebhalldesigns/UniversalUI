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
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_sdlrenderer2.h>

static bool running = true;

float counter = 0;

static bool open = true;

static void Render(UWindow *uWindow)
{
    ULog::Info("Render");

    SDL_SetRenderDrawColor(uWindow->SdlRenderer, 0, 0, 0, 255); // Blue background
    SDL_RenderClear(uWindow->SdlRenderer);

    // Start the ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();

    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    static bool init = true;
    ImGuiID dock_id_left, dock_id_right;
    ImGuiID dock_id_top_right, dock_id_bottom_right;
    if (init) {
        init = false;
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

    	ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.75f, &dock_id_left, &dock_id_right);
        ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.5f, &dock_id_top_right, &dock_id_bottom_right);
        ImGui::DockBuilderDockWindow("Window_1", dock_id_left);
        ImGui::DockBuilderDockWindow("Window_2", dock_id_top_right);
        ImGui::DockBuilderDockWindow("Window_3", dock_id_bottom_right);

        ImGui::DockBuilderFinish(dockspace_id);
    }
    ImGui::Begin("Window_1");
    ImGui::Text("Text 1");
    ImGui::End();
    ImGui::Begin("Window_2");
    ImGui::Text("Text 2");
    ImGui::End();
        ImGui::Begin("Window_3");
    ImGui::Text("Text 3");
    ImGui::End();


    // ImGui::PopStyleVar(3);

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
