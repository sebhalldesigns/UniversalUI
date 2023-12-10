#ifndef GRENDERSURFACE_H
#define GRENDERSURFACE_H

#include "Graphics/General/gColor.h"
#include "Graphics/General/gSize.h"

#ifdef _WIN32
    #include <Windows.h>
    #include <gl/GL.h>
    #include <gl/glu.h>


    typedef HWND uWindowHandle;

    struct gRenderSurfaceResources {
        HWND windowHandle;
        HGLRC glRenderContextHandle;
    };

#elif __linux__
    #include <X11/Xlib.h>

    typedef Window uWindowHandle;

    struct gRenderSurfaceResources {
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

    struct gRenderSurfaceResources {
        NSOpenGLPixelFormat* pixelFormat;
        NSOpenGLContext* glContext; // OpenGL Context
    };

#else
    #error "Unsupported platform :("
#endif

class gRenderSurface {

    gRenderSurfaceResources resources;

    public:
        gSize size;
        gColor backgroundColor;

        static gRenderSurface* InitForWindow(uWindowHandle windowHandle, float width, float height);

        void SizeChanged(float width, float height);

        void Render();
};

#endif // GRENDERSURFACE_H