#ifndef ULAYOUTTREE_H
#define ULAYOUTTREE_H

#include "uLayoutNode.h"

// in initial implementation, constraints are solved top down,
// starting with the root node

// extensive use of pointers, so there is privating and friending to ensure memory is
// only ever allocated and deallocated correctly.

class uLayoutTree {
public:

    uLayoutNode rootNode;

};


#endif // ULAYOUTTREE_H