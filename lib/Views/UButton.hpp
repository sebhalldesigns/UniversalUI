#ifndef UBUTTON_HPP
#define UBUTTON_HPP

#include <View/UView.hpp>

#include <string>

class UButton : public UView
{
public:
    UButton();
    ~UButton();

    std::string Text;
    float FontSize;
    
    void Draw();
    void Measure(USize availableSize);
    void Arrange(URect finalRect);
};

#endif // UBUTTON_HPP