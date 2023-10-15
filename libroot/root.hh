#ifndef ROOT_HH_
#define ROOT_HH_

#include <memory>
#include <optional>
#include <string>
#include "server.hh"

class Root {
public:
    virtual ~Root() = default;

    virtual std::string interface_name() const = 0;
    virtual Server& server() = 0;
};

#endif //ROOT_HH_
