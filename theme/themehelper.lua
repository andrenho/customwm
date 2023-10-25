theme = {}

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


function merge_theme(new_theme)
    theme = merge_tables(theme, new_theme)
end


function getprop(key, ...)
    if type(key) == 'function' then
        return key(...)
    else
        return key
    end
end