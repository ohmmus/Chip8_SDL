// Console representation class
#pragma once

class CPU;
class Window;
class RAM;

class Chip8
{
private: 
	RAM * _systemRam;
	CPU * _cpu;
	Window * _window;

	bool drawFlag;

public:
	Chip8(const char * fileName);
	~Chip8();

	bool Update();
	void GetKeyDown(int keyDown);
	void GetKeyUp(int keyDown);
};

