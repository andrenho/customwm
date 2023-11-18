#ifndef WINDOWMANAGER_HH_
#define WINDOWMANAGER_HH_

#include <memory>
#include <unordered_map>

#include "graphics/whandle.hh"
#include "window/parentwindow.hh"

class WindowManager {
public:
    explicit WindowManager(class Theme* theme, class Graphics* graphics) : theme_(theme), graphics_(graphics) {}

    void init();

    void add_child_window(WindowHandle child_handle);
    void remove_window(WindowHandle window_handle);

    static void create_lua_metatable(class Theme* theme);

private:
    class Theme* theme_;
    class Graphics* graphics_;

    ParentWindow* find_parent(WindowHandle parent_handle) const;

    std::unordered_map<WindowHandle, std::unique_ptr<ParentWindow>> parents_;
};

#endif //WINDOWMANAGER_HH_
