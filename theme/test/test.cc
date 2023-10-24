#include "theme/theme.hh"

int main()
{
    Theme theme;

    theme.load_theme_code(R"(
        return {
            wm = {
                test = { 10, 20, 30 },
                aba = function() end,
            },
            x = 42,
            y = "hello"
        }
    )");

    theme.load_theme_code(R"(
        return {
            wm = {
                test = 'hello',
            },
            a = {},
        }
    )");

    theme.print_effective_theme();
}