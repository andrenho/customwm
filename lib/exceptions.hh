#ifndef EXCEPTIONS_HH_
#define EXCEPTIONS_HH_

#include <stdexcept>

class PropertyNotFoundException : public std::runtime_error {
public:
    explicit PropertyNotFoundException(std::string const& property)
        : std::runtime_error("Property '" + property + "' not found") {}
};

class LuaException : public std::runtime_error {
public:
    explicit LuaException(std::string const& error) : std::runtime_error(error) {}
};

#endif //EXCEPTIONS_HH_
