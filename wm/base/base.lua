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
            -- window:draw_rectangles({ { 0, 5, 10, 15 }, { 0, 40, 10, 15 } }, "#ffab12", true)
            window:draw_line({ 0, 0 }, { 20, 20 }, "#ff0000")
        end
    }
}
