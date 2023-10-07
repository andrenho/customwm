return {
    metadata = {
        name = "Base WM",
        author = "Andre Wagner",
        link = {
            "https://github.com/andrenho/customwm"
        }
    },

    window = {
        padding = { 24, 3, 3, 3 },
        starting_pos = "random",
        on_expose = function(window, area)
            area = window:area()
            print(area.x, area.y, area.w, area.h)
        end
    }
}
