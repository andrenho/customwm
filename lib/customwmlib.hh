#ifndef CUSTOMWMLIB_HH_
#define CUSTOMWMLIB_HH_

#include <thread>
#include <string>

#include "theme.hh"

class CustomWMLib {
public:
    CustomWMLib(int argc, char* argv[]);

    void check_for_theme_reload();

    std::string const& display() const { return display_; }
    Theme const& theme() const { return theme_; }

private:
    std::string display_;
    std::string theme_name_;
    Theme       theme_;
    std::string current_theme_filename_;
    time_t      theme_file_last_modified_ = 0;

    void read_args(int argc, char *argv[]);

    static void display_help(int exit_status);

    void load_theme();
};

#endif //CUSTOMWMLIB_HH_
