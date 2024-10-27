#ifndef UWINDOWFACTORY_HPP
#define UWINDOWFACTORY_HPP

class UWindowFactory 
{
public:
    UWindowFactory();
    ~UWindowFactory();

    void CreateWindow();
    void DestroyWindow();
    
    void Render();

};

#endif // UWINDOWFACTORY_HPP