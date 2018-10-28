#include <SDL.h>
#include "Chip8.h"
#include "CPU.h"
#include "Window.h"
#include "RAM.h"
Chip8::Chip8()
{
	_systemRam = new RAM;

	// TEST ONLY
	_systemRam->LoadROM("./res/roms/INVADERS");

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
	
	if (_systemRam->GetDrawFlag())
	{
		_window->Draw(_systemRam->GetGFXMem());
		_systemRam->SetDrawFlag(false);
	}

	SDL_Delay(1000 / 60);

	return continueEmulation;
}

void Chip8::GetKeyDown(int keyDown)
{
	_cpu->ProcessKeyDown(keyDown);
}

void Chip8::GetKeyUp(int keyDown)
{
	_cpu->ProcessKeyUp(keyDown);
}
