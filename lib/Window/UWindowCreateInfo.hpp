#ifndef UWINDOWCREATEINFO_HPP
#define UWINDOWCREATEINFO_HPP

#include <string>
#include <Geometry/USize.hpp>


struct UWindowCreateInfo
{   
    USize Size;
    std::string Title;
};

#endif // UWINDOWCREATEINFO_HPP