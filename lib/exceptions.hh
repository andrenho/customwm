#ifndef EXCEPTIONS_HH_
#define EXCEPTIONS_HH_

#include <stdexcept>

class PropertyNotFoundException : public std::runtime_error {
public:
    explicit PropertyNotFoundException(std::string const& property)
        : std::runtime_error("Property '" + property + "' not found") {};
};

#endif //EXCEPTIONS_HH_
