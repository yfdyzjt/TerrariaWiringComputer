function sections_size(sections)
    local size = {}

    size.text = 0
    size.data = 0
    size.rodata = 0

    for _, section in ipairs(sections) do
        if section.name == ".text" then
            size.text = size.text + section.size
        elseif section.name == ".data" then
            size.data = size.data + section.size
        elseif section.name == ".bss" then
            size.data = size.data + section.size
        elseif section.name == ".rodata" then
            size.rodata = size.rodata + section.size
        end
    end

    return size
end

function parse_size(size_str)
    local num = tonumber(size_str:match("([%d%.]+)"))
    local unit = size_str:match("%a")

    if not num then
        raise("Invalid size format")
    end

    if unit == "K" or unit == "k" then
        return num * 1024
    elseif unit == "M" or unit == "m" then
        return num * 1024 * 1024
    else
        return num
    end
end

function parse_freq(freq_str)
    local num = tonumber(freq_str:match("([%d%.]+)"))
    local unit = freq_str:match("%a")

    if not num then
        raise("Invalid freq format")
    end

    if unit == "K" or unit == "k" then
        return num * 1000
    elseif unit == "M" or unit == "m" then
        return num * 1000 * 1000
    else
        return num
    end
end

function assign_mem_addr(parts, origin)
    local origin_step = 64 << 10

    for i, part in ipairs(parts) do
        part.origin = origin
        origin = origin + math.ceil(part.length / origin_step) * origin_step
    end

    return origin
end

function deepcopy(orig)
    local copy = {}
    for key, value in pairs(orig) do
        copy[key] = value
    end
    return copy
end

function rename_part(part, name_counter)
    local copy_part = deepcopy(part)
    if name_counter[part.name] then
        name_counter[part.name] = name_counter[part.name] + 1
        copy_part.name = part.name .. "_" .. name_counter[part.name]
    else
        name_counter[part.name] = 0
    end
    return copy_part
end