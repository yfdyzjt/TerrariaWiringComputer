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

function Node:fit(width, height)
    if self.used then
        return self.right and self.right:fit(width, height) or nil
            or self.down and self.down:fit(width, height) or nil
    elseif width <= self.width and height <= self.height then
        self.used = true
        self.right = Node:new(self.x + width, self.y, self.width - width, self.height)
        self.down = Node:new(self.x, self.y + height, self.width, self.height - height)
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
    local range = 5

    local io_size_x = 360
    local io_size_y = 500
    local io_parts = {}
    for i = #parts, 1, -1 do
        local part = parts[i]
        if part.type == "input" or part.type == "output" then
            table.insert(io_parts, part)
            table.remove(parts, i)
        end
    end
    local io_root = Node:new(0, 0, io_size_x, io_size_y)
    table.sort(io_parts, SortByArea)
    for _, io_part in ipairs(io_parts) do
        local node = io_root:fit(io_part.sch.MaxTilesX, io_part.sch.MaxTilesY)
        if node then
            local pos_x = range + io_size_x - io_part.sch.MaxTilesX - node.x
            local pos_y = range + io_size_y - io_part.sch.MaxTilesY - node.y
            local pos = Point(pos_x, pos_y)
            Tool.Paste(world, pos, io_part.sch)
            if io_part.env.SpawnPos then
                world.SpawnTileX = pos_x + io_part.env.SpawnPos[1]
                world.SpawnTileY = pos_y + io_part.env.SpawnPos[2]
            end
        else
            error("Could not fit io part into the io area.")
        end
    end

    local root = Node:new(range, range, world.MaxTilesX - range, world.MaxTilesY - range)
    local io_node = root:fit(io_size_x, io_size_y)
    table.sort(parts, SortByArea)
    for _, part in ipairs(parts) do
        local node = root:fit(part.sch.MaxTilesX, part.sch.MaxTilesY)
        if node then
            local pos = Point(node.x, node.y)
            Tool.Paste(world, pos, part.sch)
        else
            error("Could not fit part into the world area.")
        end
    end
end
