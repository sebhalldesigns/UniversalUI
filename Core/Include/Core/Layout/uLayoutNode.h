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


    std::vector<uLayoutNode*> childNodes;
    
    uRectangle frame;

    void AddConstraint(uLayoutNode* parentNode, uLayoutAnchor parentAnchor, uLayoutAnchor secondaryAnchor, double offset, double multiplier = 1.0) {
        uLayoutConstraint* constraint = new uLayoutConstraint(parentNode, 
       parentAnchor, this, secondaryAnchor, offset, multiplier);

        dependentConstraints.push_back(constraint);
        parentNode->dependingConstraints.push_back(constraint);
    }

    void EvaluateConstraints() {
       
    }


};


#endif // ULAYOUTNODE_H