#ifndef RESOURCES_HH_
#define RESOURCES_HH_

#include "theme/types/lwindow.hh"

class Resources {
public:
    virtual ~Resources() = default;

    virtual void                   set_property(WHandle window, std::string const& name, WHandle const& value) = 0;
    virtual std::optional<WHandle> get_property_whandle(WHandle window, std::string const& name) = 0;
};


#endif //RESOURCES_HH_
