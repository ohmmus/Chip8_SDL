class SDL_Window;
class SDL_Renderer;

const int SCREEN_W = 640;
const int SCREEN_H = 480;

const int CHIP8_W = 64;
const int CHIP8_H = 32;

class Window
{
private: 
	SDL_Window * _window;
	SDL_Renderer * _renderer;

public:
	Window();
	~Window();

	void Draw(const unsigned char * gfxBuffer);
};