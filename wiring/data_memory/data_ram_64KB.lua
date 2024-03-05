WIRE_WRITE DATA_RAM_64KB

Origin=0x100000

MaxRow=1024
MaxLine=256

OffsetX=22
OffsetY=26

ActRow={}

function GetLamp(x,y)
	if(tiles[x][y].U==18)then
		return true
	else
		return false
	end
end

function SetLamp(x,y,var)
	tiles[x][y].IsActive=true
	tiles[x][y].Type=419
	if(var)then
		tiles[x][y].U=18
	else
		tiles[x][y].U=0
	end
end

function InvLamp(x,y)
	if(tiles[x][y].U==18)then
		tiles[x][y].U=0
	else
		tiles[x][y].U=18
	end
end

function IsOne(data,bit)
	if(((1<<bit)&data)~=0)then
		return true
	else
		return false
	end
end

function TryReadByte()
	if(bin.BaseStream.Position>=bin.BaseStream.Length)then
		byte=0
	else
		byte=bin.ReadByte()
	end
	return byte
end

bin.BaseStream.Position=Origin
for row=0,MaxRow-1,1 do
	for dir=-1,1,2 do
		bit=8
		for line=0,MaxLine,1 do
			x=sign.X+OffsetX+row*3+dir
			y=sign.Y+OffsetY+line*3
			if(line==0)then
				isActRow=GetLamp(x,y)
			else
				if(bit==8)then
					data=TryReadByte()
					bit=0
				end
				var=IsOne(data,bit)
				SetLamp(x,y,var)
				if(isActRow)then
					ActRow[line]=var
				end
				bit=bit+1
			end
		end
	end
end

for row=0,MaxRow-1,1 do
	for dir=-1,1,2 do
		for line=1,MaxLine,1 do
			x=sign.X+OffsetX+row*3+dir
			y=sign.Y+OffsetY+line*3
			if((row==0) and (ActRow[line]==true))then
				InvLamp(x-2,y)
				InvLamp(x-3,y)
			end
			if(ActRow[line]==true)then
				InvLamp(x,y)
			end
		end
	end
end
