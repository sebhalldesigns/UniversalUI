//
// Created by sebam on 11/1/2023.
//

#ifndef UWINDOW_H
#define UWINDOW_H

#include <memory>
#include <string>


#ifdef _WIN32
    #include <Windows.h>

    typedef HWND uWindowHandle;
#elif __linux__
    #include <X11/Xlib.h>

    typedef Window uWindowHandle;


#elif __APPLE__
    #include <Cocoa/Cocoa.h>

    typedef NSWindow* uWindowHandle;

#else
    #error "Unsupported platform :("
#endif

#include "Graphics/Surface/uRenderSurface.h"


enum uWindowVisibility {
    HIDDEN,
    VISIBLE,
    MINIMISED
};

class uWindow {

    friend class uApplication;

    double width;
    double height;

    ~uWindow() = default;

public:

    uWindowHandle systemHandle = NULL;
    uRenderSurface* renderSurface;

    //uView rootView;
    //uLayoutTree layoutTree;

    uWindow(double width, double height, std::string title);
    uWindow() { }

    void SetFullscreen(bool fullscreen);
    void SetVisibility(uWindowVisibility visibility);

    virtual void OnCreated() { }

};


#endif //UWINDOW_H