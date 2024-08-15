Using(self, "System.Drawing")

local function CompareByMaxTilesY(a, b)
    return a.Sch.MaxTilesY > b.Sch.MaxTilesY
end

local function Paste(world, parts, x, y)
    local save = x
    table.sort(parts, CompareByMaxTilesY)
    for _, part in ipairs(parts) do
        local pos = Point(x, y)
        Tool.Paste(world, pos, part.Sch)
        x = x + part.Sch.MaxTilesX
    end
    return {X = x - save, Y = parts[1].Sch.MaxTilesY}
end

local function PasteInputs(world, parts, x, y)
    table.sort(parts, CompareByMaxTilesY)
    for _, part in ipairs(parts) do
        local pos = Point(x - part.Sch.MaxTilesX, y - part.Sch.MaxTilesY)
        Tool.Paste(world, pos, part.Sch)
        y = y - part.Sch.MaxTilesY

        if part.Env.SpawnPos then
            world.SpawnTileX = x - part.Sch.MaxTilesX + part.Env.SpawnPos[1]
            world.SpawnTileY = y + part.Env.SpawnPos[2]
        end
    end
end

local function PasteOthers(world, outputs, inputs, others, x, y)
    local pos : table = Paste(world, outputs, x, y)
    Paste(world, others, x + pos.X, y)
    PasteInputs(world, inputs, x, y + pos.Y)
end

function Link(world, parts)
    local mem_ins : table = {}
    local mem_data : table = {}
    local input : table = {}
    local output : table = {}
    local other : table = {}

    local x : int = 50
    local y : int = 50

    for _, part in ipairs(parts) do
        if part.Type == "memory_ins" then
            table.insert(mem_ins, part)
        elseif part.Type == "memory_data" then
            table.insert(mem_data, part)
        elseif part.Type == "input" then
            table.insert(input, part)
        elseif part.Type == "output" then
            table.insert(output, part)
        else
            table.insert(other, part)
        end
    end

    y = y + Paste(world, mem_ins, x, y).Y
    y = y + Paste(world, mem_data, x, y).Y
    PasteOthers(world, output, input, other, x, y)
end
