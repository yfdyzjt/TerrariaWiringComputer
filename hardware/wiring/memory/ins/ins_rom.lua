function Write(sch, bin, maxLine : int, maxRow : int, offsetX : table, offsetY : table)
	local x : int = 0
	local y : int = 0

	local data : ushort = 0
	local long_ins : bool = false
	local long_ins_high : ushort = 0

	for row = 0, maxRow - 1, 1 do
		for dir = -1, 1, 2 do
			for line = 0, maxLine - 1, 1 do
				for color = 0, 3, 1 do
					for cell = 1, 2, 1 do
						if bin.BaseStream.Position >= bin.BaseStream.Length then
							data = 0
						elseif long_ins then
							data = long_ins_high
							long_ins = false
						elseif (bin.ReadUInt16() & 3) == 3 then
							bin.BaseStream.Position = bin.BaseStream.Position - 2
							local full : uint = bin.ReadUInt32()
							full = Tool.BinaryToInvGrayCode(full)
							data = full & 0xFFFF
							long_ins_high = (full >> 16) & 0xFFFF
							long_ins = true
						else
							bin.BaseStream.Position = bin.BaseStream.Position - 2
							data = bin.ReadUInt16()
							data = Tool.BinaryToInvGrayCode(data)
						end
						y = offsetY[cell] + (line + ((color == 2 or color == 3) and 1 or 0)) * 3
						for bit = 0, 15, 1 do
							x = offsetX[cell] + row * 33 + dir * (bit + 1)
							Tool.SetWire(sch.Tile[x, y], color, Tool.IsZero(data, bit))
						end
					end
				end
			end
		end
	end

	return sch
end
