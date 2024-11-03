#ifndef USIZE_HPP
#define USIZE_HPP

class USize
{
public:
    float Width;
    float Height;

    USize();
    USize(float width, float height) : Width(width), Height(height) {}
};

#endif // USIZE_HPP