local function _deepcopy(orig)
    local copy = {}
    for key, value in pairs(orig) do
        copy[key] = value
    end
    return copy
end

local function _convert_source_files(source_files, include_dirs)
    local include_files = {}
    for _, source_file in ipairs(source_files) do
        local source_file_content = io.readfile(source_file)
        for include_file in source_file_content:gmatch('#include%s+"(.-)"') do
            for _, include_dir in ipairs(include_dirs) do
                if os.isfile(include_dir .. "/" .. include_file) then
                    local include_match = false
                    for _, file in ipairs(include_files) do
                        if include_file == file then
                            include_match = true
                            break
                        end
                    end
                    if not include_match then
                        table.insert(include_files, include_file)
                    end
                end
            end
        end
    end
    return include_files
end

local function _convert_section(include_files, section_name, section_size, section_type, io_parts, text_size, rodata_size, have_input)
    if section_name and section_size and section_size ~= 0 and section_type then
        local include_match = false
        for _, include_file in ipairs(include_files) do
            if section_name == include_file then
                local io_part_name = section_name:gsub("%.h$", "")
                local io_part = {}
                io_part.name = io_part_name
                io_part.file = io_part_name
                io_part.section = section_name
                io_part.memory = string.upper(io_part_name)
                io_part.length = tonumber(section_size, 16)
                io_part.rwx = section_type:gsub("A", "r"):gsub("W", "w"):gsub("X", "x"):gsub("[^rwx]", "")
                if string.find(io_part.rwx, "w") then
                    io_part.type = "output"
                else
                    io_part.type = "input"
                    have_input = true
                end
                table.insert(io_parts, io_part)
                include_match = true
                -- print(string.format("include section: %s, size: 0x%s, type: %s", section_name, section_size, section_type))
                break
            end
        end
        if not include_match then
            if string.find(section_type, "X") then
                text_size = text_size + tonumber(section_size, 16)
                -- print(string.format("text section: %s, size: 0x%s, type: %s", section_name, section_size, section_type))
            elseif string.find(section_type, "A") and not (string.find(section_type, "W") or string.find(section_type, "X")) then
                rodata_size = rodata_size + tonumber(section_size, 16)
                -- print(string.format("rodata section: %s, size: 0x%s, type: %s", section_name, section_size, section_type))
            end
        end
    end
    return io_parts, text_size, rodata_size, have_input
end

local function _convert_object_files(object_files, include_files, bin_dir)
    local text_size = 0
    local rodata_size = 0
    local have_input = false
    local io_parts = {}

    for _, object_file in ipairs(object_files) do
        local object_file_content = os.iorunv(bin_dir .. "riscv-none-elf-readelf", {"-S", "--wide", object_file})
        for line in object_file_content:gmatch("[^\r\n]+") do
            local section_name, section_size, section_type = line:match("%s+%[%s*%d+%]%s+([%p%w]+)%s+[%p%w]+%s+%x+%s+%x+%s+(%x+)%s+%x+%s+([%w]+)")
            io_parts, text_size, rodata_size, have_input = _convert_section(include_files, section_name, section_size, section_type, io_parts, text_size, rodata_size, have_input)
        end
    end

    return io_parts, text_size, rodata_size, have_input
end

local function _parse_size(size_str)
    local num = tonumber(size_str:match("%d+"))
    local unit = size_str:match("%a")

    if not num then
        error("Invalid size format")
    end

    if unit == "K" then
        return num << 10
    elseif unit == "M" then
        return num << 20
    else
        return num
    end
end

local function _sort_by_length_asc(a, b)
    return a.length < b.length
end

local function _rename_part(part, name_counter)
    local copy_part = _deepcopy(part)
    if name_counter[part.name] then
        name_counter[part.name] = name_counter[part.name] + 1
        copy_part.name = part.name .. "_" .. name_counter[part.name]
    else
        name_counter[part.name] = 0
    end
    return copy_part
end

local function _assign_mem_addr(parts, origin)
    local origin_step = 64 << 10
    for i, part in ipairs(parts) do
        part.origin = origin
        origin = origin + math.ceil(part.length / origin_step) * origin_step
    end
    return origin
end

local function _get_mem_parts(dir, prefix, type, rwx, target_length, origin)
    local mem_files = os.files(dir .. "/" .. prefix .. "_*.TEditSch")
    local mem_parts = {}
    for _, mem_file in ipairs(mem_files) do
        local name = path.filename(mem_file):gsub("%.TEditSch", "")
        local size = name:match("_(%d+[KM]?)B")
        if size then
            table.insert(mem_parts, {name = name, file = name, type = type, rwx = rwx, length = _parse_size(size)})
        end
    end

    local parts = {}
    local used_length = 0
    local name_counter = {}

    table.sort(mem_parts, _sort_by_length_asc)
    while used_length < target_length do
        for i, part in ipairs(mem_parts) do
            if used_length + part.length >= target_length or i == #mem_parts then
                used_length = used_length + part.length
                table.insert(parts, _rename_part(part, name_counter))
                break
            end
        end
    end

    origin = _assign_mem_addr(parts, origin)

    return parts, origin
end

local function _combine_parts(as, bs)
    for _, b in ipairs(bs) do
        table.insert(as, b)
    end
end

local function _link_parts(memory, parts)
    if parts[1] then
        local length = 0
        for _, part in ipairs(parts) do
            length = length + part.length
        end
        return {memory = memory, rwx = parts[1].rwx, origin = parts[1].origin, length = length}
    else
        return {}
    end
end

function main(source_files, object_files, include_dirs, bin_dir, cpu, driver, ram)
    local parts = {}
    local link_parts = {}

    local origin = 0

    local text_parts = {}
    local rodata_parts = {}
    local data_parts = {}

    local include_files = _convert_source_files(source_files, include_dirs)
    local io_parts, text_size, rodata_size, have_input = _convert_object_files(object_files, include_files, bin_dir)
    local data_size = _parse_size(ram)
    local have_data_rom = rodata_size >= data_size / 2

    -- print(string.format("text size: %d bytes", text_size))
    -- print(string.format("rodata size: %d bytes", rodata_size))

    text_parts, origin = _get_mem_parts("hardware/wiring/memory/ins", "ins_rom", "mem_ins", "x", text_size, origin)
    if have_data_rom then rodata_parts, origin = _get_mem_parts("hardware/wiring/memory/data", "data_rom", "mem_data", "r", rodata_size, origin) end
    data_parts, origin = _get_mem_parts("hardware/wiring/memory/data", "data_ram", "mem_data", "rw", data_size, origin)
    origin = _assign_mem_addr(io_parts, origin)
    
    table.insert(link_parts, _link_parts("INS_ROM", text_parts))
    if have_data_rom then table.insert(link_parts, _link_parts("DATA_ROM", rodata_parts)) end
    table.insert(link_parts, _link_parts("DATA_RAM", data_parts))
    _combine_parts(link_parts, io_parts)

    table.insert(parts, {name = cpu, file = cpu, type = "cpu"})
    _combine_parts(parts, text_parts)
    if have_data_rom then _combine_parts(parts, rodata_parts) end
    _combine_parts(parts, data_parts)
    _combine_parts(parts, io_parts)
    if not have_input then table.insert(parts, {name = "power_switch", file = "power_switch", type = "input"}) end
    table.insert(parts, {name = driver, file = driver, type = "driver"})

    return parts, link_parts
end