#ifndef UAPPLICATION_H
#define UAPPLICATION_H


#include "Core/uView.h"
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

    uView* currentActiveView = nullptr;

    uWindow* NewWindow(double width, double height, std::string title);

    // destroy and deallocate window memory
    //  any subsequent reference to the window will cause a segfault
    //  this should be the last thing called on a window
    void DestroyWindow(uWindow* window);
    uWindow* GetWindowFromHandle(uWindowHandle handle);

    //  override functions
    virtual void FinishedLaunching();
    virtual void WillQuit();
    virtual bool ShouldQuit();

    int Run();
    
};

#endif //UAPPLICATION_H