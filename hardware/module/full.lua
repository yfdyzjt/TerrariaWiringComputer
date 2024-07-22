World = "terraria_computer_large"
Link = "semi_auto"
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
        Name = "data_rom_256KB",
        Type = "memory_data",
        Length = 256 << 10,
    },
    {
        Name = "display_96_64",
        Type = "output",
        Length = 1 << 10,
    },
    {
        Name = "power_switch",
        Type = "input",
        Length = 0,
    },
    {
        Name = "driver_portal_gun_station_x20_6.96kHz",
        Type = "driver",
    },
}

local origin = 0
for _, part in ipairs(Parts) do
    if part.Type ~= "cpu" and part.Type ~= "driver" then
        part.Origin = origin << 20
        origin = origin + 1
    end
end
