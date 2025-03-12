local function _combine_parts(as, bs)
    for _, b in ipairs(bs) do
        table.insert(as, b)
    end
end

local function _combine_mem_parts(memory, parts)
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

local function _have_input(io_parts)
    for _, part in ipairs(io_parts) do
        if part.type == "input" then
            return true
        end
    end
    return false
end

function main(text_parts, rodata_parts, data_parts, io_parts, driver_parts, config)
    local parts = {}
    local mem_parts = {}
    
    table.insert(mem_parts, _combine_mem_parts("INS_ROM", text_parts))
    table.insert(mem_parts, _combine_mem_parts("DATA_ROM", rodata_parts))
    table.insert(mem_parts, _combine_mem_parts("DATA_RAM", data_parts))
    _combine_parts(mem_parts, io_parts)

    table.insert(parts, {name = config.cpu, file = config.cpu, type = "cpu"})
    _combine_parts(parts, text_parts)
    _combine_parts(parts, rodata_parts)
    _combine_parts(parts, data_parts)
    _combine_parts(parts, io_parts)
    if not _have_input(io_parts) then table.insert(parts, {name = "power_switch", file = "power_switch", type = "input"}) end
    _combine_parts(parts, driver_parts)

    return parts, mem_parts
end