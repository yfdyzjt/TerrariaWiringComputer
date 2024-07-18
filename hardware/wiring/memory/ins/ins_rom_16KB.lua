AddrPos = nil

local MaxLine : int = 64
local MaxRow : int = 8

local OffsetX = {35, 332}
local OffsetY = {21, 21}

function Write(sch, bin)
	return Include(self, "./hardware/wiring/**/ins_rom.lua").Write(sch, bin, MaxLine, MaxRow, OffsetX, OffsetY)
end
