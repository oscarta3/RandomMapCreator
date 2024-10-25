#pragma once

#include <string>
#include <SDL.h>
#include "PixelInfo.h"

class Window {
public:
	Window(const std::string& title, int widht, int height);
	~Window();

	void Clear();
	void RenderRect();
	void DrawRect(int w, int h, int x, int y, Color color);

	void ClearMap();

	inline bool isClosed() const { return _closed; };

private:
	bool Init();

private: 
	std::string _title;

	int _randomNum;

	bool _island = false;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

public:
	bool _closed = false;
	int _width = 800;
	int _height = 800;
};
