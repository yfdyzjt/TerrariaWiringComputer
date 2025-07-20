Using(self, "System.IO")
-- Using(self, "System.Text")
-- Using(self, "System.Runtime.InteropServices")

local function MakeSch(part, bin)
    local sch = LoadSchematic("./hardware/wiring/**/" .. part.file .. ".sch")
    local env = {}
    if Directory.GetFiles("./hardware/wiring/", part.file .. ".lua", SearchOption.AllDirectories).Length ~= 0 then
        env = Include(self, "./hardware/wiring/**/" .. part.file .. ".lua")
    end

    if env.AddrPos then
        for bit, pos in ipairs(env.AddrPos) do
            Tool.SetLogicLamp(sch.Tile[pos[1], pos[2]], Tool.IsZero(part.origin, bit + 15))
        end
    end
    
    if part.origin then
        bin.BaseStream.Position = part.origin
    end
    
    if env.Write then
        return env, env.Write(sch, bin)
    else
        return env, sch
    end
end

function MakeWorld(worldName, softwareName)
    local parts_str = File.ReadAllText("./system/part/" .. softwareName .. "_part.lua")
    local parts = table.FromLson(parts_str)

    print("Start load world: " .. worldName)
    local world = LoadWorld("./hardware/world/" .. worldName .. ".wld")

    local fs = FileStream("./system/bin/" .. softwareName .. ".bin", FileMode.Open)
    local br = BinaryReader(fs)
    for _, part in ipairs(parts) do
        print("Start make part: " .. part.name)
        part.env, part.sch = MakeSch(part, br)
    end
    br.Close()
    fs.Close()

    print("Start link part to world")
    Include(self, "./hardware/module/linkworld.lua").Link(world, parts)

    print("Start save world")
    world.Name = world.Name .. "_" .. softwareName
    SaveWorld(world, "./system/world/" .. world.Name .. ".wld")
end
