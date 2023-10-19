local cascade_ = 0

local function window_starting_position(outer_rect, screen_size)
    local strategy = get_property("wm.position_strategy")
    if type(strategy) == "table" then
        return { x = outer_rect.x, y = outer_rect.y }
    elseif strategy == "cascade" then
        local x = cascade_ * 24
        local y = cascade_ * 24
        cascade_ = cascade_ + 1
        if cascade_ == 5 then cascade_ = 0 end
        return { x=x, y=y }
    elseif strategy == "center" then
        return {
            x = (screen_size.w / 2) - (outer_rect.w / 2),
            y = (screen_size.h / 2) - (outer_rect.h / 2),
        }
    elseif strategy == "random" then
        return { x = math.random(screen_size.w // 3), y = math.random(screen_size.h // 3) }
    elseif strategy == "maximized" then
        error("Not implemented")
    elseif strategy == "requested" then
        return { x = outer_rect.x, y = outer_rect.y }
    else
        error("Invalid value '"..strategy.."' for wm.position_strategy.")
    end
end

local theme = {

    wm = {

        --
        -- PROPERTIES
        --

        padding = { top = 24, bottom = 3, left = 3, right = 3 },

        position_strategy = "center",   -- cascade, center, random, maximized, requested

        starting_location = function(outer_rect, screen_size)
            local pos = window_starting_position(outer_rect, screen_size)
            local padding = get_property("wm.padding", outer_rect)
            local w = outer_rect.w + padding.left + padding.right + 1
            local h = outer_rect.h + padding.top + padding.bottom + 1
            return {
                { x = pos.x, y = pos.y, w = w, h = h },
                { w = padding.left, h = padding.top }
            }
        end,

        --
        -- EVENTS (overwriteable)
        --

        after_start = function()
            print("Started " .. root:interface_name() .. " backend.")
        end,

        after_window_registered = function(window)
            print("Window registered: ", window.outer_id, window.child_id)
        end,

        after_window_unregistered = function(window)
            print("Window unregistered: ", window.outer_id, window.child_id)
        end,

        --
        -- INTERNAL FUNCTIONS
        --

        __windows = {},

        __register_window = function(outer_id, child_id)
            local window = {
                outer_id = outer_id,
                child_id = child_id,
            }
            wm.__windows[outer_id] = window
            if wm.after_window_registered then wm.after_window_registered(window) end
        end,

        __unregister_window = function(outer_id)
            local window = wm.__windows[outer_id]
            if window then
                wm.__windows[outer_id] = nil
                if wm.after_window_unregistered then wm.after_window_unregistered(window) end
            end
        end,

        __find_window_by_child_id = function(child_id)
            for _, w in pairs(wm.__windows) do
                if w.child_id == child_id then return w end
            end
            return nil
        end,

    }

}

wm = theme.wm

return theme