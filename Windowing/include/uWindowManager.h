//
// Created by sebam on 11/1/2023.
//

#ifndef UWINDOWMANAGER_H
#define UWINDOWMANAGER_H

#include "uWindow.h"

#include <vector>

#ifdef _WIN32
    #include <gl/GL.h>
    #include <windows.h>

    struct uWindowStuffForManager {
        uWindow* windowPointer;
        HGLRC glRenderContextHandle;
    };

#endif //_WIN32

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <Cocoa/Cocoa.h>

struct uWindowStuffForManager {
    uWindow* windowPointer;
    CGLContextObj glRenderContext; // Core OpenGL Context
};

#endif // __APPLE__


#ifdef __linux__
#include <GL/gl.h>
#include <GL/glx.h>

struct uWindowStuffForManager {
    uWindow* windowPointer;
    GLXContext glRenderContext; // GLX OpenGL Context
};

#endif // __linux__




class uWindowManager {

    

public:

    static std::vector<uWindowStuffForManager> windows;

    static bool Init();

    static void PollEvents();

    static bool IsWindowsEmpty();

    static void CreateNewWindow(uWindow* window, double width, double height, std::string title);
    static void SetWindowVisibility(uWindow* window, uWindowVisibility visibility);

    //  for now used for win32 processing
    static void DestroyWindowByHandle(uWindowHandle handle);
};

#endif //UWINDOWMANAGER_H
