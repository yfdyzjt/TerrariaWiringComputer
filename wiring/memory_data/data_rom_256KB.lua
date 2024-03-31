WIRE_WRITE DATA_ROM_256KB

Origin=0x200000

MaxLine=256
MaxRow=32

OffsetX=54
OffsetY=23

function Xor(data)
	return data~(data<<1)
end

function TryReadUInt32()
	bytes={}
	for i=0,3,1 do
		if(bin.BaseStream.Position>=bin.BaseStream.Length)then
			bytes[i]=0
		else
			bytes[i]=bin.ReadByte()
		end
	end
	return ((bytes[0]&0xFF)<<0)|((bytes[1]&0xFF)<<8)|((bytes[2]&0xFF)<<16)|((bytes[3]&0xFF)<<24)
end

function IsZero(data,bit)
	if(((1<<bit)&data)==0)then
		return true
	else
		return false
	end
end

bin.BaseStream.Position=Origin
for line=0,MaxLine-1,1 do
	for color=0,3,1 do
		if(color==2 or color==3)then
			y=sign.Y+OffsetY+line*3+3
		else
			y=sign.Y+OffsetY+line*3
		end
		for row=0,MaxRow-1,1 do
			for dir=-1,1,2 do
				data=Xor(TryReadUInt32())
				for bit=0,31,1 do
					x=sign.X+OffsetX+row*65+dir*(bit+1)
					if(color==0)then
						tiles[x][y].WireRed=IsZero(data,bit)
					elseif(color==1)then
						tiles[x][y].WireBlue=IsZero(data,bit)
					elseif(color==2)then
						tiles[x][y].WireGreen=IsZero(data,bit)
					elseif(color==3)then
						tiles[x][y].WireYellow=IsZero(data,bit)
					end
				end
			end
		end
	end
end