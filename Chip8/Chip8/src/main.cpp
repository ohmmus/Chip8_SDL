#include <SDL.h> 

#include "Chip8.h"

int main(int argc, char* args[])
{
	Chip8 * chip8_Emulator = new Chip8;

	bool continueEmulation = false;

	while (!continueEmulation)
	{
		continueEmulation = chip8_Emulator->Update();
	}

	delete chip8_Emulator;

	return 0;
}