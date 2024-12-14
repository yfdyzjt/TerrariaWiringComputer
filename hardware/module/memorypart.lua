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

function main(parts)
    local memory_parts = {}

    table.insert(link_parts, _link_parts("INS_ROM", text_parts))
    if have_data_rom then table.insert(link_parts, _link_parts("DATA_ROM", rodata_parts)) end
    table.insert(link_parts, _link_parts("DATA_RAM", data_parts))
    _combine_parts(link_parts, io_parts)

    return memory_parts
end