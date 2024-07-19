Parts = {
    {
        Name = "cpu_rv32imc",
        Type = "cpu",
    },
    {
        Name = "ins_rom_256KB",
        Type = "memory_ins",
        Length = 256 << 10,
    },
    {
        Name = "data_ram_64KB",
        Type = "memory_data",
        Length = 64 << 10,
    },
    {
        Name = "display_96_64",
        Type = "output",
        Length = 1 << 10,
    },
    {
        Name = "driver_target_x100_6kHz",
        Type = "driver",
        Origin = 4095 << 20,
        Length = 1,
    }
}

local origin = 0

for _, part in ipairs(Parts) do
    if part.Type ~= "cpu" and part.Type ~= "driver" then
        part.Origin = origin << 20
        origin = origin + 1
    end
end
