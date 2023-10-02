#include "customwmlib.hh"
#include "exceptions.hh"

#include <cstdio>
#include <getopt.h>

CustomWMLib::CustomWMLib(int argc, char **argv)
    : display_(getenv("DISPLAY")), theme_name_("default"), theme_()
{
    read_args(argc, argv);
    load_theme();

    watch_theme_ = std::thread([](){});  // TODO
}

void CustomWMLib::load_theme()
{
    try {
        theme_.load(theme_name_);
    } catch (LuaException& e) {
        fprintf(stderr, "lua error: %s", e.what());
        exit(EXIT_FAILURE);
    }
}

void CustomWMLib::reload_theme()
{
    theme_.reset();
    load_theme();
}

void CustomWMLib::read_args(int argc, char **argv)
{
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
                display_ = optarg;
                break;
            case 't':
                theme_name_ = optarg;
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

        if (optind < argc)
            display_help(EXIT_FAILURE);
    }
}

void CustomWMLib::display_help(int exit_status)
{
    printf("    -d, --display   Sets the display (default: $DISPLAY)\n");
    printf("    -t, --theme     Sets the theme (default: default)\n");
    printf("    -h, --help      Prints this help\n");
    exit(EXIT_FAILURE);
}

