//
// Created by sebam on 11/11/2023.
//

#include "../include/uWindowManager.h"

std::vector<uWindowStuffForManager> uWindowManager::windows;

#include <cstdio>

#include "uBitmap.h"

#ifdef _WIN32

const wchar_t CLASS_NAME[]  = L"UniversalUI Window";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif


bool uWindowManager::Init() {

#ifdef _WIN32

    // Register the window class.
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = GetModuleHandleW(nullptr);
    wc.lpszClassName = CLASS_NAME;

    if (RegisterClass(&wc) == 0) {
        return false;
    }

#endif

    return true;
}


void uWindowManager::PollEvents() {
    #ifdef _WIN32
        // Run the message loop.
        MSG msg = { };
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    #endif
}

bool uWindowManager::IsWindowsEmpty() {
    return windows.empty();
}

void uWindowManager::CreateNewWindow(uWindow* window, double width, double height, std::string title) {

    uWindowStuffForManager stuffForManager;

    #ifdef _WIN32
        std::wstring wideTitle = std::wstring(title.begin(), title.end());

        window->systemHandle = CreateWindowExW(
                0,                              // Optional window styles.
                CLASS_NAME,                     // Window class
                wideTitle.c_str(),  // Window text
                WS_OVERLAPPEDWINDOW,            // Window style

                // Size and position
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,


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

        if (window->systemHandle == nullptr)
        {
            printf("ERROR: Window creation failed!");
            return;
        }

        stuffForManager.windowPointer = window;

        HDC hdc = GetDC(window->systemHandle);
        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pixelFormat, &pfd);
        stuffForManager.glRenderContextHandle = wglCreateContext(hdc);
        wglMakeCurrent(hdc, stuffForManager.glRenderContextHandle);
        ReleaseDC(window->systemHandle, hdc);

        windows.push_back(stuffForManager);

        //uRenderManager::SetupForWindow(window);
        //uRenderManager::RenderToWindow(window);


    #endif // _WIN32

}

void uWindowManager::DestroyWindowByHandle(uWindowHandle handle) {
    for (int i = 0; i < windows.size(); i++) {
        if (windows[i].windowPointer->systemHandle == handle) {

            #ifdef _WIN32
                if (DestroyWindow(handle)) {
                    wglMakeCurrent(NULL, NULL);
                    wglDeleteContext(windows[i].glRenderContextHandle);
                    windows.erase(windows.begin() + i);
                    i--;
                }
            #endif // _WIN32
        }
    }
}

void uWindowManager::SetWindowVisibility(uWindow* window, uWindowVisibility visibility) {

    #ifdef _WIN32
        switch (visibility) {
            case uWindowVisibility::VISIBLE:
                ShowWindow(window->systemHandle, SW_SHOW);
                break;
            case uWindowVisibility::MINIMISED:
                ShowWindow(window->systemHandle, SW_SHOWMINIMIZED);
                break;
            default:
                ShowWindow(window->systemHandle, SW_HIDE);
                break;
        }
    #endif // _WIN32
}



#ifdef _WIN32

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            uWindowManager::DestroyWindowByHandle(hwnd);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            for (int i = 0; i < uWindowManager::windows.size(); i++) {
                if (uWindowManager::windows[i].windowPointer->systemHandle == hwnd) {
                    wglMakeCurrent(hdc, uWindowManager::windows[i].glRenderContextHandle);
                }
            }

            // Set up OpenGL viewport, projection, etc.
            glViewport(0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, ps.rcPaint.right, ps.rcPaint.bottom, 0.0, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Clear screen
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Render the texture
            /*glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, /* texture ID);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(ps.rcPaint.right, 0.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(ps.rcPaint.right, ps.rcPaint.bottom);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, ps.rcPaint.bottom);
            glEnd();
            glDisable(GL_TEXTURE_2D);*/

            SwapBuffers(hdc);

            EndPaint(hwnd, &ps);
        }
            return 0;
        default:
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#endif
