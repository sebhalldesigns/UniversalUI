#ifndef ULAYOUTNODE_H
#define ULAYOUTNODE_H

#include <vector>
#include <math.h>
#include <stdio.h>

#include "uLayoutConstraint.h"

struct uRectangle {
    double x;
    double y;
    double width;
    double height;
};

class uLayoutNode {
public:

    std::vector<uLayoutConstraint*> dependentConstraints;
    std::vector<uLayoutConstraint*> dependingConstraints;

    std::vector<uLayoutNode> childNodes;
    
    uRectangle frame;

    void AddConstraint(uLayoutNode* parentNode, uLayoutAnchor parentAnchor, uLayoutAnchor secondaryAnchor, double offset, double multiplier = 1.0) {
        uLayoutConstraint* constraint = new uLayoutConstraint(parentNode, 
       parentAnchor, this, secondaryAnchor, offset, multiplier);

        dependentConstraints.push_back(constraint);
        parentNode->dependingConstraints.push_back(constraint);
    }

    void EvaluateConstraints() {
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

        for (uLayoutNode childNode : childNodes) {
            childNode.EvaluateConstraints();
        }
    }


};


#endif // ULAYOUTNODE_H