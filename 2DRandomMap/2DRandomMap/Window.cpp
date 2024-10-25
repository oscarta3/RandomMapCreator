#include "Window.h"
#include <iostream>
#include <iomanip>

Window::Window(const std::string& title, int widht, int height) : _title(title), _width(widht), _height(height) 
{
	_closed = !Init();
}

Window::~Window()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

bool Window::Init() 
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "Failed to initialize SDL. \n";
		return 0;
	}

	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_RESIZABLE);

	if (_window == nullptr) {
		std::cerr << "Failed to create window. \n";
		return 0;
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (_renderer == nullptr) {
		std::cerr << "Failed to create renderer. \n";
		return 0;
	}

	return true;
}

void Window::Clear() 
{
	
	SDL_RenderClear(_renderer);
	SDL_RenderPresent(_renderer);
}

void Window::DrawRect(int w, int h, int x, int y, Color color)
{
	SDL_Rect rect;
	rect.w = w;
	rect.h = h;
	rect.x = x;
	rect.y = y;

	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(_renderer, &rect);
}

void Window::RenderRect()
{
	SDL_RenderPresent(_renderer);
}

void Window::ClearMap()
{
	SDL_RenderClear(_renderer);
	SDL_RenderPresent(_renderer);
}
