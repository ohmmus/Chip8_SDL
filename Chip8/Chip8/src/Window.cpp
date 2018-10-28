#include <SDL.h>
#include "Window.h"
#include "RAM.h"

Window::Window()
{
	_window = SDL_CreateWindow("Chip 8",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_W,
		SCREEN_H,
		SDL_WINDOW_SHOWN);

	if (_window != NULL)
	{
		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}

	SDL_RenderSetLogicalSize(_renderer, CHIP8_W, CHIP8_H);
	//SDL_RenderSetIntegerScale(_renderer, SDL_TRUE);
}

Window::~Window()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);

	_renderer = NULL;
	_window = NULL;
}

void Window::Draw(const unsigned char * gfxBuffer)
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
	SDL_RenderClear(_renderer);
	SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);

	// Chip8 display is 64x32
	for (int y = 0; y < CHIP8_H; y++)
	{
		for (int x = 0; x < CHIP8_W; x++)
		{
			if (gfxBuffer[(y * CHIP8_W) + x] == 1) 
			{
				SDL_RenderDrawPoint(_renderer,x, y);
			}
		}
	}

	SDL_RenderPresent(_renderer);
}
