Using(self, "System.Drawing")

Node = {}
Node.__index = Node

function Node:new(x, y, width, height)
    return setmetatable({
        x = x,
        y = y,
        width = width,
        height = height,
        used = false,
        down = nil,
        right = nil,
    }, Node)
end

function Node:fit(part)
    if self.used then
        return self.right and self.right:fit(part) or nil
            or self.down and self.down:fit(part) or nil
    elseif part.sch.MaxTilesX <= self.width and part.sch.MaxTilesY <= self.height then
        self.used = true
        self.right = Node:new(self.x + part.sch.MaxTilesX, self.y, self.width - part.sch.MaxTilesX, self.height)
        self.down = Node:new(self.x, self.y + part.sch.MaxTilesY, self.width, self.height - part.sch.MaxTilesY)
        return self
    else
        return nil
    end
end

function SortByArea(a, b)
    local areaA = a.sch.MaxTilesX * a.sch.MaxTilesY
    local areaB = b.sch.MaxTilesX * b.sch.MaxTilesY
    return areaA > areaB
end

function Link(world, parts)
    local range = 40
    local root = Node:new(range, range, world.MaxTilesX - range, world.MaxTilesY - range)
    table.sort(parts, SortByArea)
    for _, part in ipairs(parts) do
        local node = root:fit(part)
        if node then
            local pos = Point(node.x, node.y)
            Tool.Paste(world, pos, part.sch)
        else
            print("Error: Could not fit part into the world.")
        end
    end
end
