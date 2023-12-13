#ifndef UVIEW_H
#define UVIEW_H

#include <vector>
#include <stdio.h>
//#include "uLayoutTree.h"
//#include "uCanvas.h"
//#include "uRenderNode.h"
#include "Core/Layout/uLayoutConstraint.h"
#include "Graphics/Canvas/uCanvas.h"
#include "Graphics/General/uPoint.h"
#include "Graphics/General/uColor.h"

struct uRect {
    float x;
    float y;
    float width;
    float height;

    bool ContainsPoint(uPoint point) {
        if (point.x >= x && point.y >= y && point.x <= (x + width) && point.y <= (y + height)) {
            return true;
        }

        return false;
    }
};

struct uAnchorPoint {
    float viewProportionX; // relative to view origin
    float viewProportionY;
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
    friend class uApplication;
    friend class uWindow;

    uAnchorPointSet anchorPoints;

    std::vector<uView*> subviews;

    std::vector<uLayoutConstraint*> dependentConstraints;
    std::vector<uLayoutConstraint*> dependingConstraints;

public:

    uRect frame;
    uColor backgroundColor;

    uView(uRect initFrame) {
        frame = initFrame;
        backgroundColor = {0.0f, 0.0f, 0.0f, 0.0f};
    }

    // default constructor
    uView() { 
                backgroundColor = {0.0f, 0.0f, 0.0f, 0.0f};
    }

    // view contains logic for layout and input?? 

    virtual void Draw(uCanvas& canvas) {
        //printf("DRAW!\n");
    }

    virtual void MouseEnter() { }
    virtual void MouseExit() { }
    virtual void MouseDown() { }
    virtual void MouseUp() { }

    void AddSubview(uView* view) {

        // add to views tree and layout tree
        subviews.push_back(view);
    }

    void RemoveSubview(uView* view) {


        for (int i = 0; i < subviews.size(); i++) {
            if (subviews[i] == view) {
                subviews.erase(subviews.begin() + i);
                i--;
            }
        }

    }

    std::vector<uView*> Subviews() {
        return subviews;
    }

  

    void AddConstraint(uView* parentNode, uLayoutAnchor parentAnchor, uLayoutAnchor secondaryAnchor, double offset, double multiplier = 1.0) {
        uLayoutConstraint* constraint = new uLayoutConstraint(parentNode, 
        parentAnchor, this, secondaryAnchor, offset, multiplier);

        dependentConstraints.push_back(constraint);
        parentNode->dependingConstraints.push_back(constraint);
    }

    void LayoutSubviews() {
         for (uLayoutConstraint* constraint : dependingConstraints) {

            double primaryMeasurement = NAN;

            switch (constraint->primaryAnchor) {
                case uLayoutAnchor::TOP:
                    primaryMeasurement = frame.y;
                    break;
                case uLayoutAnchor::BOTTOM:
                    primaryMeasurement = frame.y + frame.height;
                    break;
                case uLayoutAnchor::LEFT:
                    primaryMeasurement = frame.x;
                    break;
                case uLayoutAnchor::RIGHT:
                    primaryMeasurement = frame.x + frame.width;
                    break;
                case uLayoutAnchor::WIDTH:
                    primaryMeasurement = frame.width;
                    break;
                case uLayoutAnchor::HEIGHT:
                    primaryMeasurement = frame.height;
                    break;
            }

            if (isnan(primaryMeasurement)) {
                printf("PRIMARY MEASUREMENT FAILED\n");
                break;
            }

            switch (constraint->secondaryAnchor) {
                case uLayoutAnchor::TOP:
                    constraint->secondaryNode->frame.y = (primaryMeasurement * constraint->multiplier) + constraint->offset; 
                    break;
                case uLayoutAnchor::BOTTOM:
                    constraint->secondaryNode->frame.height = ((primaryMeasurement * constraint->multiplier) + constraint->offset) - constraint->secondaryNode->frame.y;
                    break;
                case uLayoutAnchor::LEFT:
                    constraint->secondaryNode->frame.x = (primaryMeasurement * constraint->multiplier) + constraint->offset; 
                    break;
                case uLayoutAnchor::RIGHT:
                    constraint->secondaryNode->frame.width = ((primaryMeasurement * constraint->multiplier) + constraint->offset) - constraint->secondaryNode->frame.x;
                    break;
                case uLayoutAnchor::WIDTH:
                    constraint->secondaryNode->frame.width = (primaryMeasurement * constraint->multiplier) + constraint->offset; 
                    break;
                case uLayoutAnchor::HEIGHT:
                    constraint->secondaryNode->frame.height = (primaryMeasurement * constraint->multiplier) + constraint->offset; 
                    break;
            }

        }

        for (uView* subview : subviews) {
            subview->LayoutSubviews();
        }
    }

};



#endif // UVIEW_H
