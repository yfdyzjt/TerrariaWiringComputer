local function _get_include_files(source_files, include_dirs)
    local include_files = {}

    for _, source_file in ipairs(source_files) do
        local source_file_content = io.readfile(source_file)

        for include_file in source_file_content:gmatch('#include%s+"(.-)"') do
            for _, include_dir in ipairs(include_dirs) do
                if os.isfile(include_dir .. "/" .. include_file) then
                    for _, file in ipairs(include_files) do
                        if include_file == file then
                            goto match
                        end
                    end

                    table.insert(include_files, include_file)
                    :: match ::
                end
            end
        end
    end

    return include_files
end

local function _get_io_parts(object_files, include_files, sdk_program)
    import("readelf")

    local io_parts = {}

    for _, object_file in ipairs(object_files) do
        local sections = readelf(sdk_program, object_file)

        for _, section in ipairs(sections) do
            for _, include_file in ipairs(include_files) do
                if section.name == include_file then
                    local io_part = {}
                    local io_part_name = section.name:gsub("%.h$", "")

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
    end

    return io_parts
end

local function _parse_size(size_str)
    local num = tonumber(size_str:match("%d+"))
    local unit = size_str:match("%a")

    if not num then
        print("Invalid size format")
        error()
    end

    if unit == "K" then
        return num << 10
    elseif unit == "M" then
        return num << 20
    else
        return num
    end
end

local function _assign_mem_addr(parts, origin)
    local origin_step = 64 << 10

    for i, part in ipairs(parts) do
        part.origin = origin
        origin = origin + math.ceil(part.length / origin_step) * origin_step
    end

    return origin
end

local function _sort_by_length_asc(a, b)
    return a.length < b.length
end

local function _deepcopy(orig)
    local copy = {}
    for key, value in pairs(orig) do
        copy[key] = value
    end
    return copy
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

local function _get_mem_parts(dir, prefix, type, rwx, target_length)
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

    return parts
end

local function _get_ins_mem(size, origin)
    local parts = _get_mem_parts("hardware/wiring/memory/ins", "ins_rom", "mem_ins", "x", size)
    local origin = _assign_mem_addr(parts, origin)
    return parts, origin
end

local function _get_rodata_mem(size, origin)
    local parts = _get_mem_parts("hardware/wiring/memory/data", "data_rom", "mem_data", "r", size)
    local origin = _assign_mem_addr(parts, origin)
    return parts, origin
end

local function _get_data_mem(size, origin)
    local parts = _get_mem_parts("hardware/wiring/memory/data", "data_ram", "mem_data", "rw", size)
    local origin = _assign_mem_addr(parts, origin)
    return parts, origin
end

local function _get_io_mem(object_files, include_files, sdk_program, origin)
    local parts = _get_io_parts(object_files, include_files, sdk_program)
    local origin = _assign_mem_addr(parts, origin)
    return parts, origin
end

function main(target, size, config, sdk_program)
    local source_files = os.files("software/src/" .. config.software .. "/**.c")
    local object_files = target:objectfiles()
    local include_files = _get_include_files(source_files, target:get("includedirs"))
    local target_data_size = _parse_size(config.ram)

    local text_parts = {}
    local rodata_parts = {}
    local data_parts = {}
    local io_parts = {}

    local origin = 0

    if size.data > target_data_size then print("ram cannot hold data.") error() end
    text_parts, origin = _get_ins_mem(size.text, origin)
    local have_data_rom = size.rodata >= target_data_size / 2 and size.data < target_data_size / 2
    if have_data_rom then rodata_parts, origin = _get_rodata_mem(size.rodata, origin) end
    data_parts, origin = _get_data_mem(target_data_size, origin)
    io_parts, origin = _get_io_mem(object_files, include_files, sdk_program, origin)

    return text_parts, rodata_parts, data_parts, io_parts
end