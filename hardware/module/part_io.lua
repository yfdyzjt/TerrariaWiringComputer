function main(sections)
    local io_parts = {}

    for _, section in ipairs(sections) do
        if string.find(section.name, "%.h$", 1, false) then
            local io_part_name = section.name:gsub("%.h$", "")
            local input_file = "./hardware/wiring/input/" .. io_part_name .. ".TEditSch"
            local output_file = "./hardware/wiring/output/" .. io_part_name .. ".TEditSch"
            if os.isfile(input_file) or os.isfile(output_file) then
                local io_part = {}

                io_part.name = io_part_name
                io_part.file = io_part_name
                io_part.section = section.name
                io_part.memory = string.upper(io_part_name)
                io_part.length = section.size
                io_part.rwx = section.type:gsub("A", "r"):gsub("W", "w"):gsub("X", "x"):gsub("[^rwx]", "")

                if string.find(io_part.rwx, "w") then
                    io_part.type = "output"
                else
                    io_part.type = "input"
                end

                table.insert(io_parts, io_part)
                -- print(string.format("include section: %s, size: 0x%s, type: %s", section.name, section.size, section.type))
            end
        end
    end

    return io_parts
end