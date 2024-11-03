#ifndef UAPPLICATION_HPP
#define UAPPLICATION_HPP

#include <string>

#include <Application/UApplicationDelegate.hpp>
#include <Window/UWindowDelegate.hpp>


class UApplication
{
public:

    std::string Name;
    std::string Description;
    std::string Developer;
    std::string Version;

    UApplicationDelegate *applicationDelegate;
    UWindowDelegate *windowDelegate;

    int Run();  
};


#endif // UAPPLICATION_HPP