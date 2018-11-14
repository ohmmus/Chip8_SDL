#pragma once

class RAM;

class CPU
{
private: 
	const unsigned char VFIndex = 0xF;
	const unsigned int _numRegisters = 16;

	unsigned char _registers[16];
	unsigned char _delay_timer;
	unsigned char _sound_timer;

	unsigned short _indexRegister;
	unsigned short _programCounter;

	unsigned short _stack[16];
	unsigned short _stackPointer;

	// down = 1, up = 0
	unsigned char _keyPress[16]; 

	RAM * _systemRamInstance;

	const unsigned char chip8_fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	unsigned int timeLastTick;

public:
	CPU(RAM * systemRamInstance);
	~CPU();

	void Initialize();
	void Emulate();
	void ProcessKeyDown(int key);
	void ProcessKeyUp(int key);

private: 
	unsigned short FetchNextOp();
	void Decode(unsigned short opcode);

	// Opcode handlers
	void ClearScreen();
	void ReturnFromSubroutine();
	void JumpToLocation(unsigned short opcode);
	void CallSubroutineAtLocation(unsigned short opcode);
	void SkipIfRegEqualVal(unsigned short opcode);
	void SkipIfRegNotEqualVal(unsigned short opcode);
	void SkipIfRegEqualReg(unsigned short opcode);
	void LoadValToReg(unsigned short opcode);
	void Add(unsigned short opcode);
	void RegisterOps(unsigned short opcode);
	void SkipNextIfNotEqu(unsigned short opcode);
	void SetIndexRegister(unsigned short opcode);
	void Jump(unsigned short opcode);
	void SetRandWithValue(unsigned short opcode);
	void DisplaySprite(unsigned short opcode);
	void SkipNextInstIf(unsigned short opcode);
	void LoadDelayTimerVal(unsigned short opcode);
	void WaitForKeyPress(unsigned short opcode);
	void SetDelayTimerVal(unsigned short opcode);
	void SetSoundTimerVal(unsigned short opcode);
	void AddToIndex(unsigned short opcode);
	void SetSpriteLoc(unsigned short opcode);
	void StoreBCDRepresentation(unsigned short opcode);
	void LoadIntoMemory(unsigned short opcode);
	void LoadIntoRegisters(unsigned short opcode);
};