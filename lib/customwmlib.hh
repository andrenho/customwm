#ifndef CUSTOMWMLIB_HH_
#define CUSTOMWMLIB_HH_

#include <string>
#include "theme.hh"

class CustomWMLib {
public:
    CustomWMLib(int argc, char* argv[]);

    [[nodiscard]] std::string_view display() const { return display_; }

private:
    std::string display_;
    std::string theme_name_;
    Theme theme_;

    void read_args(int argc, char *argv[]);

    static void display_help(int exit_status);
};

#endif //CUSTOMWMLIB_HH_
