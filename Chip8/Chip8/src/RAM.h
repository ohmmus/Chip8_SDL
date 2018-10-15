#pragma once
#define TOTAL_RAM 4096
#define GFX_RAM 2048

class RAM
{
private: 
	unsigned char _memory[TOTAL_RAM];
	unsigned char _gfxMem[GFX_RAM];
	bool _drawFlag;

	// Properties
public: 
	bool GetDrawFlag() { return _drawFlag; }
	void SetDrawFlag(bool draw) { _drawFlag = draw; }
	void ClearGFXMem();

public:
	RAM();
	~RAM();

	void LoadROM(const char * fileName);
	unsigned short GetWordSys(unsigned short addressLoc);
	unsigned char GetByteSys(unsigned char addressLoc);
	void SetWordSys(unsigned short addressLoc, unsigned short data);
	void SetByteSys(unsigned char addressLoc, unsigned char data);


	unsigned short GetWordGfx(unsigned char addressLoc);
	unsigned char GetByteSysGfx(unsigned char addressLoc);
	void SetByteGfx(unsigned char addressLoc, unsigned char data);
};

