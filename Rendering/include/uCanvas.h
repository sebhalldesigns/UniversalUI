#ifndef UCANVAS_H
#define UCANVAS_H

#include <vector>
#include <string>

struct uColor {
    double r;
    double g;
    double b;
    double a;
};

const static uColor PAINFUL_RED = { 1.0, 0.0, 0.0, 1.0 };
const static uColor PAINFUL_GREEN = { 0.0, 1.0, 0.0, 1.0 };



class Dot {
    friend class uCanvas;
    Dot() {}
    ~Dot() {}
public:
    double x;
    double y;
    double radius;
    uColor color;
};

class Rectangle {
    friend class uCanvas;
    Rectangle() {}
    ~Rectangle() {}
public:
    double x;
    double y;
    double width;
    double height;

    double radius;

    uColor fillColor;

    double borderThickness;
    uColor borderColor;
};

class Image {
    friend class uCanvas;
    Image() {}
    ~Image() {}
public:
    double x;
    double y;
    double width;
    double height;

    double radius;

    uColor fillColor;

    double borderThickness;
    uColor borderColor;

    std::string filePath;
};


class uCanvas {

    // overall width and height
    double width;
    double height;

    std::vector<Dot*> points;
    std::vector<Rectangle*> rectangles;
    std::vector<Image*> images;

    friend class uRenderNode;
    

public:


    // should probably be private
    uCanvas(double initWidth, double initHeight);
    ~uCanvas();

    Dot* NewPoint(double x, double y);
    Rectangle* NewRectangle(double x, double y, double width, double height);
    Image* NewImage(std::string filePath, double x, double y, double width, double height);
    
    
    // TODO add more canvas items such as LINES, TRIANGLES, circles, arcs, images etc
    



};

#endif // UCANVAS_H