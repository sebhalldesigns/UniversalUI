//
// Created by sebam on 11/11/2023.
//

#ifndef UAPPLICATION_H
#define UAPPLICATION_H

#include <string>

#include "Core/Windowing/uWindow.h"

class uApplication {
public:

    uWindow* simpleWindow;

    std::string name;
    int majorVersion;
    int minorVersion;

    bool quitWhenLastWindowClosed = true;

    //  override functions
    virtual void FinishedLaunching();
    virtual void WillQuit();
    virtual bool ShouldQuit();

};

#endif //UAPPLICATION_H
