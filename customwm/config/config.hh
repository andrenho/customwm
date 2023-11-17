#ifndef CONFIG_HH_
#define CONFIG_HH_

#include <optional>
#include <string>

class Config {
public:
    void init(int argc, char* argv[]);

    std::optional<std::string> display {};
    std::optional<std::string> theme_file {};

    bool throw_exceptions = false;

private:
    static void display_help(int exit_status);
};

#endif //CONFIG_HH_
