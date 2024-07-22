Using(self, "System.Drawing")

local function CompareByMaxTilesY(a, b)
    return a.Sch.MaxTilesY > b.Sch.MaxTilesY
end

local function SortAndPaste(world, parts, x, y)
    table.sort(parts, CompareByMaxTilesY)
    for _, part in ipairs(parts) do
        local pos = Point(x, y)
        if part.Type == "output" then
            world.SpawnTileX = x + 5
            world.SpawnTileY = y + part.Sch.MaxTilesY + 7
        elseif part.Type == "input" then
            pos = Point(world.SpawnTileX - 5, world.SpawnTileY - 7)
        end
        Tool.Paste(world, pos, part.Sch)

        x = x + part.Sch.MaxTilesX
    end
    return parts[1].Sch.MaxTilesY
end

function Link(world, parts)
    local mem_ins : table = {}
    local mem_data : table = {}
    local other : table = {}

    local x : int = 50
    local y : int = 50

    for _, part in ipairs(parts) do
        if part.Type == "memory_ins" then
            table.insert(mem_ins, part)
        elseif part.Type == "memory_data" then
            table.insert(mem_data, part)
        else
            table.insert(other, part)
        end
    end

    y = y + SortAndPaste(world, mem_ins, x, y)
    y = y + SortAndPaste(world, mem_data, x, y)
    y = y + SortAndPaste(world, other, x, y)
end
