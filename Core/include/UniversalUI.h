//
// Created by sebam on 11/11/2023.
//

#ifndef UNIVERSALUI_H
#define UNIVERSALUI_H

#include "uApplication.h"
#include "uWindowManager.h"

//  UniversalUI main function - takes a subclass
//  of uApplication and only returns on application quit.
//  It should be used at the end of a simple main function
//  e.g:
//
//      MyAppClass* myApp = new MyAppClass();
//      return UniversalUI(myApp);
//
int UniversalUI(uApplication* application);

#endif //UNIVERSALUI_H
