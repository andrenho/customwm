
theme = {
    resources = {
        fonts = {},
        images = {},
        slices = {},
    },
    wm = {},
}
__property_cache = {}


function merge_theme(new_theme)

    local function shallow_merge(first_table, second_table)
        if first_table == nil or second_table == nil then return end
        for k,v in pairs(second_table) do first_table[k] = v end
    end

    theme.super = deepcopy(theme)
    if new_theme.resources then
        shallow_merge(theme.resources.fonts, new_theme.resources.fonts)
        shallow_merge(theme.resources.images, new_theme.resources.images)
        shallow_merge(theme.resources.slices, new_theme.resources.slices)
    end
    if new_theme.wm then
        shallow_merge(theme.wm, new_theme.wm)
    end

end

local function find_property(property, optional)
    local cached_value = __property_cache[property]
    if cached_value ~= nil then return cached_value end

    local cur = theme
    for part in string.gmatch(property, "([^%.]+)") do
        if cur[part] == nil then
            if optional == nil or optional == true then
                return nil
            else
                error("Property " .. property .. " not found.")
            end
        end
        cur = cur[part]
    end

    __property_cache[property] = cur
    return cur
end


function getprop(key, ...)
    local cur = find_property(key, false)
    if type(cur) == "function" then
        return cur(...)
    else
        return cur
    end
end


function callopt(key, ...)
    local f = find_property(key, true)
    if f ~= nil then
        if type(f) ~= "function" then error("Expected function for property " .. property .. ".") end
        f(...)
    end
end
