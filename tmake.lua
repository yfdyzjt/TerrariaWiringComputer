Using(self, "System.IO")
Using(self, "System.Text")
Using(self, "System.Drawing")

function MakeLinkScript(hardwarename)
    local parts = Include(self,"./hardware/src/" .. hardwarename .. ".lua").Parts

    local sb = StringBuilder()

    local ins_name : string = ""
    local data_ram_name : string = ""
    local data_rom_name : string = ""

    sb.Append("MEMORY\n") 
    sb.Append("{\n")
    for _, part in ipairs(parts) do
        if part.Type == "memory_ins" or part.Type == "memory_data" then
            sb.Append("\t" .. part.Name.ToUpper() .. " (" .. part.Permission .. ") : ORIGIN = " .. cast(string, part.Origin) .. ", LENGTH = " .. cast(string, part.Length) .. "\n")
        end

        if part.Type == "memory_ins" then
            ins_name = part.Name.ToUpper() 
        elseif part.Type == "memory_data" then
            if part.Permission == "rw" then
                data_ram_name = part.Name.ToUpper() 
            elseif part.Permission == "r" then
                data_rom_name = part.Name.ToUpper() 
            end
        end
    end
    if data_rom_name == "" then
        data_rom_name = data_ram_name
    end
    sb.Append("}\n")

    sb.Append("ENTRY(reset);\n")

    sb.Append("SECTIONS\n")
    sb.Append("{\n")

    sb.Append("\t.text :\n")
    sb.Append("\t{\n")
    sb.Append("\t\tKEEP(*(.text.start));\n")
    sb.Append("\t\t*(.text.reset);\n")
    sb.Append("\t\t*(.text .text.*);\n")
    sb.Append("\t} > " .. ins_name .. "\n")

    sb.Append("\t.bss :\n")
    sb.Append("\t{\n")
    sb.Append("\t\t*(.bss .bss.*);\n")
    sb.Append("\t} > " .. data_ram_name .. "\n")

    sb.Append("\t.data :\n")
    sb.Append("\t{\n")
    sb.Append("\t\t*(.data .data.*);\n")
    sb.Append("\t} > " .. data_ram_name .. "\n")

    sb.Append("\t.rodata :\n")
    sb.Append("\t{\n")
    sb.Append("\t\t*(.rodata .rodata.*);\n")
    sb.Append("\t} > " .. data_rom_name .. "\n")

    sb.Append("\t_stack_start = ORIGIN(" .. data_ram_name .. ") + LENGTH(" .. data_ram_name .. ");\n")

    sb.Append("}\n")

    if not Directory.Exists("./system/bin/") then
        Directory.CreateDirectory("./system/bin/")
    end
    File.WriteAllText("./system/bin/" .. hardwarename .."_link.ld", sb.ToString())
end

local function MakeSch(part, bin)
    local env = Include(self, "./hardware/wiring/**/" .. part.Name .. ".lua")
    local sch = LoadSchematic("./hardware/wiring/**/" .. part.Name .. ".sch")

    if env.AddrPos then
        local pos : table = env.AddrPos
        for bit = 1, 12, 1 do
            Tool.SetLogicLamp(sch.Tile[pos[bit][1], pos[bit][2]], Tool.IsZero(part.Origin, bit + 19))
        end
    end

    if part.Type ~= "cpu" and part.Type ~= "driver" then
        bin.BaseStream.Position = part.Origin
    end

    return env.Write(sch, bin)
end

function MakeWorld(softwarename, hardwarename)
    print("Start make world")

    local parts = Include(self, "./hardware/src/" .. hardwarename .. ".lua").Parts

    local fs = FileStream("./system/bin/" .. softwarename .. ".bin", FileMode.Open)
    local br = BinaryReader(fs)

    local world = LoadWorld("./hardware/world/terraria_computer_base.wld")

    local range : int = 100

    local x : int = range
    local y : int = range

    for _, part in ipairs(parts) do
        print("Start make part: " .. part.Name)
        local sch = MakeSch(part, br)
        local pos = Point(x, y)
        Tool.Paste(world, pos, sch)
        x = x + sch.MaxTilesX
    end

    br.Close()
    fs.Close()

    world.Name = "terraria_computer"
    SaveWorld(world, "./system/bin/terraria_computer.wld")

    print("Finish make world")
end
