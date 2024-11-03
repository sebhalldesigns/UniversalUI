#ifndef UWINDOW_HPP
#define UWINDOW_HPP

#include <Geometry/USize.hpp>
#include <SDL2/SDL.h>
#include <imgui/imgui.h>

class UWindow
{

private:


    //USize Size;
    
public:

    SDL_Window* SdlWindow;
    SDL_Renderer* SdlRenderer;
    ImGuiContext* ImGuiContext;

    UWindow();
    ~UWindow();

    

    //USize GetSize();
    //void SetSize(USize size);

    friend class UApplication;
};


#endif // UWINDOW_HPP