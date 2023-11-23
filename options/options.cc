#include "options.hh"
#include "util/log.hh"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <getopt.h>

#include <string>
using namespace std::string_literals;

void Options::display_help(int exit_status)
{
    printf("    -b, --backend   Sets the backend (dummy, x11 or wayland, default: x11)\n");
    printf("    -r  --render    Sets the rendering library (xlib [for x11 only] or opengl\n");
    printf("    -d, --display   Sets the display (default: $DISPLAY)\n");
    printf("    -t, --theme     Load theme file\n");
    // printf("    -v, --verbose   Print debugging logs\n");
    // printf("    -D, --debug     Run in debugging mode (very slow)\n");
    // printf("    -x, --exception Throw naked exception on error\n");
    printf("    -h, --help      Prints this help\n");
    exit(exit_status);
}

Options::Options(int argc, char **argv)
{
    const char* display_env = getenv("DISPLAY");
    if (display_env)
        display = display_env;

    while (true) {
        static struct option long_options[] = {
                { "backend",   required_argument, nullptr, 'b' },
                { "render",    required_argument, nullptr, 'r' },
                { "display",   required_argument, nullptr, 'd' },
                { "theme",     required_argument, nullptr, 't' },
                { "verbose",   no_argument,       nullptr, 'v' },
                { "debug",     no_argument,       nullptr, 'D' },
                { "exception", no_argument,       nullptr, 'x' },
                { "help",      required_argument, nullptr, 'h' },
        };

        int idx;
        int c = getopt_long(argc, argv, "b:r:d:t:hvDx", long_options, &idx);
        if (c == -1)
            break;

        switch (c) {
            case 'b':
                if (strcmp(optarg, "dummy") == 0)
                    backend = Backend::Dummy;
                else if (strcmp(optarg, "wayland") == 0)
                    backend = Backend::Wayland;
                else if (strcmp(optarg, "x11") != 0)
                    throw std::runtime_error("Backend type `"s + optarg + "` not supported.");
                break;
            case 'r':
                if (strcmp(optarg, "opengl") == 0)
                    render = Render::OpenGL;
                else if (strcmp(optarg, "xlib") != 0)
                    throw std::runtime_error("Rendering type `"s + optarg + "` not supported.");
                break;
            case 'd':
                display = optarg;
                break;
            case 't':
                theme_file = optarg;
                break;
            case 'h':
                display_help(EXIT_SUCCESS);
                break;
            case 'x':
                throw_exceptions = true;
                break;
            case 'v':
                log_level = LogLevel::Debug;
                break;
            case 'D':
                debug_mode = true;
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
}
