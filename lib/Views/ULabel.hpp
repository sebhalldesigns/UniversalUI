#ifndef ULABEL_HPP
#define ULABEL_HPP

#include <View/UView.hpp>

#include <string>

class ULabel : public UView
{
public:
    ULabel();
    ~ULabel();

    std::string Text;
    float FontSize;
    
    void Draw();
    void Measure(USize availableSize);
    void Arrange(URect finalRect);
};

#endif // ULABEL_HPP