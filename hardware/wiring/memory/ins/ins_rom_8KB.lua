local MaxLine : int = 64
local MaxRow : int = 4

local OffsetX = {34, 199}
local OffsetY = {21, 21}

function Write(sch, bin)
	return Include(self, "./hardware/wiring/**/ins_rom.lua").Write(sch, bin, MaxLine, MaxRow, OffsetX, OffsetY)
end
