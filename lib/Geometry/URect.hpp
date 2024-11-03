#ifndef URECT_HPP
#define URECT_HPP

class URect
{
public:
    float X;
    float Y;
    float Width;
    float Height;

    URect();
    URect(float x, float y, float width, float height) : X(x), Y(y), Width(width), Height(height) {}
};

#endif // URECT_HPP