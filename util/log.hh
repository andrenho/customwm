#ifndef LOG_HH_
#define LOG_HH_

#include <string>

enum class LogLevel { Info, Debug };
extern LogLevel log_level;

void info(const char* fmt, ...);
void info(std::string const& text);

void debug(const char* fmt, ...);
void debug(std::string const& text);

void error(const char* fmt, ...);
void error(std::string const& text);

#endif //LOG_HH_
