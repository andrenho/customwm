#include "windows.hh"

Window& Windows::add(const Window &w)
{
    return windows_.insert({ w.inner_id, w }).first->second;
}

void Windows::remove(uint32_t id)
{
    auto ow = find(id);
    if (ow)
        windows_.erase(ow->inner_id);
}

std::optional<Window> Windows::find(uint32_t id) const
{
    auto it = windows_.find(id);
    if (it != windows_.end())
        return it->second;

    for (auto const& w: windows_)
        if (w.second.outer_id == id)
            return w.second;

    return {};
}

