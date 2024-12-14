
local function _templink(target)
    import("core.tool.linker")

    local temp_link_file = ""
    local program, argv = linker.linkargv(target:targetkind(), target:sourcekinds(), target:objectfiles(), target:targetfile(), {target = target})
                
    for i, arg in ipairs(argv) do
        if string.find(arg, "-Wl,-T", 1, true) then
            argv[i] = "-Wl,-T,./hardware/module/temp_link.ld"
        elseif string.find(arg, target:targetfile(), 1, true) then
            argv[i] = string.gsub(arg, "(%.elf)$", "_temp%1")
            
            temp_link_file = argv[i]
        end
    end

    os.runv(program, argv)

    return temp_link_file
end

local function _size(sections)
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

function main(target, sdk_program)
    import("readelf")

    local link_temp_file = _templink(target)
    local link_temp_sections = readelf(sdk_program, link_temp_file)
    local link_temp_sections_size = _size(link_temp_sections)

    return link_temp_sections_size
end