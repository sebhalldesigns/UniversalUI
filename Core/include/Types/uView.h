//  uView.h - Core - UniversalUI
//  First created by Seb Hall on 18/Feb/2024
//
//  UniversalUI is a cross-platform UI framework
//  that provides an efficient, simple, and
//  intuitive API for developing apps and GUIs.
//
//  uView contains the View type, the base class
//  for every control and visual element in
//  UniversalUI.

#ifndef UVIEW_H
#define UVIEW_H

#include <vector>
#include "GspSystem.h"

typedef gFrame uFrame;

class uView {
public:
    
    std::vector<uView*> subviews;

    virtual void Resized(uFrame newFrame) { }
    


};



#endif // UVIEW_H
