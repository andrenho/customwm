theme = {}

function merge_theme(new_theme)

    local function merge_tables(original_tbl, new_tbl)

        if type(original_tbl) ~= 'table' or type(new_tbl) ~= 'table' then
            error('Expected table')
        end

        local function merge_keys(original, new)

            for k,v in pairs(new) do
                if type(v) == 'table' and original[k] then
                    merge_keys(original[k], v)
                else
                    original[k] = v
                end
            end

            return original
        end

        return merge_keys(original_tbl, new_tbl)

    end

    theme = merge_tables(theme, new_theme)
end

local function find_property(property, optional)
    if optional == nil then optional = false end
    local cur = theme
    for part in string.gmatch(property, "([^%.]+)") do
        if cur[part] == nil then
            if optional then
                return nil
            else
                error("Property " .. property .. " not found.")
            end
        end
        cur = cur[part]
    end
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