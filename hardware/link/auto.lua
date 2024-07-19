Using(self, "System.Drawing")

function Link(world, parts)
    local range : int = 100

    local x : int = range
    local y : int = range
    for _, part in ipairs(parts) do
        local sch = part.Sch
        local pos = Point(x, y)
        Tool.Paste(world, pos, sch)
        x = x + sch.MaxTilesX
    end
end