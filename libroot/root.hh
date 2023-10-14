#ifndef ROOT_HH_
#define ROOT_HH_

#include <optional>
#include <string>
#include "wm.hh"

class Root {
public:
    Root(std::optional<std::string> const& display) {}
    virtual ~Root() = default;

    virtual std::unique_ptr<WM> create_wm() = 0;
    virtual std::string interface_name() const = 0;
};

#endif //ROOT_HH_
