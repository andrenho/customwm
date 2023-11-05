#ifndef OPTIONS_HH_
#define OPTIONS_HH_

#include <optional>
#include <string>

class Options {
public:
    Options(int argc, char* argv[]);

    std::optional<std::string> display {};
    std::optional<std::string> theme_file {};

    bool throw_exceptions = false;

private:
    static void display_help(int exit_status);
};

#endif //OPTIONS_HH_
