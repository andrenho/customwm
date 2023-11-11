return {

    resources = {

        images = {
            main = { filename = "customwm/customwm.png" },
        },

        slices = {
            x = { image = 'main', rect = { x=0, y=0, w=24, h=24 } },
        },

    },

    wm = {

        hotspots = function(window)
            local h = getprop("super.wm.hotspots", window)
            h.close = {
                area = { x = window:rect().w - 15, y = 2, w = 13, h = 13 },
                cursor = "kill"
            }
            return h
        end,

        on_hotspot_click = function(window, hotspot, ev)
            callopt("super.wm.on_hotspot_click", window, hotspot, ev)
            if hotspot == 'close' then
                wm:close_window(window)
            end
        end,

    },

}