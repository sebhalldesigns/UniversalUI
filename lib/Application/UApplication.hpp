#ifndef UAPPLICATION_HPP
#define UAPPLICATION_HPP

#include <Application/UApplicationDelegate.hpp>

class UApplication
{
public:
    int Run(UApplicationDelegate *delegate);  
};


#endif // UAPPLICATION_HPP