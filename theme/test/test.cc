#include "theme/theme.hh"

int main()
{
    Theme theme;

    theme.load_theme_code(R"(
        return {
            wm = {
                test = { 10, 20, 30 },
                dbl = function(x) return x * 2 end,
            },
            x = 42,
            y = "hello"
        }
    )");

    theme.load_theme_code(R"(
        return {
            wm = {
                test = 'hello',
                hello = function() print('Hello world!') end,
            },
            a = {},
        }
    )");

    theme.print_effective_theme();

    printf("%d\n", theme.get_prop<int>("wm.dbl", 30));
    theme.load_theme_code("return { wm = { dbl = 50 } }");
    printf("%d\n", theme.get_prop<int>("wm.dbl", 30));

    theme.call_opt("wm.hello");
}