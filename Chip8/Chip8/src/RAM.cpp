#include "RAM.h"
#include <stdio.h>
#include <iostream>

RAM::RAM()
{
	_drawFlag = true;
}


RAM::~RAM()
{
}

void RAM::LoadROM(const char * fileName)
{
	char buffer[TOTAL_RAM];

	FILE * fp = nullptr;
	fopen_s(&fp, fileName, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	rewind(fp);

	fread(buffer, 1, fsize, fp); 

	std::cout << "Read in: " << fsize << " bytes." << std::endl;

	for (int i = 0; i < fsize; i++)
	{
		_memory[512 + i] = buffer[i];
	}

	fclose(fp);
}

unsigned short RAM::GetWordSys(unsigned short addressLoc)
{
	if (addressLoc > (TOTAL_RAM - 1))
	{
		return 0;
	}

	return (_memory[addressLoc] << 8) | (_memory[addressLoc + 1]);
}

unsigned char RAM::GetByteSys(unsigned char addressLoc)
{
	if (addressLoc >= TOTAL_RAM)
	{
		return 0;
	}

	return _memory[addressLoc];
}

void RAM::SetWordSys(unsigned short addressLoc, unsigned short data)
{
	_memory[addressLoc] = ((data >> 8) & (0x00FF)); 
	_memory[addressLoc + 1] = ((data) & (0x00FF));
}

void RAM::SetByteSys(unsigned char addressLoc, unsigned char data)
{
	_memory[addressLoc] = data;

}

unsigned short RAM::GetWordGfx(unsigned char addressLoc)
{
	if (addressLoc > (GFX_RAM - 1))
	{
		return 0;
	}

	return (_gfxMem[addressLoc] << 8) | (_gfxMem[addressLoc + 1]);
}

unsigned char RAM::GetByteSysGfx(unsigned char addressLoc)
{
	if (addressLoc >= GFX_RAM)
	{
		return 0;
	}

	return _gfxMem[addressLoc];
}

void RAM::SetByteGfx(unsigned char addressLoc, unsigned char data)
{
	_gfxMem[addressLoc] = data;
}
