//
// Created by sebam on 11/1/2023.
//

#ifndef UWINDOW_H
#define UWINDOW_H

#include <memory>
#include <string>

#include "Native/Windowing/nWindow.h"

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

    nWindow* nativeWindow;

    

public:
    //uWindowHandle systemHandle = NULL;

    //uView rootView;
    //uLayoutTree layoutTree;

    uWindow(double width, double height, std::string title);
    uWindow() { }

    void SetFullscreen(bool fullscreen);
    void SetVisibility(uWindowVisibility visibility);

    virtual void OnCreated() { }

};


#endif //UWINDOW_H