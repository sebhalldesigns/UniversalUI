#ifndef URENDERSURFACE_H
#define URENDERSURFACE_H

#include "Graphics/General/uColor.h"
#include "Graphics/General/uSize.h"
#include "Graphics/Canvas/uCanvas.h"

#include <vector>

#ifdef _WIN32
    #include <Windows.h>
    #include <gl/GL.h>
    #include <gl/glu.h>


    typedef HWND uWindowHandle;

    struct uRenderSurfaceResources {
        HWND windowHandle;
        HGLRC glRenderContextHandle;
    };

#elif __linux__
    #include <X11/Xlib.h>

    typedef Window uWindowHandle;

    struct uRenderSurfaceResources {
        Atom deleteButtonAtom; // atom for intercepting delete button
        XSetWindowAttributes setWindowAttributes;
        XVisualInfo *visualInfo;
        Colormap colormap;
        GLXContext glxContext; // GLX OpenGL Context
        GC xGraphicsContext;
        Pixmap backBuffer;
    };

#elif __APPLE__
    #include <Cocoa/Cocoa.h>
    
    typedef NSWindow* uWindowHandle;

    struct uRenderSurfaceResources {
        NSOpenGLPixelFormat* pixelFormat;
        NSOpenGLContext* glContext; // OpenGL Context
    };

#else
    #error "Unsupported platform :("
#endif

class uRenderSurface {

    uRenderSurfaceResources resources;

    public:
        uSize size;
        uColor backgroundColor;

        // all canvases to be drawn on render. Each contains a fully defined set of drawing commands
        std::vector<uCanvas*> canvasList;

        static uRenderSurface* InitForWindow(uWindowHandle windowHandle, float width, float height);

        void SizeChanged(float width, float height);

        void Render();
};

#endif // URENDERSURFACE_H