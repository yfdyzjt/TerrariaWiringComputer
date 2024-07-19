Using(self, "System.Drawing")

function Link(world, parts)
    local x : int = 100
    local y : int = 100
    for _, part in ipairs(parts) do
        Tool.Paste(world, Point(x, y), part.Sch)
        x = x + part.Sch.MaxTilesX
    end
end
