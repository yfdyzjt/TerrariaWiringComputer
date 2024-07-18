function Write(sch, bin, maxLine : int, maxRow : int, offsetX : int, offsetY : int)
	local x : int = 0
	local y : int = 0

	local origin : int = bin.BaseStream.Position

	for line = 0, maxLine - 1, 1 do
		bin.BaseStream.Position = origin + line
		local meshdata : byte = bin.BaseStream.Position < bin.BaseStream.Length and bin.ReadByte() or 0

		for row = 0, maxRow - 1, 1 do
			for dir = -1, 1, 2 do
				bin.BaseStream.Position = origin + line + maxLine * (row * 2 + (dir == -1 and 0 or 1))
				local data : byte = bin.BaseStream.Position < bin.BaseStream.Length and bin.ReadByte() or 0

				x = offsetX + row * 3 + dir

				for bit = 0, 7, 1 do
					y = offsetY + line * 24 + bit * 3
					
					local var : bool = Tool.IsOne(data, bit)
					if row == 0 and dir == -1 then
						Tool.SetLogicLamp(sch.Tile[x - 2, y], var)
						Tool.SetLogicLamp(sch.Tile[x - 3, y], var)
					elseif Tool.IsOne(meshdata, bit) then
						var = not var
					end
					Tool.SetLogicLamp(sch.Tile[x, y], var)
				end
			end
		end
	end

	return sch
end
