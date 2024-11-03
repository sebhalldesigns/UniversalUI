#include <Application/UApplication.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <SDL2/SDL.h>

#include <cstdio>
#include <random>

#include <Log/ULog.hpp> 
#include <Window/UWindow.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_sdlrenderer2.h>

static bool running = true;

float counter = 0;

static bool open = true;

ImFont* customFont = nullptr;

static void Render(UWindow *uWindow)
{
    ULog::Info("Render");

    SDL_SetRenderDrawColor(uWindow->SdlRenderer, 0, 0, 0, 255); // Blue background
    SDL_RenderClear(uWindow->SdlRenderer);

    // Start the ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();

    if (customFont)
        ImGui::PushFont(customFont);

    // Number of windows to create
    const int numWindows = 25;

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(0.0, 3000.0); // Assuming window width is 800
    std::uniform_real_distribution<> disY(0.0, 1200.0); // Assuming window height is 600

    for (int i = 0; i < numWindows; ++i) {
        ImGui::SetNextWindowPos(ImVec2(disX(gen), disY(gen)), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_Once);
        ImGui::Begin(("Window " + std::to_string(i)).c_str()); // Create a window with a unique title
        ImVec2 windowSize = ImGui::GetWindowSize();
        std::string longText = "This is some really long and useful text that spans across the window to test the text wrapping and alignment functionality in ImGui. \n";
        longText = longText + longText + longText + longText + longText + longText + longText + longText + longText + longText + longText + longText + longText + longText + longText + longText + longText + longText + longText + longText;
        ImVec2 textSize = ImGui::CalcTextSize(longText.c_str());

        ImGui::SetCursorPosX((windowSize.x - textSize.x) / 2); // Center text horizontally
        ImGui::SetCursorPosY((windowSize.y - textSize.y) / 2); // Center text vertically

        ImGui::TextWrapped(longText.c_str()); // Display the text

        textSize = ImGui::CalcTextSize("This is some useful text.");
        ImGui::SetCursorPosY((windowSize.y - textSize.y) / 2); // Center text vertically
        ImGui::Text("This is some useful text."); // Display the text

        // Draw vector graphics
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();

        // Draw a red rectangle
        draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + canvas_size.x, p.y + canvas_size.y), IM_COL32(255, 0, 0, 255));

        // Draw a green circle
        draw_list->AddCircle(ImVec2(p.x + canvas_size.x / 2, p.y + canvas_size.y / 2), 50.0f, IM_COL32(0, 255, 0, 255), 32, 2.0f);

        // Draw a blue line
        draw_list->AddLine(ImVec2(p.x, p.y), ImVec2(p.x + canvas_size.x, p.y + canvas_size.y), IM_COL32(0, 0, 255, 255), 2.0f);

        ImGui::End();
    }



     // Pop custom font
    if (customFont)
        ImGui::PopFont();

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

    io.Fonts->AddFontDefault(); // Load default font

    customFont = io.Fonts->AddFontFromFileTTF("./JetBrainsMono-Regular.ttf", 18.0f);
    if (!customFont) {
        ULog::Error("Failed to load custom font!");
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

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    delegate->ApplicationWillTerminate(this);

    delete window;
    SDL_Quit();

    return 0;
}
