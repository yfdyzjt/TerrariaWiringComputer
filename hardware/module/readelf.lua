function main(elf_file)
    local sections = {}
    local content = os.iorunv("riscv-none-elf-readelf", {"-S", "--wide", elf_file})

    for line in content:gmatch("[^\r\n]+") do
        local section = {}

        section.name, section.size, section.type = line:match("%s+%[%s*%d+%]%s+([%p%w]+)%s+[%p%w]+%s+%x+%s+%x+%s+(%x+)%s+%x+%s+([%w]+)")
        
        if  section.name and
            section.name ~= "NULL" and
            section.size and 
            section.type 
        then
            section.size = tonumber(section.size, 16)
            table.insert(sections, section)
        end
    end

    return sections
end