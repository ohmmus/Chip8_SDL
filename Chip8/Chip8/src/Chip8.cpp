#include "Chip8.h"
#include "CPU.h"
#include "Window.h"
#include "RAM.h"
Chip8::Chip8()
{
	_systemRam = new RAM;

	// TEST ONLY
	_systemRam->LoadROM("./res/roms/PONG");

	_cpu = new CPU(_systemRam);
	_cpu->Initialize();

	_window = new Window();
}

Chip8::~Chip8()
{
	delete _cpu;
	delete _systemRam;
}

bool Chip8::Update()
{
	bool continueEmulation = false;
	_cpu->Emulate();

	return continueEmulation;
}
