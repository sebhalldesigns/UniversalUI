#ifndef UNICODE
#define UNICODE
#endif 

#include "Core/Application/uApplication.h"



#include <cstdio>


#include <windows.h>
#include <gl/GL.h>
#include <gl/glu.h>



const static wchar_t CLASS_NAME[]  = L"UniversalUI Window";

static uApplication* app;

static bool Win32Init();
static LRESULT CALLBACK Win32WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int uApplication::Run() {

    app = this;

    printf("Starting UniversalUI...\n");

    if (!Win32Init()) {
        printf("Failed to set up Win32\n");
        return -1;
    }

    this->FinishedLaunching();

     MSG msg = { };

    while (!(quitWhenLastWindowClosed && this->windows.size() <= 0)) {
           
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    printf("Closing as no windows left\n");
    
    return 0;
    
}


uWindow* uApplication::NewWindow(double width, double height, std::string title) {
    uWindow* window = new uWindow();

    std::wstring wideTitle = std::wstring(title.begin(), title.end());


     window->systemHandle = CreateWindowExW(
            0,                              // Optional window styles.
            CLASS_NAME,                     // Window class
            wideTitle.c_str(),  // Window text
            WS_OVERLAPPEDWINDOW,            // Window style

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, (int)width, (int)height,


            nullptr,       // Parent window
            nullptr,       // Menu
            GetModuleHandleW(nullptr),  // Instance handle
            nullptr        // Additional application data
    );

    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
    SetClassLongPtr(window->systemHandle, GCLP_HBRBACKGROUND, (LONG_PTR)brush);

    
    if (window->systemHandle == nullptr)
    {
        printf("ERROR: Window creation failed!");
        return nullptr;
    }

    uRenderSurface* renderSurface = uRenderSurface::InitForWindow(window->systemHandle, width, height);

    if (renderSurface == nullptr) {
        printf("ERROR: Render surface creation failed!");
        return nullptr;
    }

    window->renderSurface = renderSurface;


    ShowWindow(window->systemHandle, SW_SHOW);

        
    windows.push_back(window);


    return window;
}

void uApplication::DestroyWindow(uWindow* window) {

    if (CloseWindow(window->systemHandle)) {

        // remove all occurrences of window from windows 
        //  I cant think why this would be more than one 
        //  but assume it might be more than one
        for (int i = 0; i < app->windows.size(); i++) {
            if (app->windows[i] == window) {
                app->windows.erase(app->windows.begin() + i);
                i--;
            }
        }

        delete window;
    }

}

uWindow* uApplication::GetWindowFromHandle(uWindowHandle handle) {
    for (int i = 0; i < app->windows.size(); i++) {
        if (app->windows[i]->systemHandle == handle) {
            return app->windows[i];
        }
    }
    return nullptr;
}

void uApplication::FinishedLaunching() {

}
void uApplication::WillQuit() {

}

bool uApplication::ShouldQuit() {
    return true;
}



bool Win32Init() {

    // Register the window class.
    WNDCLASS wc = { };

    wc.lpfnWndProc   = Win32WindowProcedure;
    wc.hInstance     = GetModuleHandleW(nullptr);
    wc.lpszClassName = CLASS_NAME;

    if (RegisterClass(&wc) == 0) {
        return false;
    }
    
    return true;
}

LRESULT CALLBACK Win32WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

     switch (uMsg) {
        case WM_CLOSE: {
                uWindow* window = app->GetWindowFromHandle(hwnd);

                if (window != nullptr) {
                    app->DestroyWindow(window);
                }

                printf("Window %x closed\n", hwnd);

                return 0;
            }

        case WM_SIZE: {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);

                uWindow* window = app->GetWindowFromHandle(hwnd);

                if (window != nullptr) {
                    window->renderSurface->SizeChanged(width, height);
                }

                return 0;
            }


        case WM_PAINT: {
            
            uWindow* window = app->GetWindowFromHandle(hwnd);

            if (window != nullptr) {
                window->renderSurface->Render();
            }
                    
            return 0;
        }

        case WM_DISPLAYCHANGE: {
            InvalidateRect(hwnd, NULL, FALSE); // Request a redraw
            return 0;
        }
            
        default:
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}