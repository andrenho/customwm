
local __cascade = 0

local theme = {

    resources = {

        fonts = {
            basic = { "helvetica-18" },
        },

    },

    wm = {

        --
        -- PROPERTIES
        --

        padding = { top = 24, bottom = 2, left = 2, right = 2 },

        resize_border = 3,

        position_strategy = "cascade",   -- cascade, center, random, maximized, requested

        window_starting_location = function(child_rect, screen_size)

            local function window_starting_position(child_rect_, screen_size_)
                local strategy = getprop("wm.position_strategy")
                if type(strategy) == "table" then
                    return { x = child_rect_.x, y = child_rect_.y }
                elseif strategy == "cascade" then
                    local x = __cascade * 24
                    local y = __cascade * 24
                    __cascade = __cascade + 1
                    if __cascade == 5 then __cascade = 0 end
                    return { x=x, y=y }
                elseif strategy == "center" then
                    return {
                        x = (screen_size_.w / 2) - (child_rect_.w / 2),
                        y = (screen_size_.h / 2) - (child_rect_.h / 2),
                    }
                elseif strategy == "random" then
                    return { x = math.random(screen_size_.w / 3), y = math.random(screen_size_.h / 3) }
                elseif strategy == "maximized" then
                    error("Not implemented")
                elseif strategy == "requested" then
                    return { x = child_rect_.x, y = child_rect_.y }
                else
                    error("Invalid value '"..(strategy or 'nil').."' for wm.position_strategy.")
                end
            end

            local starting_pos = window_starting_position(child_rect, screen_size)
            local padding = getprop("wm.padding", child_rect, screen_size)
            local w = child_rect.w + padding.left + padding.right + 1
            local h = child_rect.h + padding.top + padding.bottom + 1
            return {
                parent_rect = { x = starting_pos.x, y = starting_pos.y, w = w, h = h },
                child_offset = { x = padding.left, y = padding.top }
            }
        end,

        hotspots = function(window)
            local r = window:rect()
            local border = getprop("wm.resize_border", window)
            return {
                title = {
                    area = { x = border, y = border, w = r.w - (border * 2), h = getprop("wm.padding", window).top - border },
                    cursor = "pointer",
                    grab = "move",
                },
                top_left = {
                    area = { x = 0, y = 0, w = border, h = border },
                    cursor = "top-left",
                    grab = "top-left",
                },
                top = {
                    area = { x = border, y = 0, w = r.w - (border*2), h = border },
                    cursor = "top",
                    grab = "top",
                },
                top_right = {
                    area = { x = r.w - border, y = 0, w = border, h = border },
                    cursor = "top-right",
                    grab = "top",
                },
                left = {
                    area = { x = 0, y = border, w = border, h = r.h - (border*2) },
                    cursor = "left",
                    grab = "top",
                },
                right = {
                    area = { x = r.w - border, y = border, w = border, h = r.h - (border*2) },
                    cursor = "right",
                    grab = "top",
                },
                bottom_left = {
                    area = { x = 0, y = r.h - border, w = border, h = border },
                    cursor = "bottom-left",
                    grab = "top",
                },
                bottom_right = {
                    area = { x = r.w - border, y = r.h - border, w = border, h = border },
                    cursor = "bottom-right",
                    grab = "top",
                },
                bottom = {
                    area = { x = border, y = r.h - border, w = r.w - (border * 2), h = border },
                    cursor = "bottom",
                    grab = "top",
                },
            }
        end,

        --
        -- EVENTS (overrideable)
        --

        after_start = function()
            print("Started " .. wm:interface_name() .. " backend.")
        end,

        after_window_registered = function(window)
            print("Window registered: ", window:id())
        end,

        on_window_unregistered = function(window)
            print("Window unregistered: ", window:id())
        end,

        on_expose = function(window, exposed_area)
            if window:focused() then
                window:fill("#a0a0ff")
            else
                window:fill("#a0a0a0")
            end
            window:text(0, 0, window:name(), {
                font = "basic", w = window:rect().w, h = 24, halign = "center", valign = "center"
            })
            window:fill("#ff0000", getprop("wm.hotspots", window).close.area)
        end,

        on_window_click = function(window, ev)
            if ev.button == 'left' and not ev.pressed then
                print('release')
                wm:grab(window, nil)
            end
        end,

        on_desktop_click = function(ev)
        end,

        on_hotspot_click = function(window, hotspot, ev)
            local hs = getprop("wm.hotspots", window)[hotspot]
            if hs and hs.grab and ev.pressed and ev.button == 'left' then
                print('grab')
                wm:grab(window, hs.grab)
            end
        end,

        on_window_move_pointer = function(window, pos)
        end,

        on_move_pointer = function(pos)
        end,

        on_enter_hotspot = function(window, hotspot)
            print('Entering hotspot ' .. hotspot)
        end,

        on_leave_hotspot = function(window, hotspot)
            print('Leaving hotspot ' .. hotspot)
        end,
    }

}

merge_theme(theme)