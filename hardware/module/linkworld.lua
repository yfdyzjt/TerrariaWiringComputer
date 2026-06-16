Using(self, "System.Drawing")

local Node = {}
Node.__index = Node

function Node:new(x, y, width, height)
    return setmetatable({
        x = x, y = y, width = width, height = height,
        used = false, down = nil, right = nil,
    }, Node)
end

function Node:fit(width, height)
    if self.used then
        return self.right and self.right:fit(width, height) or nil
            or self.down and self.down:fit(width, height) or nil
    elseif width <= self.width and height <= self.height then
        self.used = true
        self.right = Node:new(self.x + width, self.y, self.width - width, self.height)
        self.down = Node:new(self.x, self.y + height, width, self.height - height)
        return self
    else
        return nil
    end
end

local function SortByArea(a, b)
    return a.sch.MaxTilesX * a.sch.MaxTilesY > b.sch.MaxTilesX * b.sch.MaxTilesY
end

local function SortByWidth(a, b)
    return a.sch.MaxTilesX > b.sch.MaxTilesX
end

local function SortParts(p, cmp)
    table.sort(p, function(a, b)
        local ap = a.env.PriorPlace or false
        local bp = b.env.PriorPlace or false
        if ap ~= bp then
            return ap
        end
        return cmp(a, b)
    end)
end

local function ExtractParts(parts, part_type)
    local extracted = {}
    for i = #parts, 1, -1 do
        local part = parts[i]
        if part.type == part_type then
            table.insert(extracted, part)
            table.remove(parts, i)
        end
    end
    return extracted
end

local function MergeTables(...)
    local result = {}
    for _, t in ipairs({...}) do
        for _, v in ipairs(t) do
            table.insert(result, v)
        end
    end
    return result
end

local function ReportErrors(parts, positions, errFmt, hasError)
    if not hasError then return end
    for i, pos in ipairs(positions) do
        if not pos then print(string.format(errFmt, parts[i].name)) end
    end
end

local function PackRects(parts, max_w, max_h, pre_w, pre_h)
    local positions = {}
    if max_w <= 0 or max_h <= 0 then
        for _ = 1, #parts do table.insert(positions, nil) end
        return positions, pre_w, pre_h, false
    end
    local root = Node:new(0, 0, max_w, max_h)
    if pre_w > 0 and pre_h > 0 then
        root:fit(pre_w, pre_h)
    end
    local area_w, area_h = pre_w, pre_h
    local all_ok = true
    for _, part in ipairs(parts) do
        local node = root:fit(part.sch.MaxTilesX, part.sch.MaxTilesY)
        if node then
            table.insert(positions, {x = node.x, y = node.y})
            local r = node.x + part.sch.MaxTilesX
            if r > area_w then area_w = r end
            local b = node.y + part.sch.MaxTilesY
            if b > area_h then area_h = b end
        else
            table.insert(positions, nil)
            all_ok = false
        end
    end
    return positions, area_w, area_h, all_ok
end

local function TransformPositions(parts, positions, area_x, area_y, area_w, area_h, flipX, flipY)
    local result = {}
    for i, pos in ipairs(positions) do
        if pos then
            local w, h = parts[i].sch.MaxTilesX, parts[i].sch.MaxTilesY
            local x = flipX and (area_x - pos.x - w + 1) or (area_x + pos.x)
            local y = flipY and (area_y - pos.y - h + 1) or (area_y + pos.y)
            table.insert(result, {x = x, y = y})
        else
            table.insert(result, nil)
        end
    end
    return result
end

local function PasteParts(world, parts, positions)
    for i, pos in ipairs(positions) do
        local part = parts[i]
        if pos then
            print(string.format("Paste part: %s to (%d, %d)", part.name, pos.x, pos.y))
            Tool.Paste(world, Point(pos.x, pos.y), part.sch)
            if part.env.SpawnPos then
                world.SpawnTileX = pos.x + part.env.SpawnPos[1]
                world.SpawnTileY = pos.y + part.env.SpawnPos[2]
            end
        end
    end
end

function Link(world, parts)
    local UNUSABLE = 2
    local INV_TOP, INV_LEFT, INV_BOTTOM, INV_RIGHT = 41, 41, 42, 42

    local iopos_x, iopos_y = world.env.IOPos[1], world.env.IOPos[2]

    local input_parts = ExtractParts(parts, "input")
    SortParts(input_parts, SortByWidth)
    local in_pos, in_w, in_h, in_ok = PackRects(input_parts, iopos_x - UNUSABLE, iopos_y - UNUSABLE, 0, 0)

    local output_parts = ExtractParts(parts, "output")
    SortParts(output_parts, SortByWidth)
    local out_pos, out_w, out_h, out_ok = PackRects(output_parts, world.MaxTilesX - (iopos_x + 1) - UNUSABLE, iopos_y - UNUSABLE, 0, 0)

    SortParts(parts, SortByArea)

    local io_width = iopos_x + out_w + 1
    local io_height = world.MaxTilesY - iopos_y - 1 + math.max(in_h, out_h)

    local max_w = world.MaxTilesX - INV_LEFT - INV_RIGHT
    local max_h = world.MaxTilesY - INV_TOP - INV_BOTTOM
    local pre_w, pre_h = io_width - INV_LEFT, io_height - INV_BOTTOM
    local other_pos, other_w, other_h, other_ok = PackRects(parts, max_w, max_h, pre_w, pre_h)
    local other_area_x, other_area_y = INV_LEFT, world.MaxTilesY - 1 - INV_BOTTOM

    if not other_ok then
        max_w = world.MaxTilesX - 2 * UNUSABLE
        max_h = world.MaxTilesY - 2 * UNUSABLE
        pre_w, pre_h = io_width - UNUSABLE, io_height - UNUSABLE
        other_pos, other_w, other_h, other_ok = PackRects(parts, max_w, max_h, pre_w, pre_h)
        other_area_x, other_area_y = UNUSABLE, world.MaxTilesY - 1 - UNUSABLE
    end

    ReportErrors(input_parts, in_pos, "Could not fit io part: %s into the io area.", not in_ok)
    ReportErrors(output_parts, out_pos, "Could not fit io part: %s into the io area.", not out_ok)
    ReportErrors(parts, other_pos, "Could not fit part: %s into the world area.", not other_ok)

    in_pos = TransformPositions(input_parts, in_pos, iopos_x, iopos_y, in_w, in_h, true, true)
    out_pos = TransformPositions(output_parts, out_pos, iopos_x + 1, iopos_y, out_w, out_h, false, true)
    other_pos = TransformPositions(parts, other_pos, other_area_x, other_area_y, other_w, other_h, false, true)

    local all_parts = MergeTables(input_parts, output_parts, parts)
    local all_pos = MergeTables(in_pos, out_pos, other_pos)
    PasteParts(world, all_parts, all_pos)
end
