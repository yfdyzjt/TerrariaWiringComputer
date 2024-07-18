function Write(sch, bin, maxLine : int, maxRow : int, offsetX : int, offsetY : int)
	local x : int = 0
	local y : int = 0

	for line = 0, maxLine - 1, 1 do
		for color = 0, 3, 1 do
			y = offsetY + (line + (color == 2 or color == 3) and 1 or 0) * 3
			for row = 0, maxRow - 1, 1 do
				for dir = -1, 1, 2 do
					local data = {}
					for byte = 0, 3, 1 do
						data[byte] = bin.BaseStream.Position < bin.BaseStream.Length and bin.ReadByte() or 0
					end
					data = Tool.BinaryToInvGrayCodeAny(data)
					for byte = 0, 3, 1 do
						for bit = 0, 7, 1 do
							x = offsetX + row * 65 + dir * (byte * 8 + bit + 1)
							Tool.SetWire(sch.Tile[x, y], color, Tool.IsZero(data[byte], byte * 8 + bit))
						end
					end
				end
			end
		end
	end

	return sch
end
