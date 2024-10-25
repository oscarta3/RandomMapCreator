#pragma once

typedef struct {
	int w, h, x, y;
} RectInfo;

struct Color {
	int r, g, b;
	Color(int red, int green, int blue) : r(red), g(green), b(blue) {}
};

enum Biome {
	DESERT = 0,
	SAVANNA,
	JUNGLE,
	GRASSLAND,
	WOODLAND,
	BOREALFOREST,
	TUNDRA,
	ICE
};

enum TerrainType {
	DEEPDEEP_WATER = 0,
	DEEP_WATER,
	WATER,
	BEACH,
	LOW_GRASS,
	GRASS,
	HIGH_GRASS,
	MOUNTAIN,
	HIGH_MOUNTAIN
};

struct PixelInfo {
	Biome biome;
	TerrainType type;
	RectInfo info;
};