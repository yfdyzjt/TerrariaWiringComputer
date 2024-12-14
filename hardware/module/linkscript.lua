function main(parts)
    local result = ""
    local have_data_rom = false

    result = result .. "MEMORY\n"
    result = result .. "{" .. "\n"
    for _, part in ipairs(parts) do
        if part.memory then
            result = result .. "\t" .. string.format("%s (%s) : ORIGIN = %d, LENGTH = %d", part.memory, part.rwx, part.origin, part.length) .. "\n"
            if part.memory == "DATA_ROM" then
                have_data_rom = true
            end
        end
    end
    result = result .. "}\n"
    result = result .. "SECTIONS\n"
    result = result .. "{\n"
    for _, part in ipairs(parts) do
        if part.memory then
            if part.memory == "INS_ROM" then
                result = result .. "\t.text :\n"
                result = result .. "\t{\n"
                result = result .. "\t\tKEEP(*(.text.start));\n"
                result = result .. "\t\tKEEP(*(.text.reset));\n"
                result = result .. "\t\t*(.text .text.*);\n"
                result = result .. "\t} > " .. part.memory .. "\n"
            elseif part.memory == "DATA_RAM" then
                result = result .. "\t.data :\n"
                result = result .. "\t{\n"
                result = result .. "\t\t*(.data .data.* .sdata .sdata.*);\n"
                result = result .. "\t} > " .. part.memory .. "\n"
                result = result .. "\t.bss :\n"
                result = result .. "\t{\n"
                result = result .. "\t\t*(.bss .bss.* .sbss .sbss.*);\n"
                result = result .. "\t} > " .. part.memory .. "\n"
                if not have_data_rom then
                    result = result .. "\t.rodata :\n"
                    result = result .. "\t{\n"
                    result = result .. "\t\t*(.rodata .rodata.* .srodata .srodata.*);\n"
                    result = result .. "\t} > " .. part.memory .. "\n"
                end
            elseif part.memory == "DATA_ROM" then
                result = result .. "\t.rodata :\n"
                result = result .. "\t{\n"
                result = result .. "\t\t*(.rodata .rodata.* .srodata .srodata.*);\n"
                result = result .. "\t} > " .. part.memory .. "\n"
            else
                result = result .. "\t" .. part.section .. " :\n"
                result = result .. "\t{" .. "\n"
                result = result .. "\t\t*(" .. part.section .. ");\n"
                result = result .. "\t} > " .. part.memory .. "\n"
            end
        end
    end
    result = result .. "\t_stack_start = ORIGIN(DATA_RAM) + LENGTH(DATA_RAM);\n"
    result = result .. "}\n"
    return result
end