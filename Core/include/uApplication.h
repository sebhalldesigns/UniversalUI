//
// Created by sebam on 11/11/2023.
//

#ifndef UAPPLICATION_H
#define UAPPLICATION_H

#include <string>

class uApplication {

public:

    std::string name;
    int majorVersion;
    int minorVersion;

    bool quitWhenLastWindowClosed = true;

    // internal should quit flag
    bool shouldQuit = false;

    //  override functions
    virtual void FinishedLaunching();
    virtual void WillQuit();
    virtual bool ShouldQuit();


};

#endif //UAPPLICATION_H
