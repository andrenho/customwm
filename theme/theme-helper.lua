function merge_theme(new_theme)

    function merge_child(old, new)
        if not old or not new then return end
        for k,v in pairs(old) do new[k] = v end
    end

    merge_child(theme.wm, new_theme.wm)

end