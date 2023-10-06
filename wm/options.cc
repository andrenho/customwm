#include "options.hh"

#include <cstdio>
#include <cstdlib>
#include <getopt.h>

void Options::display_help(int exit_status)
{
    printf("    -d, --display   Sets the display (default: $DISPLAY)\n");
    printf("    -t, --theme     Sets the theme (default: default)\n");
    printf("    -h, --help      Prints this help\n");
    exit(EXIT_FAILURE);
}

Options::Options(int argc, char **argv)
{
    const char* display_env = getenv("DISPLAY");
    if (display_env)
        display = display_env;

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
                display = optarg;
                break;
            case 't':
                theme_file = optarg;
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
