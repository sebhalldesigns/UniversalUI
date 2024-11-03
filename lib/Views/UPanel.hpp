#ifndef UPANEL_HPP
#define UPANEL_HPP

#include <View/UView.hpp>

class UPanel : public UView
{
public:
    UPanel();
    ~UPanel();
    
    void Draw();
    void Measure(USize availableSize);
    void Arrange(URect finalRect);
};

#endif // UPANEL_HPP