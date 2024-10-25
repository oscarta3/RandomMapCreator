#pragma once

#include "PixelInfo.h"

typedef struct {
	float x, y;
} Vector2;


class Window;

class MapManager {
public:
	MapManager(Window* const);
	~MapManager();

	void ConvertToIsland(int arr[250][250]);

	void pollEvents();

	void CreateMap();
	void HeatMap();
	void DrawHeatMap(int arr[250][250]);
	void MoistureMap();
	void DrawMoistureMap(int arr[250][250]);
	void DrawBiomes();

	Vector2 RandomGradient(int ix, int iy);
	float DotGridGradient(int ix, int iy, float x, float y);
	float Interpolate(float a0, float a1, float w);

	void DrawMap(int arr[250][250]);

	void AddBiomes();

	float Perlin(float x, float y);

private:
	int _randomNum;
	bool _island = false;

	Window* const window;

	int MapInfo[250][250];
	int HeatMapInfo[250][250];
	int MoistureMapInfo[250][250];
	PixelInfo pixel[10][10];

	Biome BiomeTable[6][6] = {
		//COLDEST     //COLDER       //COLD               //HOT             //HOTTER        //HOTTEST
		{ Biome::ICE, Biome::TUNDRA, Biome::GRASSLAND,    Biome::DESERT,    Biome::DESERT,  Biome::DESERT },  //DRYEST
		{ Biome::ICE, Biome::TUNDRA, Biome::GRASSLAND,    Biome::DESERT,    Biome::DESERT,  Biome::DESERT },  //DRYER
		{ Biome::ICE, Biome::TUNDRA, Biome::WOODLAND,     Biome::WOODLAND,  Biome::SAVANNA, Biome::SAVANNA }, //DRY
		{ Biome::ICE, Biome::TUNDRA, Biome::BOREALFOREST, Biome::WOODLAND,  Biome::SAVANNA, Biome::SAVANNA }, //WET
		{ Biome::ICE, Biome::TUNDRA, Biome::BOREALFOREST, Biome::JUNGLE,	Biome::JUNGLE,  Biome::JUNGLE },  //WETTER
		{ Biome::ICE, Biome::TUNDRA, Biome::BOREALFOREST, Biome::JUNGLE,	Biome::JUNGLE,  Biome::JUNGLE }   //WETTEST
	};
};