local MaxLine : int = 128
local MaxRow : int = 16

local OffsetX = {36, 601}
local OffsetY = {22, 22}

function Write(sch, bin)
	return Include(self, "./hardware/wiring/**/ins_rom.lua").Write(sch, bin, MaxLine, MaxRow, OffsetX, OffsetY)
end
