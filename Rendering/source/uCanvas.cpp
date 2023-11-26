#include "../include/uCanvas.h"

uCanvas::uCanvas(double initWidth, double initHeight) {

}

// free memory from all "new" drawing items
uCanvas::~uCanvas() {
    
    while (points.size() > 0) {
        delete points[0];
        points.erase(points.begin());
    }

    while (rectangles.size() > 0) {
        delete rectangles[0];
        rectangles.erase(rectangles.begin());
    }

    while (images.size() > 0) {
        delete images[0];
        images.erase(images.begin());
    }
}   


Dot* uCanvas::NewPoint(double x, double y) {
    Dot* newPoint = new Dot();
    newPoint->x = x;
    newPoint->y = y;
    points.push_back(newPoint);
    return newPoint;
}

Rectangle* uCanvas::NewRectangle(double x, double y, double width, double height) {
    Rectangle* newRectangle = new Rectangle();
    newRectangle->x = x;
    newRectangle->y = y;
    newRectangle->width;
    newRectangle->height;
    rectangles.push_back(newRectangle);

    return newRectangle;
}

Image* uCanvas::NewImage(std::string filePath, double x, double y, double width, double height) {
    Image* newImage = new Image();
    newImage->x = x;
    newImage->y = y;
    newImage->width = width;
    newImage->height = height;
    newImage->filePath = filePath;
    images.push_back(newImage);

    return newImage;
}