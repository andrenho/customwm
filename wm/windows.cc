#include "windows.hh"

void Windows::add(const Window &w)
{
    windows_[w.outer_id] = w;
}

void Windows::remove(uint32_t id)
{
    auto ow = find(id);
    if (ow)
        windows_.erase(ow->outer_id);
}

std::optional<Window> Windows::find(uint32_t id) const
{
    auto it = windows_.find(id);
    if (it != windows_.end())
        return it->second;

    for (auto const& w: windows_)
        if (w.second.inner_id == id)
            return w.second;

    return {};
}

