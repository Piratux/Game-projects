// AssetManager is responsible for loading all the data in the game such as:
// - spritesheets
// - map data
// - animation frame indexes for spritesheets

// This class can only recognise .png files that are used as spritesheets only in 
// "./Assets/Spritesheets" folder

// To load a map you have to do as follows:
// - Create map with Tiled
// - Make the map have finite amount of tiles
// - Add 4 tile layers in this order:
// -- Trees (everything that is rendered on top)
// -- Walls (collision layer)
// -- Floor2 (background decorations)
// -- Floor (rendered in the background)
// - Export the map in .json format and rename it to "map.json"
// - Put the "map.json", "tilesheet.png" inside "./Assets/Map" folder

#pragma once
#include "olcPixelGameEngine.h"
#include "Animation.h"

class AssetManager
{
public:
	enum class AssetType
	{
		Player, Enemy
	};
	enum class MapLayer : char
	{
		Floor = 0, Floor2, Walls, Trees
	};
public:
	std::map<std::string, std::unique_ptr<olc::Renderable>> sprites;
	std::map<Animation::Type, std::unique_ptr<Animation::Premade>> animation_sheets;

	olc::vi2d tile_size; // in pixels
	olc::vi2d map_size; // in tiles
	olc::vi2d tile_count; // total tiles in x and y direction in tilesheet

	std::unique_ptr<olc::Renderable> tilemap;
	std::vector<int32_t> tilemap_floor;
	std::vector<int32_t> tilemap_floor2;
	std::vector<int32_t> tilemap_walls;
	std::vector<int32_t> tilemap_trees;

public:
	AssetManager();
	olc::Renderable* GetRenderable(const std::string& name);
	Animation::Premade* GetAnimationSheet(Animation::Type type);

private:
	// loads sprites from "./Assets/Spritesheets" folder
	void LoadSpritesheets();
	void LoadPremadeAnimations();
	// load map data from "./Assets/Map" folder
	void LoadMap();
};

