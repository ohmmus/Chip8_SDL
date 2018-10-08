// Console representation class
#pragma once

#include "CPU.h"

class Chip8
{
private: 
	CPU * _cpu;
	bool drawFlag;

public:
	Chip8();
	~Chip8();

	bool Update();
};

