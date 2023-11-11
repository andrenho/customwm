#ifndef RESOURCES_HH_
#define RESOURCES_HH_

#include "theme/types/lwindow.hh"

struct Cursors {
    static constexpr const char* Pointer = "pointer";
    static constexpr const char* Wait = "wait";
    static constexpr const char* BottomLeft = "bottom-left";
    static constexpr const char* BottomRight = "bottom-right";
    static constexpr const char* TopLeft = "top-left";
    static constexpr const char* TopRight = "top-right";
    static constexpr const char* Top = "top";
    static constexpr const char* Bottom = "bottom";
    static constexpr const char* Right = "right";
    static constexpr const char* Left = "left";
    static constexpr const char* Kill = "kill";
};

class Resources {
public:

    virtual ~Resources() = default;

    virtual void                   set_property(WHandle window, std::string const& name, WHandle const& value) = 0;
    virtual std::optional<WHandle> get_property_whandle(WHandle window, std::string const& name) const = 0;
};


#endif //RESOURCES_HH_
