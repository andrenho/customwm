#ifndef LOGGER_HH_
#define LOGGER_HH_

#include <string>

class Logger {
public:
    enum { Info, Debug } level = Info;

    void info(const char* fmt, ...);
    void info(std::string const& text);

    void debug(const char* fmt, ...);
    void debug(std::string const& text);

    void error(const char* fmt, ...);
    void error(std::string const& text);
};

extern Logger LOG;

#endif //LOGGER_HH_
