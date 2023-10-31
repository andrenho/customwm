local __cascade = 0

-- calculate the window starting position { x, y } based on properties provided by the user

__window_metatable = {

    -- based on the text properties, draw text on the screen
    window_draw_text = function(window, x, y, text, properties)
        properties = properties or {}
        properties.color = properties.color or "#000000"
        properties.font = properties.font or "basic"
        window:_draw(x, y, text, properties.font, properties.color)
    end

}

local theme = {

    resources = {

        fonts = {
            basic = { "helvetica-18" },
        },

        images = {
            main = { filename = "customwm/customwm.png" },
        },

        slices = {
            x = { image = 'main', rect = { x=0, y=0, w=24, h=24 } },
        },

    },

    wm = {

        --
        -- PROPERTIES
        --

        padding = { top = 100, bottom = 3, left = 3, right = 3 },

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
            window:fill("#ffdfff")
            window:text(24, 24, "Hello world!", {
                font = "basic", color = "#ff0000",
                w = 60,
            })
            window:draw(0, 0, "x")
        end,
    }

}

return theme
