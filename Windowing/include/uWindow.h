//
// Created by sebam on 11/1/2023.
//

#ifndef UWINDOW_H
#define UWINDOW_H

#include <memory>
#include <string>

#include "uView.h"
#include "uLayoutTree.h"

#ifdef _WIN32

    #ifndef UNICODE
        #define UNICODE
    #endif

    #include <Windows.h>

    typedef HWND uWindowHandle;

#elif __linux__
    #include <X11/Xlib.h>

    typedef Window uWindowHandle;

#elif __APPLE__

#else
    #error "Unsupported platform :("
#endif




enum uWindowVisibility {
    HIDDEN,
    VISIBLE,
    MINIMISED
};

class uWindow {

    friend class uWindowManager;
    friend class uRenderManager;

    double width;
    double height;
    
    ~uWindow();

public:
    uWindowHandle systemHandle = NULL;

    uView rootView;
    uLayoutTree layoutTree;
    
    // TODO: make friend class and these private
    static bool _POST();
    static void _POLL();

    uWindow(double width, double height, std::string title);

    void SetFullscreen(bool fullscreen);
    void SetVisibility(uWindowVisibility visibility);

};


#endif //UWINDOW_H
