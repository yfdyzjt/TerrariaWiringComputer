function Write(sch, bin, maxLine : int, maxRow : int, offsetX : int, offsetY : int)
	local x : int = 0
	local y : int = 0

	for line = 0, maxLine - 1, 1 do
		for color = 0, 3, 1 do
			y = offsetY + line * 3
			for row = 0, maxRow - 1, 1 do
				for dir = -1, 1, 2 do
					local data : int = 0
					for byte = 0, 3, 1 do
						data = data | (cast(int, bin.BaseStream.Position < bin.BaseStream.Length and bin.ReadByte() or 0) << (byte * 8))
					end
					data = Tool.BinaryToInvGrayCode(data)
					for bit = 0, 31, 1 do
						x = offsetX + row * 65 + dir * (bit + 1)
						Tool.SetWire(sch.Tile[x, y], color, Tool.IsZero(data, bit))
					end
				end
			end
		end
	end

	return sch
end
