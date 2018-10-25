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
	Chip8();
	~Chip8();

	bool Update();
};

