WIRE_WRITE DATA_RAM_64KB

Origin=0x100000

MaxRow=1024
MaxLine=32

OffsetX=22
OffsetY=26

ActRow={row,dir}
ActRowData={}

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
		x=sign.X+OffsetX+row*3+dir
		y=sign.Y+OffsetY-3
		if(GetLamp(x,y))then
			ActRow.row=row
			ActRow.dir=dir
			for line=0,MaxLine-1,1 do
				ActRowData[line]=TryReadByte()
			end
			goto WriteData
		end
		bin.BaseStream.Position=bin.BaseStream.Position+256
	end
end

::WriteData::
bin.BaseStream.Position=Origin
for row=0,MaxRow-1,1 do
	for dir=-1,1,2 do
		for line=0,MaxLine-1,1 do
			data=TryReadByte()
			mesh=ActRowData[line]
			for bit=0,7,1 do
				x=sign.X+OffsetX+row*3+dir
				y=sign.Y+OffsetY+line*24+bit*3
				var=IsOne(data,bit)
				if(row==0 and dir==-1)then
					SetLamp(x-2,y,var)
					SetLamp(x-3,y,var)
				end
				if(IsOne(mesh,bit)and((row~=ActRow.row)or(dir~=ActRow.dir)))then
					var=not var
				end
				SetLamp(x,y,var)
			end
		end
	end
end
