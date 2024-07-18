AddrPos = {{2, 4}, {3, 6}, {2, 8}, {5, 4}, {6, 6}, {5, 8}, {7, 4}, {8, 6}, {7, 8}, {10, 4}, {11, 6}, {10, 8}}

local MaxLine : int = 256
local MaxRow : int = 32

local OffsetX : int = 54
local OffsetY : int = 23

function Write(sch, bin)
	return Include(self, "./hardware/wiring/**/data_rom.lua").Write(sch, bin, MaxLine, MaxRow, OffsetX, OffsetY)
end
