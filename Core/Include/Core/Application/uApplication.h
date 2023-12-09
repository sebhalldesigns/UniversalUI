#ifndef UAPPLICATION_H
#define UAPPLICATION_H

#include "Core/Windowing/uWindow.h"

#include <string>
#include <vector>

class uApplication {

  

public:

    std::vector<uWindow*> windows;

    std::string name;
    int majorVersion;
    int minorVersion;

    bool quitWhenLastWindowClosed = true;

    uWindow* NewWindow(double width, double height, std::string title);

    //  override functions
    virtual void FinishedLaunching();
    virtual void WillQuit();
    virtual bool ShouldQuit();

    int Run();
    
};

#endif //UAPPLICATION_H