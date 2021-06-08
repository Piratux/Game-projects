#include "AssetManager.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

AssetManager::AssetManager()
{
	LoadSpritesheets();
	LoadPremadeAnimations();
	LoadMap();

	std::cout << "Assets loaded succesfully\n";
}
void AssetManager::LoadSpritesheets()
{
	auto load_folder_sprites = [&](const std::string& path)
	{
		for (const auto& entry : fs::directory_iterator(path))
		{
			std::string path(fs::path(entry).u8string());

			// check if it's of type ".png"
			if (path.find(".png") != std::string::npos)
			{
				std::cout << "Loading: " << path << '\n';
				size_t from_idx = std::max(path.rfind('\\'), path.rfind('/')) + 1;
				size_t count = path.rfind('.') - from_idx;

				sprites[path.substr(from_idx, count)] = std::make_unique<olc::Renderable>();
				sprites[path.substr(from_idx, count)]->Load(path);
			}
		}
	};

	load_folder_sprites("./Assets/Spritesheets");
	load_folder_sprites("./Assets/Projectiles");
}
void AssetManager::LoadPremadeAnimations()
{
	animation_sheets[Animation::Type::Hero_10x5] = std::make_unique<Animation::Premade>(olc::vi2d{ 10, 5 }, Animation::Name::Idle);
	animation_sheets[Animation::Type::Hero_10x5]->AddAnimation(Animation::Name::Idle,    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, 4);
	animation_sheets[Animation::Type::Hero_10x5]->AddAnimation(Animation::Name::Special, { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 }, 16, false);
	animation_sheets[Animation::Type::Hero_10x5]->AddAnimation(Animation::Name::Walk,    { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 }, 12);
	animation_sheets[Animation::Type::Hero_10x5]->AddAnimation(Animation::Name::Attack,  { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 }, 16, false);
	animation_sheets[Animation::Type::Hero_10x5]->AddAnimation(Animation::Name::Death,   { 40, 41, 42, 43, 44, 45, 46, 47, 48, 49 }, 8, false);

	animation_sheets[Animation::Type::Hero_10x5]->SetTransitionalAnimation(Animation::Name::Special, Animation::Name::Idle);
	animation_sheets[Animation::Type::Hero_10x5]->SetTransitionalAnimation(Animation::Name::Attack, Animation::Name::Idle);


	animation_sheets[Animation::Type::Icicle_8x1] = std::make_unique<Animation::Premade>(olc::vi2d{ 8, 1 }, Animation::Name::Attack);
	animation_sheets[Animation::Type::Icicle_8x1]->AddAnimation(Animation::Name::Attack, { 0, 1, 2, 3, 4, 5, 6, 7 }, 12);
}
void AssetManager::LoadMap()
{
	std::cout << "Loading map data\n";

	// find "data" fields in .json
	std::ifstream map_data("./Assets/Map/map.json");
	if (map_data.is_open() == false)
		throw std::logic_error("AssetManager::LoadMap: couldn't open \"./Assets/Map/map.json\"");

	tilemap = std::make_unique<olc::Renderable>();
	tilemap->Load("./Assets/Map/tilesheet.png");
	if (tilemap == nullptr)
		throw std::logic_error("AssetManager::LoadMap: \"./Assets/Map/tilesheet.png\" file not found");

	std::string temp_line;
	char total_data_fields_found = 0;
	bool search_for_width_and_height = true;
	while (std::getline(map_data, temp_line))
	{
		if (search_for_width_and_height)
		{
			auto read_field = [](const std::string& line, int32_t& field)
			{
				std::stringstream ss(line);
				ss.ignore(100, ':');
				ss >> field;
			};

			if (temp_line.find("\"width\":") != std::string::npos)
				read_field(temp_line, map_size.x);
			else if (temp_line.find("\"height\":") != std::string::npos)
				read_field(temp_line, map_size.y);
			else if (temp_line.find("\"tilewidth\":") != std::string::npos)
				read_field(temp_line, tile_size.x);
			else if (temp_line.find("\"tileheight\":") != std::string::npos)
				read_field(temp_line, tile_size.y);
			else if (temp_line.find("\"layers\":") != std::string::npos)
				search_for_width_and_height = false;
			else if (temp_line.find("\"tilesets\":") != std::string::npos)
				search_for_width_and_height = false;
		}
		else
		{
			if (temp_line.find("}]") != std::string::npos)
				search_for_width_and_height = true;

			// if data field found read all the data into vector
			else if (temp_line.find("\"data\":") != std::string::npos)
			{
				// the order of data fields depends on order in ".json" file
				std::vector<int32_t>* tilemap_layer;

				switch ((MapLayer)total_data_fields_found)
				{
				case MapLayer::Floor:
					tilemap_layer = &tilemap_floor;
					break;
				case MapLayer::Floor2:
					tilemap_layer = &tilemap_floor2;
					break;
				case MapLayer::Walls:
					tilemap_layer = &tilemap_walls;
					break;
				case MapLayer::Trees:
					tilemap_layer = &tilemap_trees;
					break;
				default: // should never happen
					throw std::logic_error("AssetManager::LoadMap: layer is unknown");
				}

				total_data_fields_found++;

				// prescale to avoid reallocations
				tilemap_layer->reserve((size_t)map_size.x * (size_t)map_size.y);

				// reading part begins here
				std::stringstream ss(temp_line);
				ss.ignore(100, ':');
				while (ss.peek() != ']')
				{
					ss.ignore(1);

					// I have to read it into size_t first and then cast it, because 
					// stringstream doesn't read number into int32_t directly properly
					size_t tile_idx;
					ss >> tile_idx;
					tilemap_layer->push_back((int32_t)tile_idx);
				}
			}
		}
	}

	if (map_size.x == 0)
		throw std::logic_error("AssetManager::LoadMap: width field not found or it's 0");
	if (map_size.y == 0)
		throw std::logic_error("AssetManager::LoadMap: height field not found or it's 0");
	if (tile_size.x == 0)
		throw std::logic_error("AssetManager::LoadMap: tilewidth field not found or it's 0");
	if (tile_size.y == 0)
		throw std::logic_error("AssetManager::LoadMap: tileheight field not found or it's 0");

	tile_count.x = tilemap->Sprite()->width / tile_size.x;
	tile_count.y = tilemap->Sprite()->height / tile_size.y;

	map_data.close();
}
olc::Renderable* AssetManager::GetRenderable(const std::string& name)
{
	return sprites[name].get();
}
Animation::Premade* AssetManager::GetAnimationSheet(Animation::Type type)
{
	return animation_sheets[type].get();
}