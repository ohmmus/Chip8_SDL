#include <SDL.h>
#include "Chip8.h"
#include "CPU.h"
#include "Window.h"
#include "RAM.h"

#include <iostream>

Chip8::Chip8(const char * fileName)
{
	_systemRam = new RAM;


	_cpu = new CPU(_systemRam);
	_cpu->Initialize();

	_systemRam->LoadROM(fileName);

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

	return continueEmulation;
}

void Chip8::GetKeyDown(int keyDown)
{
	_cpu->ProcessKeyDown(keyDown);
	//std::cout << "Keydown " << keyDown << std::endl;
}

void Chip8::GetKeyUp(int keyup)
{
	_cpu->ProcessKeyUp(keyup);
	//std::cout << "Keyup " << keyup << std::endl;

}
