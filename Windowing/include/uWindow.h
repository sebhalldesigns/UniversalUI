//
// Created by sebam on 11/1/2023.
//

#ifndef UNIVERSALUI_UWINDOW_H
#define UNIVERSALUI_UWINDOW_H

#include <memory>
#include <string>

#ifdef _WIN32

    #ifndef UNICODE
        #define UNICODE
    #endif

    #include <Windows.h>

    typedef HWND uWindowHandle;

#endif

#ifdef __APPLE__

#endif

#ifdef __linux__

#endif

enum uWindowVisibility {
    HIDDEN,
    VISIBLE,
    MINIMISED
};

class uWindow {

    friend class uWindowManager;
    friend class uRenderManager;

  
    ~uWindow();

public:
  uWindowHandle systemHandle = nullptr;



    // TODO: make friend class and these private
    static bool _POST();
    static void _POLL();

    uWindow(double width, double height, std::string title);

    void SetFullscreen(bool fullscreen);
    void SetVisibility(uWindowVisibility visibility);

};


#endif //UNIVERSALUI_UWINDOW_H
