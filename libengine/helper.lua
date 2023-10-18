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