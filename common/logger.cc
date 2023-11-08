#include "logger.hh"

#include <cstdio>
#include <unistd.h>

Logger LOG;

void Logger::debug(char const *fmt, ...)
{
    if (level == Debug) {
        std::string gray, reset;
        if (isatty(fileno(stdout))) {
            gray = "\e[0;37m";
            reset = "\e[0m";
        }

        va_list args;
        va_start(args, fmt);
        printf("%sdebug: ", gray.c_str());
        vprintf(fmt, args);
        printf("%s\n", reset.c_str());
        va_end(args);
    }
}

void Logger::debug(std::string const &text)
{
    if (level == Debug)
        printf("debug: %s\n", text.c_str());
}

void Logger::info(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("info: ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

void Logger::error(std::string const &text)
{
    std::string red, reset;
    if (isatty(fileno(stdout))) {
        red = "\e[0;31m";
        reset = "\e[0m";
    }
    printf("%serror: %s%s\n", red.c_str(), text.c_str(), reset.c_str());
}

void Logger::error(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (isatty(fileno(stdout)))
        printf("\e[0;31m");
    printf("error: ");
    vprintf(fmt, args);
    if (isatty(fileno(stdout)))
        printf("\e[0m");
    printf("\n");
    va_end(args);
}

void Logger::info(std::string const &text)
{
    printf("info: %s\n", text.c_str());
}

