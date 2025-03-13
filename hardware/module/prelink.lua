local function _templink(target)
    import("core.tool.linker")

    local temp_link_file = ""
    local program, argv = linker.linkargv(target:targetkind(), target:sourcekinds(), target:objectfiles(), target:targetfile(), {target = target})
                
    for i, _ in ipairs(argv) do
        if string.find(argv[i], "%-Wl,%-T,%./system/.+_link%.ld$", 1, false) then
            argv[i] = "-Wl,-T,./hardware/module/temp_link.ld"
        elseif string.find(argv[i], target:targetfile(), 1, true) then
            argv[i] = string.gsub(argv[i], "(%.elf)$", "_temp%1")
            temp_link_file = argv[i]
        end
    end

    local outdata, errdata =os.iorunv(program, argv)
    return temp_link_file
end

function main(target)
    import("readelf")

    local link_temp_file = _templink(target)
    local link_temp_sections = readelf(link_temp_file)

    return link_temp_sections
end