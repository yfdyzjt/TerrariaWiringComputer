function main(target_freq)
    import("part_utils")

    local driver_files = os.files("hardware/wiring/driver/*.TEditSch")
    local driver_parts = {}

    for _, driver_file in ipairs(driver_files) do
        local name = path.filename(driver_file):gsub("%.TEditSch", "")
        local freq = name:match("_([%d%.]+[KkMm]?)Hz$")
        
        if freq then
            table.insert(driver_parts, {
                name = name,
                file = name,
                type = "driver",
                freq = part_utils.parse_freq(freq)
            })
        end
    end

    local parts = {}
    local used_freq = 0
    local name_counter = {}

    table.sort(driver_parts, function(a, b) return a.freq > b.freq end)
    while used_freq < target_freq do
        local best_part = driver_parts[#driver_parts]
        for _, part in ipairs(driver_parts) do
            if part.freq <= (target_freq - used_freq) then
                best_part = part
                break
            end
        end
        
        used_freq = used_freq + best_part.freq
        table.insert(parts, part_utils.rename_part(best_part, name_counter))
    end

    return parts
end