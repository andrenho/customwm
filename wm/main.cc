#include "options.hh"

#include <cstdio>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[])
{
    srand(time(nullptr));

    Options options(argc, argv);
    if (!options.display) {
        fprintf(stderr, "%s: no display is set\n", argv[0]);
        exit(EXIT_FAILURE);
    }

}