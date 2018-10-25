#include <SDL.h>
#include "Window.h"
#include "RAM.h"

//
//void Window::DrawToWindow(unsigned char * gfxBuffer)
//{
//	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
//	SDL_RenderClear(_renderer);
//	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
//	
//	for (int i = 0; i < GFX_RAM; i++)
//	{
//		if (gfxBuffer[i] == 1)
//		{
//			
//		}
//	}
//
//	SDL_RenderDrawPoint(_renderer, 320, 240);
//	SDL_RenderPresent(_renderer);
//	
//}

Window::Window()
{
	_window = SDL_CreateWindow("Chip 8",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_W,
		SCREEN_H,
		SDL_WINDOW_SHOWN);

	if (_window != NULL)
	{
		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}
}

void Window::DrawSprite()
{

}

Window::~Window()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);

	_renderer = NULL;
	_window = NULL;
}
