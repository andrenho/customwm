#ifndef WM_HH_
#define WM_HH_

#include <memory>

#include "../../lib/customwmlib.hh"
#include "windows.hh"
#include "../graphlib/graphiclibrary.hh"

class WM : public WM_Events {
public:
    explicit WM(CustomWMLib const& lib) : lib_(lib) {}

    void run();

    void on_create_window(Handle window_id) override;

    void on_destroy_window(Handle window_id) override;

    void on_expose_window(Handle window_id, Area area) override;

protected:
    Point window_starting_pos(Window const& w, Area const& window_sz, Area const& scr_sz, Padding const& pad) const;

    CustomWMLib const& lib_;
    Windows windows_;
    std::unique_ptr<GraphicLibrary> graph_;

    mutable uint8_t cascade_ = 0;
};

#endif //WM_HH_
