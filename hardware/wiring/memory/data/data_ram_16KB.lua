AddrPos = {{3, 35}, {3, 38}, {3, 41}, {3, 44}, {3, 47}, {3, 50}, {3, 53}, {3, 56}}

local MaxLine : int = 16
local MaxRow : int = 512

local OffsetX : int = 20
local OffsetY : int = 29

function Write(sch, bin)
	return Include(self, "./hardware/wiring/**/data_ram.lua").Write(sch, bin, MaxLine, MaxRow, OffsetX, OffsetY)
end
