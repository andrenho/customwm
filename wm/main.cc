#include "../lib/customwmlib.hh"

#include <cstdio>
#include <variant>

int main(int argc, char* argv[])
{
    CustomWMLib lib(argc, argv);
    printf("%d\n", std::get<int>(lib.theme().window_border_width()));
}