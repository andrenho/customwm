#ifndef OPTIONS_HH_
#define OPTIONS_HH_

#include <optional>
#include <string>

class Options {
public:
    enum class Backend { Dummy, X11, Wayland };
    enum class Render  { Xlib, OpenGL };

    Options(int argc, char* argv[]);

    Backend backend = Backend::X11;
    Render  render = Render::Xlib;

    std::optional<std::string> display {};
    std::optional<std::string> theme_file {};

    bool throw_exceptions = false;
    bool debug_mode = false;

private:
    static void display_help(int exit_status);
};

#endif //OPTIONS_HH_
