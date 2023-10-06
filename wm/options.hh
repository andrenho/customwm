#ifndef OPTIONS_HH_
#define OPTIONS_HH_

#include <string>

class Options {
public:
    Options(int argc, char* argv[]);

    std::optional<std::string> display {};
    std::optional<std::string> theme_file {};

private:
    static void display_help(int exit_status);
};

#endif //OPTIONS_HH_
