#include "logger.hh"

#include <cstdio>

Logger LOG;

void Logger::info(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("info: ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

void Logger::info(std::string const &text)
{
    printf("info: %s\n", text.c_str());
}

void Logger::debug(char const *fmt, ...)
{
    if (level == Debug) {
        va_list args;
        va_start(args, fmt);
        printf("debug: ");
        vprintf(fmt, args);
        printf("\n");
        va_end(args);
    }
}

void Logger::debug(std::string const &text)
{
    if (level == Debug)
        printf("debug: %s\n", text.c_str());
}
