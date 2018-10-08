#pragma once

class RAM;

class CPU
{
private: 
	
	unsigned char _registers[16];
	unsigned char _delay_timer;
	unsigned char _sound_timer;

	unsigned short _indexRegister;
	unsigned short _programCounter;

	unsigned short _stack[16];
	unsigned short _stackPointer;

	unsigned short _key[16];
	RAM * _systemRam;


public:
	CPU();
	~CPU();

	void Initialize();
	void Emulate();
	
private: 
	unsigned short FetchNextOp();
	void Decode(unsigned short opcode);
};