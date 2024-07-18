AddrPos = nil

local MaxLine : int = 256
local MaxRow : int = 32

local OffsetX = {38, 1135}
local OffsetY = {22, 22}

function Write(sch, bin)
	return Include(self, "./hardware/wiring/**/ins_rom.lua").Write(sch, bin, MaxLine, MaxRow, OffsetX, OffsetY)
end
