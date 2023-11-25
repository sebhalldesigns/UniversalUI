//
// Created by sebam on 11/1/2023.
//

#ifndef UWINDOWMANAGER_H
#define UWINDOWMANAGER_H

#include "uWindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>

#ifdef _WIN32

    #include <gl/GL.h>
    #include <windows.h>

    struct uWindowStuffForManager {
        uWindow* windowPointer;
        HGLRC glRenderContextHandle;
    };

#elif __linux__
    #include <X11/Xlib.h>
    #include <GL/gl.h>
    #include <GL/glx.h>

    struct uWindowStuffForManager {
        uWindow* windowPointer;
        Colormap colormap;
        Atom deleteButtonAtom; // atom for intercepting delete button
        GLXContext glxContext; // GLX OpenGL Context
        XSetWindowAttributes setWindowAttributes;
        XVisualInfo *visualInfo;
        GC xGraphicsContext;
        Pixmap backBuffer;
    };

#elif __APPLE__
    #include <OpenGL/OpenGL.h>
    #include <Cocoa/Cocoa.h>

    struct uWindowStuffForManager {
        uWindow* windowPointer;
        CGLContextObj glRenderContext; // Core OpenGL Context
    };
#else
    #error "Unsupported platform :("
#endif

class uWindowManager {

    

public:

    static std::vector<uWindowStuffForManager> windows;

    static bool Init();
    static void Close();

    static void PollEvents();

    static bool IsWindowsEmpty();

    static bool WindowStuffFromSystemHandle(uWindowHandle handle, uWindowStuffForManager& windowStuff);
    static void SetWindowSize(uWindow* window, double width, double height);
    static void GetWindowSize(uWindow* window, double& width, double& height);

    static void CreateNewWindow(uWindow* window, double width, double height, std::string title);
    static void SetWindowVisibility(uWindow* window, uWindowVisibility visibility);

    //  for now used for win32 processing
    static void DestroyWindowByHandle(uWindowHandle handle);
};

#endif //UWINDOWMANAGER_H
