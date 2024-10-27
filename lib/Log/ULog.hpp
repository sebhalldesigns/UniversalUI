#ifndef ULOG_HPP
#define ULOG_HPP

class ULog 
{
public:
    static void Info(const char* message, ...);
    static void Warning(const char* message, ...);
    static void Error(const char* message, ...);
};

#endif // ULOG_HPP