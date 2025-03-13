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

function main(parts, config)
    local ser_parts = {}
    local mem_parts = {}
    
    if parts.text then table.insert(mem_parts, _combine_mem_parts("INS_ROM", parts.text)) end
    if parts.rodata then table.insert(mem_parts, _combine_mem_parts("DATA_ROM", parts.rodata)) end
    if parts.data then table.insert(mem_parts, _combine_mem_parts("DATA_RAM", parts.data)) end
    if parts.io then _combine_parts(mem_parts, parts.io) end

    table.insert(ser_parts, {name = config.cpu, file = config.cpu, type = "cpu"})
    if parts.text then _combine_parts(ser_parts, parts.text) end
    if parts.rodata then _combine_parts(ser_parts, parts.rodata) end
    if parts.data then _combine_parts(ser_parts, parts.data) end
    if parts.io then _combine_parts(ser_parts, parts.io) end
    if parts.io and not _have_input(parts.io) then
        table.insert(ser_parts, {name = "power_switch", file = "power_switch", type = "input"})
    end
    if parts.driver then _combine_parts(ser_parts, parts.driver) end

    return ser_parts, mem_parts
end