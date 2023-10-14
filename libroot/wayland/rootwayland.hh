#ifndef ROOTX11_HH_
#define ROOTX11_HH_

#include "../root.hh"

class RootWayland : public Root {
public:
    std::string interface_name() const override { return "wayland"; }
};

#endif //ROOTX11_HH_
