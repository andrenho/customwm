#ifndef THEMEEXCEPTION_HH_
#define THEMEEXCEPTION_HH_

#include <stdexcept>

class ThemeException : public std::runtime_error {
public:
    explicit ThemeException(const char* what) : std::runtime_error(what) {}
};

#endif //THEMEEXCEPTION_HH_
