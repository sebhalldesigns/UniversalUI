#ifndef ULAYOUTCONSTRAINT_H
#define ULAYOUTCONSTRAINT_H

enum uLayoutAnchor {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    WIDTH,
    HEIGHT,
};

class uLayoutConstraint {

    friend class uView;
    
    uView* primaryNode;
    uView* secondaryNode;
    uLayoutAnchor primaryAnchor;
    uLayoutAnchor secondaryAnchor;
    double offset;
    double multiplier;

public:

    uLayoutConstraint(uView* primary, 
    uLayoutAnchor primaryAnch, uView* secondary, 
    uLayoutAnchor secondaryAnch, double initOffset = 0.0, double initMultiplier = 1.0) {
        primaryNode = primary;
        secondaryNode = secondary;
        primaryAnchor = primaryAnch;
        secondaryAnchor = secondaryAnch;
        offset = initOffset;
        multiplier = initMultiplier;
    }

    // public get 
    uView* PrimaryNode() {
        return primaryNode;
    }

    uView* SecondaryNode() {
        return secondaryNode;
    }



};

#endif // ULAYOUTCONSTRAINT_H