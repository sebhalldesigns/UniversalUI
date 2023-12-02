#ifndef UVIEW_H
#define UVIEW_H

#include <vector>
#include <stdio.h>
//#include "uLayoutTree.h"
//#include "uCanvas.h"
//#include "uRenderNode.h"

struct uRect {
    double x;
    double y;
    double width;
    double height;
};

struct uAnchorPoint {
    double viewProportionX; // relative to view origin
    double viewProportionY;
};

struct uAnchorPointSet {
    uAnchorPoint top = { 0.5, 0.0 };
    uAnchorPoint left = { 0.0, 0.5 };
    uAnchorPoint bottom = { 0.5, 1.0 };
    uAnchorPoint right = { 1.0, 0.5 };
    
    uAnchorPoint center = { 0.5, 0.5 };

    uAnchorPoint topLeft = { 0.0, 0.0 };
    uAnchorPoint topRight = { 1.0, 0.0 };
    uAnchorPoint bottomLeft = { 0.0, 1.0 };
    uAnchorPoint bottomRight = { 1.0, 1.0 };
};

class uView {

public:

    // actually responsible for layout + size
    uLayoutNode* layoutNode;
    //uRect frame;

    double borderThickness;
    double borderRadius;
    uColor borderColor;

    uColor backgroundColor;

    uAnchorPointSet anchorPoints;

    std::vector<uView*> subviews;

    uView(uRect initFrame) {
        //frame = initFrame;
    }

    // default constructor
    uView() { }

    virtual void Draw(uCanvas& canvas) {
        //printf("DRAW!\n");
    }

};



#endif // UVIEW_H
