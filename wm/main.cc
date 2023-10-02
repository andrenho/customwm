#include "wm/wm.hh"

#include <ctime>
#include <cstdio>
#include <cstdlib>

#include <getopt.h>

static void display_help(int exit_status)
{
    printf("    -d, --display   Sets the display (default: $DISPLAY)\n");
    printf("    -t, --theme     Sets the theme (default: default)\n");
    printf("    -h, --help      Prints this help\n");
    exit(EXIT_FAILURE);
}

static void read_args(int argc, char **argv, std::string& display_name, std::string& theme_name)
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
                display_name = optarg;
                break;
            case 't':
                theme_name = optarg;
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


int main(int argc, char* argv[])
{
    srand(time(nullptr));

    std::string display_name = ":0",
                theme_name = "default";
    read_args(argc, argv, display_name, theme_name);

    WM(display_name, theme_name).run();
}