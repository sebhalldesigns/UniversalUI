#ifndef UVIEWCONTROLLER_H
#define UVIEWCONTROLLER_H

#include "uView.h"

class uViewController {
public:

    uViewController(uView* view) {

    }

    virtual void MouseOver() {

    }

    virtual void Drag() {

    }

    // idk about this one
    virtual void Resized() {

    }

    protected:
    uView* view; // The view managed by this controller

};


#endif // UVIEWCONTROLLER_H