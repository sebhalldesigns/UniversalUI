#ifndef ULAYOUTCONSTRAINT_H
#define ULAYOUTCONSTRAINT_H

class uLayoutNode;

enum uLayoutAnchor {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    WIDTH,
    HEIGHT,
};

class uLayoutConstraint {

    friend class uLayoutNode;
    
    uLayoutNode* primaryNode;
    uLayoutNode* secondaryNode;
    uLayoutAnchor primaryAnchor;
    uLayoutAnchor secondaryAnchor;
    double offset;
    double multiplier;

public:

    uLayoutConstraint(uLayoutNode* primary, 
    uLayoutAnchor primaryAnch, uLayoutNode* secondary, 
    uLayoutAnchor secondaryAnch, double initOffset = 0.0, double initMultiplier = 1.0) {
        primaryNode = primary;
        secondaryNode = secondary;
        primaryAnchor = primaryAnch;
        secondaryAnchor = secondaryAnch;
        offset = initOffset;
        multiplier = initMultiplier;
    }

    // public get 
    uLayoutNode* PrimaryNode() {
        return primaryNode;
    }

    uLayoutNode* SecondaryNode() {
        return secondaryNode;
    }



};

#endif // ULAYOUTCONSTRAINT_H