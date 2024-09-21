function main(parts)
    local result = ""

    result = result .. "MEMORY\n"
    result = result .. "{" .. "\n"
    for _, part in ipairs(parts) do
        result = result .. "\t" .. string.format("%s (%s) : ORIGIN = %d, LENGTH = %d", part.memory, part.rwx, part.origin, part.length) .. "\n"
    end
    result = result .. "}\n"
    result = result .. "SECTIONS\n"
    result = result .. "{\n"
    for _, part in ipairs(parts) do
        if part.memory == "INS_ROM" then
            result = result .. "\t.text :\n"
            result = result .. "\t{\n"
            result = result .. "\t\tKEEP(*(.text.start));\n"
            result = result .. "\t\tKEEP(*(.text.reset));\n"
            result = result .. "\t\t*(.text .text.*);\n"
            result = result .. "\t} > " .. part.memory .. "\n"
        elseif part.memory == "DATA_RAM" then
            result = result .. "\t.bss :\n"
            result = result .. "\t{\n"
            result = result .. "\t\t*(.bss .bss.*);\n"
            result = result .. "\t} > " .. part.memory .. "\n"
            result = result .. "\t.data :\n"
            result = result .. "\t{\n"
            result = result .. "\t\t*(.data .data.*);\n"
            result = result .. "\t} > " .. part.memory .. "\n"
        elseif part.memory == "DATA_ROM" then
            result = result .. "\t.rodata :\n"
            result = result .. "\t{\n"
            result = result .. "\t\t*(.rodata .rodata.*);\n"
            result = result .. "\t} > " .. part.memory .. "\n"
        else
            result = result .. "\t" .. part.section .. " :\n"
            result = result .. "\t{" .. "\n"
            result = result .. "\t\t*(" .. part.section .. ");\n"
            result = result .. "\t} > " .. part.memory .. "\n"
        end
    end
    result = result .. "\t_stack_start = ORIGIN(DATA_RAM) + LENGTH(DATA_RAM);\n"
    result = result .. "}\n"
    return result
end