#include "Display.h"
#include "Window.h"

Display::Display()
{
	_window = new Window();
}

Display::~Display()
{
	delete _window;
}
