#include <SDL.h> 

#include "Chip8.h"

int main(int argc, char* args[])
{
	Chip8 * chip8_Emulator = new Chip8;

	bool continueEmulation = true;

	SDL_Event e;

	while (continueEmulation)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				case SDL_QUIT: 
				case SDL_KEYDOWN: 
					continueEmulation = false; 
				break;

				default: continueEmulation = true;
			}
		}

		continueEmulation |= chip8_Emulator->Update();
	}

	delete chip8_Emulator;

	return 0;
}