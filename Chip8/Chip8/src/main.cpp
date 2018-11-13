#include <SDL.h> 

#include "Chip8.h"
#include <iostream>

int main(int argc, char* args[])
{
	for (int i = 0; i < argc; i++)
	{
		std::cout << args[i] << std::endl;
	}

	Chip8 * chip8_Emulator = nullptr;

	if (argc == 1)
	{
		chip8_Emulator = new Chip8("./res/roms/BC_TEST");
	}
	else
	{
		chip8_Emulator = new Chip8(args[1]);
	}

	bool continueEmulation = true;

	SDL_Event e;

	while (continueEmulation)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT: 
					continueEmulation = false; 
				break;
				case SDL_KEYDOWN:
					chip8_Emulator->GetKeyDown(e.key.keysym.sym);
					break;
				case SDL_KEYUP: 
					chip8_Emulator->GetKeyUp(e.key.keysym.sym);
					break;
				default: continueEmulation = true;
			}
		}

		continueEmulation |= chip8_Emulator->Update();
	}

	delete chip8_Emulator;

	return 0;
}