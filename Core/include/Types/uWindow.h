//  uWindow.h - Core - UniversalUI
//  First created by Seb Hall on 18/Feb/2024
//
//  UniversalUI is a cross-platform UI framework
//  that provides an efficient, simple, and
//  intuitive API for developing apps and GUIs.
//
//  uWindow.h defines the uWindow class, representing
//  the window or viewport in which views are drawn.

#ifndef UWINDOW_H
#define UWINDOW_H

#include <vector>
#include <string>
#include "GspSystem.h"

typedef gSize uSize;
typedef gPoint uPoint;
typedef gWindowVisibility uWindowVisibility;
typedef gWindowFocus uWindowFocus;

class uWindow {
    
    gWindow windowHandle;

public:
   
   // constructor
   uWindow(std::string title, uSize size);

   virtual void ResizeCallback(gWindow window, uSize newSize);
   virtual void MoveCallback(uPoint newLocation);
   virtual void VisibilityChangedCallback(uWindowVisibility visibility);
   virtual void FocusChangedCallback(uWindowFocus focus);
   virtual void DrawCallback();
   virtual void CloseCallback();
   virtual uSize ResizeRequestCallback(uSize sizeRequest);
   virtual uPoint MoveRequestCallback(uPoint locationRequest);
   virtual bool CloseRequestCallback();


};



#endif // UWINDOW_H
