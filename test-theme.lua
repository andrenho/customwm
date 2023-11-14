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
            h.maximize = {
                area = { x = window:rect().w - 35, y = 2, w = 13, h = 13 }
            }
            h.close = {
                area = { x = window:rect().w - 15, y = 2, w = 13, h = 13 },
                cursor = "kill"
            }
            return h
        end,

        on_hotspot_click = function(window, hotspot, ev)
            callopt("super.wm.on_hotspot_click", window, hotspot, ev)
            if hotspot == "close" then
                window:close()
            elseif hotspot == "maximize" then
                window:maximize()
            end
        end,

        on_expose = function(window, exposed_area)
            callopt("super.wm.on_expose", window, exposed_area)
            window:fill("#4444ff", getprop("wm.hotspots", window).maximize.area)
            window:fill("#ff0000", getprop("wm.hotspots", window).close.area)
        end,

    },

}