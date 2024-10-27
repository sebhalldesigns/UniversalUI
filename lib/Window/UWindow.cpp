#include <Window/UWindow.hpp>

UWindow::UWindow()
{

    Size = { 800, 600 };

    SdlWindow = SDL_CreateWindow("UniversalUI Window",
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 800, 600,
                                 SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SdlRenderer = SDL_CreateRenderer(SdlWindow, -1, SDL_RENDERER_ACCELERATED);

    
}

UWindow::~UWindow()
{
    SDL_DestroyRenderer(SdlRenderer);
    SDL_DestroyWindow(SdlWindow);
}

USize UWindow::GetSize()
{
    return Size;
}

void UWindow::SetSize(USize size)
{
    Size = size;
}
