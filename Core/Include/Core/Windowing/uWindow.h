//
// Created by sebam on 11/1/2023.
//

#ifndef UWINDOW_H
#define UWINDOW_H

#include <memory>
#include <string>


#ifdef _WIN32
    #include <Windows.h>

    struct uWindowResources {
        HWND systemHandle;
        HGLRC glRenderContextHandle;
    };

#elif __linux__
    #include <X11/Xlib.h>

    struct uWindowResources {
        Window systemHandle;
        Colormap colormap;
        Atom deleteButtonAtom; // atom for intercepting delete button
        GLXContext glxContext; // GLX OpenGL Context
        XSetWindowAttributes setWindowAttributes;
        XVisualInfo *visualInfo;
        GC xGraphicsContext;
        Pixmap backBuffer;
    };

#elif __APPLE__
    #include <Cocoa/Cocoa.h>

    struct uWindowResources {
        NSWindow* systemHandle;
        NSOpenGLPixelFormat* pixelFormat;
        NSOpenGLContext* glContext; // OpenGL Context
    };
#else
    #error "Unsupported platform :("
#endif


enum uWindowVisibility {
    HIDDEN,
    VISIBLE,
    MINIMISED
};

class uWindow {

    friend class uApplication;

    double width;
    double height;

public:

    uWindowResources* resources = nullptr;

    //uView rootView;
    //uLayoutTree layoutTree;

    uWindow(double width, double height, std::string title);
    uWindow() { }

    void SetFullscreen(bool fullscreen);
    void SetVisibility(uWindowVisibility visibility);

    virtual void OnCreated() { }

};


#endif //UWINDOW_H