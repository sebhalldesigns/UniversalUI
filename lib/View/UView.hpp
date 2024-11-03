#ifndef UVIEW_HPP
#define UVIEW_HPP

#include <Geometry/USize.hpp>
#include <Geometry/URect.hpp>
#include <Geometry/UThickness.hpp>

#include <vector>

enum class ULayoutType
{
    Canvas,
    Stack,
    Dock,
    Grid
};

enum class UStackOrientation
{
    Horizontal,
    Vertical
};

enum class UDockLocation
{
    Left,
    Top,
    Right,
    Bottom
};


class UGridLayoutSize
{
public:
    float MinimumSize;
    float MaximumSize;
    float DesiredRatio;
};

class UGridLayout
{
public:
    std::vector<UGridLayoutSize> RowSizes;
    std::vector<UGridLayoutSize> ColumnSizes;
};

class UView
{
public:
    
    std::vector<UView*> Children;

    URect ComputedRect;

    USize DesiredSize;
    UThickness Margin;
    UThickness Padding;

    // layout properties affecting layout of children
    ULayoutType LayoutType;
    UStackOrientation StackOrientation;
    UGridLayout GridLayout;

    // layout properties affecting layout of this view
    UDockLocation DockLocation;
    URect CanvasRect;

    
    UView();
    ~UView();

    // CORE METHODS - HANDLED BY BASE CLASS
    void MeasureCore(USize availableSize); // propogate measure calls down tree
    void ArrangeCore(URect finalRect); // propogate arrange calls down tree

    // VIRTUAL METHODS - OVERRIDE IN DERIVED CLASSES

    virtual void Draw() = 0;
    virtual void Measure(USize availableSize) = 0;
    virtual void Arrange(URect finalRect) = 0;

};

#endif // UVIEW_HPP