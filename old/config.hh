#ifndef CONFIG_HH_
#define CONFIG_HH_

#include <string>

struct Config {
    bool        debug_messages;
    std::string display_name;

    Config(int argc, char* argv[]);
};

#endif //CONFIG_HH_
