WIRE_WRITE INS_ROM_256KB

Origin=0x000000

MaxRow=32
MaxLine=256

Offset={
	{X=38,Y=22},
	{X=1135,Y=22}
}

InsHigh=nil

function Xor(data)
	return data~(data<<1)
end

function Split(data)
	return data&(0xFFFF),(data>>16)&(0xFFFF)
end

function IsZero(data,bit)
	if(((1<<bit)&data)==0)then
		return true
	else
		return false
	end
end

function GetUInt16()
	if(bin.BaseStream.Position>=bin.BaseStream.Length)then
		result=0
	else
		if(InsHigh~=nil)then
		result=InsHigh
		InsHigh=nil
		else
			if((bin.ReadUInt16()&3)==3)then
				bin.BaseStream.Position=bin.BaseStream.Position-2
				result,InsHigh=Split(Xor(bin.ReadUInt32()))
			else
				bin.BaseStream.Position=bin.BaseStream.Position-2
				result=Xor(bin.ReadUInt16())
			end
		end
	end
	return result
end

bin.BaseStream.Position=Origin
for row=0,MaxRow-1,1 do
	for dir=-1,1,2 do
		for line=0,MaxLine-1,1 do
			for color=0,3,1 do
				for cell=1,2,1 do
					data=GetUInt16()
					if(color==0 or color==1)then
						y=sign.Y+Offset[cell].Y+line*3
					else
						y=sign.Y+Offset[cell].Y+line*3+3
					end
					for bit=0,15,1 do
						x=sign.X+Offset[cell].X+row*33+dir*(bit+1)
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
end