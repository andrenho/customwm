local theme = {

    wm = {

        --
        -- PROPERTIES
        --

        starting_location = function(outer_rect)
            -- TODO
            return { { 0, 0, 400, 400 }, { 50, 50 } }
        end,

        --
        -- EVENTS
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