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

    while (true) {
           
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    
    return 0;
    
}


uWindow* uApplication::NewWindow(double width, double height, std::string title) {
    uWindow* window = new uWindow();

    std::wstring wideTitle = std::wstring(title.begin(), title.end());

    uWindowResources* windowResources = new uWindowResources();

     windowResources->systemHandle = CreateWindowExW(
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

        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            24, // bit depth
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            16, // depth buffer
            0, // stencil buffer
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        if (windowResources->systemHandle == nullptr)
        {
            printf("ERROR: Window creation failed!");
            return nullptr;
        }

        HDC hdc = GetDC(windowResources->systemHandle);
        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pixelFormat, &pfd);
        windowResources->glRenderContextHandle = wglCreateContext(hdc);
        wglMakeCurrent(hdc, windowResources->glRenderContextHandle);

        
        glEnable(GL_BLEND);  
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
        glEnable(GL_TEXTURE_2D);   
        glEnable(GL_POINT_SMOOTH); 

        ReleaseDC(windowResources->systemHandle, hdc);


        ShowWindow(windowResources->systemHandle, SW_SHOW);


        window->resources = windowResources;
        
        windows.push_back(window);


    return window;
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
        case WM_CLOSE:
            //uWindowManager::DestroyWindowByHandle(hwnd);
            printf("Window %x close\n", hwnd);

            return 0;
        case WM_SIZE: {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);

               /*for (int i = 0; i < uWindowManager::windows.size(); i++) {
                    if (uWindowManager::windows[i].windowPointer->systemHandle == hwnd) {
                        HDC hdc = GetDC(hwnd); // Get the device context for the window
                        wglMakeCurrent(hdc, uWindowManager::windows[i].glRenderContextHandle);
                        glViewport(0, 0, width, height);
                        wglMakeCurrent(NULL, NULL); // Optionally, make no context current  

                        printf("RESIZE %d %d\n", width, height);

                        ReleaseDC(hwnd, hdc); // Release the device context
                        InvalidateRect(hwnd, NULL, FALSE); // Request a redraw

                        uWindowManager::windows[i].windowPointer->layoutTree.rootNode.frame.width = width;
                        uWindowManager::windows[i].windowPointer->layoutTree.rootNode.frame.height = height;

                        uWindowManager::windows[i].windowPointer->layoutTree.rootNode.EvaluateConstraints();
                    }
                }*/
                
                return 0;
            }


        case WM_PAINT: {

            
            for (int i = 0; i < app->windows.size(); i++) {
                if (app->windows[i]->resources->systemHandle == hwnd) {

                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    wglMakeCurrent(hdc, app->windows[i]->resources->glRenderContextHandle);


                    // Set up OpenGL viewport, projection, etc.
                    glViewport(0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    glOrtho(0.0, ps.rcPaint.right, ps.rcPaint.bottom, 0.0, -1.0, 1.0);
                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();

                    // Clear screen
                    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);

                    //RenderView(&uWindowManager::windows[i].windowPointer->rootView);

                    for (int j = 0;j < 30; j++) {
                        //RenderText("By systematically checking each of these areas, you should be able to identify and correct the issue with the color in your text rendering. If the problem persists, please provide a code snippet or more details about your rendering setup, and I can offer more specific guidance.", 10.0f, 10.0f + 15.0f*(float)j, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
                    }
                    
                    SwapBuffers(hdc);

                    EndPaint(hwnd, &ps);
                }

            
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