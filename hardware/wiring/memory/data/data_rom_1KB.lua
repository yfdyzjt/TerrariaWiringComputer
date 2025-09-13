AddrPos = {{2, 4}, {3, 6}, {6, 4}, {5, 6}, {7, 4}, {8, 6}, {11, 4}, {10, 6}}

local MaxLine : int = 32
local MaxRow : int = 1

local OffsetX : int = 55
local OffsetY : int = 23

function Write(sch, bin)
	return Include(self, "./hardware/wiring/**/data_rom.lua").Write(sch, bin, MaxLine, MaxRow, OffsetX, OffsetY)
end
