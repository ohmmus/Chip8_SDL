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
	void ClearSysMem();

	unsigned char * GetGFXMem() { return &_gfxMem[0]; }
	unsigned char * GetSysMem() { return &_memory[0]; }

public:
	RAM();
	~RAM();

	void LoadROM(const char * fileName);
	unsigned short GetWordSys(unsigned short addressLoc);
	unsigned char GetByteSys(unsigned char addressLoc);
	void SetWordSys(unsigned short addressLoc, unsigned short data);
	void SetByteSys(unsigned short addressLoc, unsigned char data);

	unsigned short GetWordGfx(unsigned char addressLoc);
	unsigned char GetByteGfx(unsigned char addressLoc);
	void SetByteGfx(unsigned char addressLoc, unsigned char data);
};