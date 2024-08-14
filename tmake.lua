Using(self, "System.IO")
Using(self, "System.Text")
Using(self, "System.Runtime.InteropServices")

function MakeLinkScript(hardwarename)
    local parts = Include(self,"./hardware/module/" .. hardwarename .. ".lua").Parts

    local sb = StringBuilder()

    local ins_name : string = ""
    local data_ram_name : string = ""
    local data_rom_name : string = ""

    sb.Append("MEMORY\n") 
    sb.Append("{\n")
    for _, part in ipairs(parts) do
        if part.Type == "memory_ins" or part.Type == "memory_data" then
            local permission : string = ""
            if part.Type == "memory_ins" then
                permission = "x"
                ins_name = part.Name.ToUpper() 
            else
                if string.find(part.Name, "rom") then
                    permission = "r"
                    data_rom_name = part.Name.ToUpper() 
                elseif string.find(part.Name, "ram") then
                    permission = "rw"
                    data_ram_name = part.Name.ToUpper() 
                end
            end
            sb.Append("\t" .. part.Name.ToUpper() .. " (" .. permission .. ") : ORIGIN = " .. cast(string, part.Origin) .. ", LENGTH = " .. cast(string, part.Length) .. "\n")
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

    if not Directory.Exists("./system/") then
        Directory.CreateDirectory("./system/")
    end
    File.WriteAllText("./system/" .. hardwarename .."_link.ld", sb.ToString())
end

local function MakeSch(part, bin)
    local env = Include(self, "./hardware/wiring/**/" .. part.Name .. ".lua")
    local sch = LoadSchematic("./hardware/wiring/**/" .. part.Name .. ".sch")

    if env.AddrPos then
        for bit, pos in ipairs(env.AddrPos) do
            Tool.SetLogicLamp(sch.Tile[pos[1], pos[2]], Tool.IsZero(part.Origin, bit + 19))
        end
    end

    if part.Type ~= "cpu" and part.Type ~= "driver" then
        bin.BaseStream.Position = part.Origin
    end

    return env.Write(sch, bin)
end

function MakeWorld(softwarename, hardwarename)
    local mode = Include(self, "./hardware/module/" .. hardwarename .. ".lua")

    local fs = FileStream("./system/" .. softwarename .. ".bin", FileMode.Open)
    local br = BinaryReader(fs)

    print("Start load world: " .. mode.World)
    local world = LoadWorld("./hardware/world/" .. mode.World .. ".wld")

    for _, part in ipairs(mode.Parts) do
        print("Start make part: " .. part.Name)
        part.Sch = MakeSch(part, br)
    end

    br.Close()
    fs.Close()

    print("Start link part to world")
    Include(self, "./hardware/link/" .. mode.Link .. ".lua").Link(world, mode.Parts)

    print("Start save world")
    world.Name = world.Name .. "_" .. hardwarename .. "_" .. softwarename
    
    SaveWorld(world, "./system/" .. world.Name .. ".wld")
end
