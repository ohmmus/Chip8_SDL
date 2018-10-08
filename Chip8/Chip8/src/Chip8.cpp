#include "Chip8.h"

Chip8::Chip8()
{
	_cpu = new CPU;
	_cpu->Initialize();
}

Chip8::~Chip8()
{
	delete _cpu;
}

bool Chip8::Update()
{
	bool continueEmulation = false;
	_cpu->Emulate();

	return continueEmulation;
}
