#ifndef CUSTOMWMLIB_HH_
#define CUSTOMWMLIB_HH_

#include <string>
#include "theme.hh"

class CustomWMLib {
public:
    CustomWMLib(int argc, char* argv[]);

    std::string const& display() const { return display_; }

    Theme const& theme() const { return theme_; }

    void reload_theme();

private:
    std::string display_;
    std::string theme_name_;
    Theme theme_;

    void read_args(int argc, char *argv[]);

    static void display_help(int exit_status);

    void load_theme();
};

#endif //CUSTOMWMLIB_HH_
