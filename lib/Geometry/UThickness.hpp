#ifndef UTHICKNESS_HPP
#define UTHICKNESS_HPP

class UThickness
{
public:
    float Left;
    float Top;
    float Right;
    float Bottom;

    UThickness();
    UThickness(float left, float top, float right, float bottom) : Left(left), Top(top), Right(right), Bottom(bottom) {}
};

#endif // UTHICKNESS_HPP