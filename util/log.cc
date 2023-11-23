#include "log.hh"

#include <cstdio>
#include <cstdarg>
#include <unistd.h>

LogLevel log_level = LogLevel::Info;

void debug(char const *fmt, ...)
{
    if (log_level == LogLevel::Debug) {
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

void debug(std::string const &text)
{
    if (log_level == LogLevel::Debug)
        printf("debug: %s\n", text.c_str());
}

void info(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("info: ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

void error(std::string const &text)
{
    std::string red, reset;
    if (isatty(fileno(stdout))) {
        red = "\e[0;31m";
        reset = "\e[0m";
    }
    printf("%serror: %s%s\n", red.c_str(), text.c_str(), reset.c_str());
}

void error(char const *fmt, ...)
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

void info(std::string const &text)
{
    printf("info: %s\n", text.c_str());
}

