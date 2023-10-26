local __cascade = 0

local function window_starting_position(child_rect, screen_size)
    local strategy = getprop("wm.position_strategy")
    if type(strategy) == "table" then
        return { x = child_rect.x, y = child_rect.y }
    elseif strategy == "cascade" then
        local x = __cascade * 24
        local y = __cascade * 24
        __cascade = __cascade + 1
        if __cascade == 5 then __cascade = 0 end
        return { x=x, y=y }
    elseif strategy == "center" then
        return {
            x = (screen_size.w / 2) - (child_rect.w / 2),
            y = (screen_size.h / 2) - (child_rect.h / 2),
        }
    elseif strategy == "random" then
        return { x = math.random(screen_size.w / 3), y = math.random(screen_size.h / 3) }
    elseif strategy == "maximized" then
        error("Not implemented")
    elseif strategy == "requested" then
        return { x = child_rect.x, y = child_rect.y }
    else
        error("Invalid value '"..(strategy or 'nil').."' for wm.position_strategy.")
    end
end

local theme = {

    wm = {

        --
        -- PROPERTIES
        --

        padding = { top = 24, bottom = 3, left = 3, right = 3 },

        position_strategy = "cascade",   -- cascade, center, random, maximized, requested

        window_starting_location = function(child_rect, screen_size)
            local starting_pos = window_starting_position(child_rect, screen_size)
            local padding = getprop("wm.padding", child_rect, screen_size)
            local w = child_rect.w + padding.left + padding.right + 1
            local h = child_rect.h + padding.top + padding.bottom + 1
            return {
                parent_rect = { x = starting_pos.x, y = starting_pos.y, w = w, h = h },
                child_offset = { x = padding.left, y = padding.top }
            }
        end,

        --
        -- EVENTS (overrideable)
        --

        after_start = function()
            print("Started " .. wm:interface_name() .. " backend.")
        end,

        after_window_registered = function(window)
            print("Window registered: ", window)
        end,

        after_window_unregistered = function(window)
            print("Window unregistered: ", window)
        end,

        on_expose = function(window, exposed_area)
            window:fill("#ff0000")
        end,
    }

}

return theme