#ifndef WINDOWS_HH_
#define WINDOWS_HH_

#include <unordered_map>
#include "../lib/types.hh"

class Windows {
public:
    Window& add(Window const& w);
    void remove(uint32_t id);

    std::optional<Window> find(uint32_t id) const;

private:
    std::unordered_map<size_t, Window> windows_;
};

#endif //WINDOWS_HH_
