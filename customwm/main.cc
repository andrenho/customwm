#include <cstdio>
#include <cstdlib>
#include <getopt.h>

#include <memory>
#include <optional>
#include <string>

#include "customwm.hh"
#include "../libengine/engine.hh"

struct Options {
    std::optional<std::string> display {};
    std::optional<std::string> theme_file {};
};

static void display_help(int exit_status)
{
    printf("    -d, --display   Sets the display (default: $DISPLAY)\n");
    printf("    -t, --theme     Sets the theme (default: default)\n");
    printf("    -h, --help      Prints this help\n");
    exit(exit_status);
}

static Options get_options(int argc, char* argv[])
{
    Options opt;

    const char* display_env = getenv("DISPLAY");
    if (display_env)
        opt.display = display_env;

    while (true) {
        static struct option long_options[] = {
                { "display", required_argument, nullptr, 'd' },
                { "theme", required_argument, nullptr, 't' },
                { "help", required_argument, nullptr, 'h' },
        };

        int idx;
        int c = getopt_long(argc, argv, "d:t:h", long_options, &idx);
        if (c == -1)
            break;

        switch (c) {
            case 'd':
                opt.display = optarg;
                break;
            case 't':
                opt.theme_file = optarg;
                break;
            case 'h':
                display_help(EXIT_SUCCESS);
                break;
            case '?':
                display_help(EXIT_FAILURE);
                break;
            default:
                abort();
        }

    }

    if (optind < argc)
        display_help(EXIT_FAILURE);

    return opt;
}

int main(int argc, char* argv[])
{
    Options options = get_options(argc, argv);

    Engine engine;
    engine.load_from_memory(customwm, customwm_len);

    if (options.theme_file)
        engine.override_with_theme_file(*options.theme_file);

    std::unique_ptr<Root> root;
    try {
        root = Root::build(options.display);
        printf("Backend: %s\n", root->interface_name().c_str());
    } catch (std::exception& e) {
        fprintf(stderr, "Error initializing customwm: %s\n", e.what());
        exit(EXIT_FAILURE);
    }
}