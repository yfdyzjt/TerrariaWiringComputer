function main(dir, prefix, type, rwx, target_length)
    import("part_utils")

    local mem_files = os.files(dir .. "/" .. prefix .. "_*.TEditSch")
    local mem_parts = {}

    for _, mem_file in ipairs(mem_files) do
        local name = path.filename(mem_file):gsub("%.TEditSch", "")
        local size = name:match("_([%d%.]+[KkMm]?)B$")
        if size then
            table.insert(mem_parts, {
                name = name, 
                file = name, 
                type = type, 
                rwx = rwx, 
                length = part_utils.parse_size(size)
            })
        end
    end

    local parts = {}
    local used_length = 0
    local name_counter = {}

    table.sort(mem_parts, function(a, b) return a.length < b.length end)
    while used_length < target_length do
        for i, part in ipairs(mem_parts) do
            if used_length + part.length >= target_length or i == #mem_parts then
                used_length = used_length + part.length
                table.insert(parts, part_utils.rename_part(part, name_counter))
                break
            end
        end
    end

    return parts
end