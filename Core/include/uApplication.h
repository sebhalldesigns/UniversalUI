//
// Created by sebam on 11/11/2023.
//

#ifndef UAPPLICATION_H
#define UAPPLICATION_H

#include <string>


class uApplication {
    bool quitWhenLastWindowClosed = true;
public:

    std::string name;
    int majorVersion;
    int minorVersion;


    // internal should quit flag
    bool shouldQuit = false;

    //  override functions
    virtual void FinishedLaunching();
    virtual void WillQuit();
    virtual bool ShouldQuit();

    void SetShouldQuitWhenLastWindowClosed(bool shouldQuit);

};

#endif //UAPPLICATION_H
