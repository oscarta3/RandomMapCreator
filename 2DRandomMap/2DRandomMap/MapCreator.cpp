#include "MapCreator.h"
#include <iostream>
#include "Window.h"
#include <math.h>
#include "ColorsContainer.h"

MapManager::MapManager(Window* const window) : window(window)
{
	CreateMap();
	HeatMap();
	MoistureMap();
}

MapManager::~MapManager()
{

}

void MapManager::CreateMap()
{
	int GRID_SIZE = 250;
	int SIZE = 20;

	_randomNum = rand() % (456457 - 1 + 1) + 1;

	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			float val = 0;

			float freq = 1;
			float amp = 1;

			for (int k = 0; k < 12; k++)
			{
				val += Perlin(i * freq / SIZE, j * freq / SIZE) * amp;

				freq *= 2;
				amp /= 2;
			}

			val *= 1.2;

			if (val > 1.0f)
			{
				val = 1.0f;
			}
			else if (val < -1.0f)
			{
				val = -1.0f;
			}

			MapInfo[i][j] = (int)(((val + 1.0f) * 0.5f) * 255);
		}
	}

	//_island = !_island;


	if (_island)
	{
		ConvertToIsland(MapInfo);
	}

	//DrawMap(MapInfo);
	HeatMap();
	MoistureMap();
	DrawBiomes();
}

void MapManager::pollEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			window->_closed = true;
			break;
		case SDL_KEYDOWN:
			if (SDLK_RIGHT == event.key.keysym.sym)
			{
				CreateMap();
			}
			break;
		default:
			break;
		}
	}
}

void MapManager::DrawMap(int arr[250][250])
{
	int GRID_SIZE = 250;
	int pixelSize = 3;

	for (int m = 0; m < GRID_SIZE; m++)
	{
		for (int n = 0; n < GRID_SIZE; n++)
		{
			RectInfo rect;
			rect.w = pixelSize;
			rect.h = pixelSize;
			rect.x = (window->_width / 2) - (GRID_SIZE * pixelSize) / 2 + n * pixelSize;
			rect.y = (window->_height / 2) - (GRID_SIZE * pixelSize) / 2 + m * pixelSize;

			if (arr[m][n] < 65)
			{
				Color test(56, 103, 175);
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, test);
			}
			else if (arr[m][n] >= 65 && arr[m][n] < 85)
			{
				Color test(65, 124, 201);
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, test);
			}
			else if (arr[m][n] >= 85 && arr[m][n] < 100)
			{
				Color test(103, 172, 214);
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, test);
			}
			else if (arr[m][n] >= 100 && arr[m][n] < 115)
			{
				Color test(242, 223, 152);
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, test);
			}
			else if (arr[m][n] >= 115 && arr[m][n] < 135)
			{
				Color test(183, 183, 71);
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, test);
			}
			else if (arr[m][n] >= 135 && arr[m][n] < 160)
			{
				Color test(122, 166, 71);
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, test);
			}
			else if (arr[m][n] >= 160 && arr[m][n] < 180)
			{
				Color test(102, 136, 59);
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, test);
			}
			else if (arr[m][n] >= 180 && arr[m][n] < 200)
			{
				Color test(63, 73, 75);
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, test);
			}
			else if (arr[m][n] >= 200)
			{
				Color test(74, 89, 100);
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, test);
			}
		}
	}

	window->RenderRect();
}

Vector2 MapManager::RandomGradient(int ix, int iy)
{
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix;
	unsigned b = iy;

	a *= _randomNum;

	b ^= a << s | a >> w - s;
	b *= 1911520717;

	a ^= b << s | b >> w - s;
	a *= 2048419325;
	float random = a * (3.14159265 / ~(~0u >> 1));

	Vector2 v;
	v.x = sin(random);
	v.y = cos(random);

	return v;
}

float MapManager::DotGridGradient(int ix, int iy, float x, float y)
{
	Vector2 gradient = RandomGradient(ix, iy);

	float dx = x - (float)ix;
	float dy = y - (float)iy;

	return (dx * gradient.x + dy * gradient.y);
}

float MapManager::Interpolate(float a0, float a1, float w)
{
	return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

float MapManager::Perlin(float x, float y)
{
	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float sx = x - x0;
	float sy = y - y0;

	float n0 = DotGridGradient(x0, y0, x, y);
	float n1 = DotGridGradient(x1, y0, x, y);
	float ix0 = Interpolate(n0, n1, sx);

	n0 = DotGridGradient(x0, y1, x, y);
	n1 = DotGridGradient(x1, y1, x, y);
	float ix1 = Interpolate(n0, n1, sx);

	return (Interpolate(ix0, ix1, sy));
}

void MapManager::ConvertToIsland(int arr[250][250])
{
	int cx = 250 / 2;
	int cy = 250 / 2;

	float maxDistance = std::sqrt(std::pow(cx, 2) + std::pow(cy, 2));

	for (int x = 0; x < 250; ++x) {
		for (int y = 0; y < 250; ++y) {

			float distance = std::sqrt(std::pow(x - cx, 2) + std::pow(y - cy, 2));

			float attenuation = 1.0f - std::pow(distance / maxDistance, 1.2f);

			arr[x][y] = static_cast<int>(arr[x][y] * attenuation);
			if (arr[x][y] < 0) arr[x][y] = 0;
		}
	}
}

void MapManager::HeatMap()
{

	int GRID_SIZE = 250;
	int SIZE = 70;

	_randomNum = rand() % (456457 - 1 + 1) + 1;

	for (int i = 0; i <= GRID_SIZE; ++i) 
	{
		int heatValue;

		if (i < GRID_SIZE / 2)
		{
			heatValue = 255 * i / (GRID_SIZE / 2);
		}
		else
		{
			heatValue = 255 * (GRID_SIZE- i)/ (GRID_SIZE / 2);
		}
		for (int j = 0; j <= GRID_SIZE; ++j)
		{
			HeatMapInfo[i][j] = heatValue;
		}
	}

	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			if (MapInfo[i][j] >= 180)
			{
				HeatMapInfo[i][j] -= MapInfo[i][j] * 0.2f;
			}
			else if(MapInfo[i][j] >= 160)
			{
				HeatMapInfo[i][j] -= HeatMapInfo[i][j] * 0.1f;
			}
			else if (MapInfo[i][j] <= 100)
			{
				HeatMapInfo[i][j] += HeatMapInfo[i][j] * 0.1f;
			}
			else if (MapInfo[i][j] <= 135)
			{
				HeatMapInfo[i][j] += MapInfo[i][j] * 0.2f;
			}

			if (HeatMapInfo[i][j] < 42)
			{
				HeatMapInfo[i][j] = 0;
			}
			else if (HeatMapInfo[i][j] >= 42 && HeatMapInfo[i][j] < 84)
			{
				HeatMapInfo[i][j] = 1;
			}
			else if (HeatMapInfo[i][j] >= 84 && HeatMapInfo[i][j] < 126)
			{
				HeatMapInfo[i][j] = 2;
			}
			else if (HeatMapInfo[i][j] >= 126 && HeatMapInfo[i][j] < 168)
			{
				HeatMapInfo[i][j] = 3;
			}
			else if (HeatMapInfo[i][j] >= 168 && HeatMapInfo[i][j] < 210)
			{
				HeatMapInfo[i][j] = 4;
			}
			else if (HeatMapInfo[i][j] >= 210)
			{
				HeatMapInfo[i][j] = 5;
			}
		}
	}

	//DrawHeatMap(HeatMapInfo);
}

void MapManager::MoistureMap()
{
	int GRID_SIZE = 250;
	int SIZE = 70;

	_randomNum = rand() % (456457 - 1 + 1) + 1;

	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			float val = 0;

			float freq = 1;
			float amp = 1;

			for (int k = 0; k < 12; k++)
			{
				val += Perlin(i * freq / SIZE, j * freq / SIZE) * amp;

				freq *= 2;
				amp /= 2;
			}

			val *= 1.2;

			if (val > 1.0f)
			{
				val = 1.0f;
			}
			else if (val < -1.0f)
			{
				val = -1.0f;
			}

			MoistureMapInfo[i][j] = (int)(((val + 1.0f) * 0.5f) * 255);

			/*if (MapInfo[i][j] >= 180)
			{
				MoistureMapInfo[i][j] -= MoistureMapInfo[i][j] * 5;
			}
			else if (MapInfo[i][j] <= 100)
			{
				MoistureMapInfo[i][j] += MapInfo[i][j] * 5;
			}*/

			if (MoistureMapInfo[i][j] < 42)
			{
				MoistureMapInfo[i][j] = 0;
			}
			else if (MoistureMapInfo[i][j] >= 42 && MoistureMapInfo[i][j] < 84)
			{
				MoistureMapInfo[i][j] = 1;
			}
			else if (MoistureMapInfo[i][j] >= 84 && MoistureMapInfo[i][j] < 126)
			{
				MoistureMapInfo[i][j] = 2;
			}
			else if (MoistureMapInfo[i][j] >= 126 && MoistureMapInfo[i][j] < 168)
			{
				MoistureMapInfo[i][j] = 3;
			}
			else if (MoistureMapInfo[i][j] >= 168 && MoistureMapInfo[i][j] < 210)
			{
				MoistureMapInfo[i][j] = 4;
			}
			else if (MoistureMapInfo[i][j] >= 210)
			{
				MoistureMapInfo[i][j] = 5;
			}
		}
	}

	//DrawMoistureMap(MoistureMapInfo);
}

void MapManager::DrawBiomes()
{
	int GRID_SIZE = 250;
	int pixelSize = 3;

	for (int m = 0; m < GRID_SIZE; m++)
	{
		for (int n = 0; n < GRID_SIZE; n++)
		{
			RectInfo rect;
			rect.w = pixelSize;
			rect.h = pixelSize;
			rect.x = (window->_width / 2) - (GRID_SIZE * pixelSize) / 2 + n * pixelSize;
			rect.y = (window->_height / 2) - (GRID_SIZE * pixelSize) / 2 + m * pixelSize;

			TerrainType temp;

			if (MapInfo[m][n] < 65)
			{
				temp = DEEPDEEP_WATER;
			}
			else if (MapInfo[m][n] >= 65 && MapInfo[m][n] < 85)
			{
				temp = DEEP_WATER;
			}
			else if (MapInfo[m][n] >= 85 && MapInfo[m][n] < 100)
			{
				temp = WATER;
			}
			else if (MapInfo[m][n] >= 100 && MapInfo[m][n] < 115)
			{
				temp = BEACH;
			}
			else if (MapInfo[m][n] >= 115 && MapInfo[m][n] < 135)
			{
				temp = LOW_GRASS;
			}
			else if (MapInfo[m][n] >= 135 && MapInfo[m][n] < 160)
			{
				temp = GRASS;
			}
			else if (MapInfo[m][n] >= 160 && MapInfo[m][n] < 180)
			{
				temp = HIGH_GRASS;
			}
			else if (MapInfo[m][n] >= 180 && MapInfo[m][n] < 200)
			{
				temp = MOUNTAIN;
			}
			else if (MapInfo[m][n] >= 200)
			{
				temp = HIGH_MOUNTAIN;
			}

			if (BiomeTable[MoistureMapInfo[m][n]][HeatMapInfo[m][n]] == 0)
			{
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, DesertColors[temp]);
			}
			else if (BiomeTable[MoistureMapInfo[m][n]][HeatMapInfo[m][n]] == 1)
			{
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, SavannaColors[temp]);
			}
			else if (BiomeTable[MoistureMapInfo[m][n]][HeatMapInfo[m][n]] == 2)
			{
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, JungleColors[temp]);
			}
			else if (BiomeTable[MoistureMapInfo[m][n]][HeatMapInfo[m][n]] == 3)
			{
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, GrasslandColors[temp]);
			}
			else if (BiomeTable[MoistureMapInfo[m][n]][HeatMapInfo[m][n]] == 4)
			{
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, WoodlandColors[temp]);
			}
			else if (BiomeTable[MoistureMapInfo[m][n]][HeatMapInfo[m][n]] == 5)
			{
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, BorealColors[temp]);
			}
			else if (BiomeTable[MoistureMapInfo[m][n]][HeatMapInfo[m][n]] == 6)
			{
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, TundraColors[temp]);
			}
			else if (BiomeTable[MoistureMapInfo[m][n]][HeatMapInfo[m][n]] == 7)
			{
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, IceColors[temp]);
			}
			else if (BiomeTable[MoistureMapInfo[m][n]][HeatMapInfo[m][n]] == 8)
			{
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, DesertColors[temp]);
			}
			else if (BiomeTable[MoistureMapInfo[m][n]][HeatMapInfo[m][n]] == 9)
			{
				window->DrawRect(rect.w, rect.h, rect.x, rect.y, DesertColors[temp]);
			}
		}
	}

	window->RenderRect();
}
