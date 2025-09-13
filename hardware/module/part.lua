local function _get_ins_mem(size, origin)
    import("part_mem")
    import("part_utils")
    local parts = part_mem("hardware/wiring/memory/ins", "ins_rom", "mem_ins", "x", size)
    local origin = part_utils.assign_mem_addr(parts, origin)
    return parts, origin
end

local function _get_rodata_mem(size, origin)
    import("part_mem")
    import("part_utils")
    local parts = part_mem("hardware/wiring/memory/data", "data_rom", "mem_data", "r", size)
    local origin = part_utils.assign_mem_addr(parts, origin)
    return parts, origin
end

local function _get_data_mem(size, origin)
    import("part_mem")
    import("part_utils")
    local parts = part_mem("hardware/wiring/memory/data", "data_ram", "mem_data", "rw", size)
    local origin = part_utils.assign_mem_addr(parts, origin)
    return parts, origin
end

local function _get_io_mem(sections, origin)
    import("part_io")
    import("part_utils")
    local parts = part_io(sections)
    local origin = part_utils.assign_mem_addr(parts, origin)
    return parts, origin
end

local function _get_driver(freq)
    import("part_driver")
    local parts = part_driver(freq)
    return parts
end

function main(sections, config)
    import("part_utils")

    local parts = {}
    local origin = 0
    local size = part_utils.sections_size(sections)
    local stack_size = part_utils.parse_size(config.stack)
    local driver_freq = part_utils.parse_freq(config.driver)

    parts.text, origin = _get_ins_mem(size.text, origin)
    local have_data_rom = size.rodata >= (size.rodata + size.data + stack_size) / 2
    if have_data_rom then 
        parts.rodata, origin = _get_rodata_mem(size.rodata, origin) 
        parts.data, origin = _get_data_mem(size.data + stack_size, origin)
    else
        parts.data, origin = _get_data_mem(size.rodata + size.data + stack_size, origin)
    end
    parts.io, origin = _get_io_mem(sections, origin)
    parts.driver = _get_driver(driver_freq)

    return parts
end