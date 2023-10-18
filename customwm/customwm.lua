--
-- GLOBAL FUNCTIONS
--

function get_property(property, ...)
    local cur = theme
    for part in string.gmatch(property, "([^%.]+)") do
        if cur[part] == nil then error("Property " .. property .. " not found.") end
        cur = cur[part]
    end
    if type(cur) == "function" then
        return cur(...)
    else
        return cur
    end
end

local theme = {

    wm = {

        --
        -- PROPERTIES
        --

        padding = { top = 24, bottom = 3, left = 3, right = 3 },

        position_strategy = "cascade",   -- cascade, center, random, maximized, requested

        starting_location = function(outer_rect, screen_size)
            return { { 0, 0, 200, 200 }, { 50, 50 } }
            --[[
            local pos = window_starting_position(outer_rect, screen_size)
            local padding = get_property("wm.padding", outer_rect)
            local w = outer_rect.w + padding.left + padding.right + 1
            local h = outer_rect.h + padding.top + padding.bottom + 1
            return { { pos.x, pos.y, w, h }, { padding.left, padding.top } }
            ]]
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