#include "EntityManager.h"

EntityManager::EntityManager(olc::PixelGameEngine* pge, AssetManager* asset_manager, PhysicsEngine* physics_engine)
	: pge(pge),
	asset_manager(asset_manager),
	physics_engine(physics_engine)
{
	AddMapWallColliders();
	CreatePlayer();
	CreateEnemies();
}
void EntityManager::Update(float delta)
{
	// y-sort entities
	YsortEntities();

	for(size_t i = 0; i < entities.size(); ++i)
	{
		// if entity needs to be deleted
		if (entities[i]->Update(delta) == false)
		{
			std::swap(entities[i], entities.back());
			entities.pop_back();
			--i;
			continue;
		}
	}

	camera->Update();

	DrawEverything();
}
void EntityManager::CreatePlayer()
{
	//CreateEntity<Player>(olc::vf2d{ 10.0f * 16.0f, 240.0f * 16.0f });
	CreateEntity<Player>(olc::vf2d{ 48.0f * 16.0f, 193.0f * 16.0f });
	SetLastCreatedEntityAnimation("Wizard4", Animation::Type::Hero_10x5);
	AddLastCreatedEntityCircleCollider(b2BodyType::b2_dynamicBody, 10.0f, 0.0f, 6.0f, Entity::ColliderFlag::Player, Entity::ColliderFlag::Wall);

	// this should be right after player creation
	olc::vi2d camera_size = { pge->ScreenWidth(), pge->ScreenHeight() };
	camera = std::make_unique<Camera>(camera_size, entities.back()->pos - camera_size / 2);
	camera->follow = entities.back().get();
}
void EntityManager::CreateEnemies()
{
	// temporary
	auto create_snake = [&](olc::vf2d tile_pos)
	{
		CreateEntity<Enemy>(olc::vf2d{ tile_pos.x * 16.0f, tile_pos.y * 16.0f });
		SetLastCreatedEntityAnimation("Snake", Animation::Type::Hero_10x5);
		AddLastCreatedEntityCircleCollider(b2BodyType::b2_dynamicBody, 10.0f, 0.0f, 6.0f, Entity::ColliderFlag::Enemy, Entity::ColliderFlag::Wall | Entity::ColliderFlag::PlayerProjectile);
	};
	
	for(size_t i = 0; i < 50; ++i)
		create_snake({ 60.0f + rand() % 70, 170.0f + rand() % 70 });
}

void EntityManager::SetLastCreatedEntityAnimation(const std::string& spritesheet_name, Animation::Type type)
{
	entities.back()->SetAnimation(asset_manager->GetRenderable(spritesheet_name), asset_manager->GetAnimationSheet(type));
}
void EntityManager::AddLastCreatedEntityRectangleCollider(b2BodyType body_type, float collider_size_x, float collider_size_y, float offset_x, float offset_y, uint16_t category, uint16_t mask)
{
	entities.back()->AddRectangleCollider(physics_engine, body_type, collider_size_x, collider_size_y, offset_x, offset_y, category, mask);
}
void EntityManager::AddLastCreatedEntityCircleCollider(b2BodyType body_type, float radius, float offset_x, float offset_y, uint16_t category, uint16_t mask)
{
	entities.back()->AddCircleCollider(physics_engine, body_type, radius, offset_x, offset_y, category, mask);
}

void EntityManager::DrawEverything()
{
	pge->Clear(olc::DARK_BLUE);
	pge->SetPixelMode(olc::Pixel::MASK);

	DrawTilemap(AssetManager::MapLayer::Floor);
	DrawTilemap(AssetManager::MapLayer::Floor2);
	DrawTilemap(AssetManager::MapLayer::Walls);

	for (auto& entity : entities)
	{
		entity->DrawEntity(camera->pos);
	}

	DrawTilemap(AssetManager::MapLayer::Trees);

	pge->SetPixelMode(olc::Pixel::NORMAL);
}
void EntityManager::DrawTilemap(AssetManager::MapLayer layer)
{
	std::vector<int32_t>* tilemap_layer;
	switch (layer)
	{
	case AssetManager::MapLayer::Floor:
		tilemap_layer = &asset_manager->tilemap_floor;
		break;
	case AssetManager::MapLayer::Floor2:
		tilemap_layer = &asset_manager->tilemap_floor2;
		break;
	case AssetManager::MapLayer::Walls:
		tilemap_layer = &asset_manager->tilemap_walls;
		break;
	case AssetManager::MapLayer::Trees:
		tilemap_layer = &asset_manager->tilemap_trees;
		break;
	default: // should never happen
		throw std::logic_error("EntityManager::DrawTilemap: layer is unknown");
		break;
	}

	// calculate which part of map player can actually see
	int32_t height_start = (int32_t)camera->pos.y / asset_manager->tile_size.y;
	int32_t width_start = (int32_t)camera->pos.x / asset_manager->tile_size.x;
	if (height_start < 0)
		height_start = 0;
	if (width_start < 0)
		width_start = 0;

	int32_t height_end = height_start + 2 + camera->size.y / asset_manager->tile_size.y;
	int32_t width_end = width_start + 2 + camera->size.x / asset_manager->tile_size.x;
	if (height_end > asset_manager->map_size.y)
		height_end = asset_manager->map_size.y;
	if (width_end > asset_manager->map_size.x)
		width_end = asset_manager->map_size.x;

	// iterate over the tilemap and draw each tile
	for (int32_t height_idx = height_start; height_idx < height_end; ++height_idx)
	{
		for (int32_t width_idx = width_start; width_idx < width_end; ++width_idx)
		{
			int32_t index = asset_manager->map_size.x * height_idx + width_idx;
			DrawTile({ width_idx * asset_manager->tile_size.x, height_idx * asset_manager->tile_size.y }, (*tilemap_layer)[index]);
		}
	}
}
void EntityManager::DrawTile(olc::vi2d tile_pos, int32_t tile_idx)
{
	// 0 means empty tile
	if (tile_idx == 0)
		return;

	// Read tile flip information
	// - 1st to last: horizontal flip bit
	// - 2nd to last: vertical flip bit
	// - 3rd to last: diagonal flip bit (from top left to bottom right) (or horizontal flip + left rotation)

	bool horizontal_flip = (tile_idx & 0x80000000);
	bool vertical_flip = (tile_idx & 0x40000000);
	bool diagonal_flip = (tile_idx & 0x20000000);

	// now that we got flip information, clear the flip bits
	tile_idx &= 0x1FFFFFFF;


	olc::vi2d tilesheet_pos;

	// -1 because indexes start from 1 here since 0 is reserved for empty tile
	tilesheet_pos.x = ((tile_idx - 1) % asset_manager->tile_count.x) * asset_manager->tile_size.x;
	tilesheet_pos.y = ((tile_idx - 1) / asset_manager->tile_count.x) * asset_manager->tile_size.y;

	// calculate tile rotation and flip
	olc::vf2d scale;
	float angle;
	if (diagonal_flip)
	{
		if (horizontal_flip && vertical_flip)
		{
			scale.x = 1.0f;
			scale.y = -1.0f;
			angle = -3.14f / 2.0f;
		}
		else if (!horizontal_flip && vertical_flip)
		{
			scale.x = 1.0f;
			scale.y = 1.0f;
			angle = -3.14f / 2.0f;
		}
		else if (horizontal_flip && !vertical_flip)
		{
			scale.x = 1.0f;
			scale.y = 1.0f;
			angle = 3.14f / 2.0f;
		}
		else if (!horizontal_flip && !vertical_flip)
		{
			scale.x = 1.0f;
			scale.y = -1.0f;
			angle = 3.14f / 2.0f;
		}
	}
	else
	{
		scale.x = horizontal_flip ? -1.0f : 1.0f;
		scale.y = vertical_flip ? -1.0f : 1.0f;
		angle = 0;
	}

	pge->DrawPartialRotatedDecal(
		pge->NormaliseToScreenSpace(-camera->pos) + tile_pos + asset_manager->tile_size / 2, // render pos
		asset_manager->tilemap->Decal(), // decal ptr
		angle, // angle
		asset_manager->tile_size / 2, // rotation center (and also acts as -offset)
		tilesheet_pos, // source pos
		asset_manager->tile_size, // source size
		scale); // scale
}

void EntityManager::AddMapWallColliders()
{	
	// TODO: replace "asset_manager->tilemap_walls[index]" with get function
	auto create_tile_hitbox = [&](int32_t x_idx, int32_t y_idx, int32_t x_length, int32_t y_length, bool horizontal_hitbox)
	{
		olc::vi2d tile_pos{ x_idx, y_idx };
		tile_pos *= asset_manager->tile_size;

		olc::vi2d tile_offset{ asset_manager->tile_size / 2 };

		if (horizontal_hitbox)
			tile_offset *= olc::vi2d{ x_length, -1 };
		else
			tile_offset *= olc::vi2d{ -1, y_length };

		tile_pos -= tile_offset;

		b2Body* body = physics_engine->CreateBody(b2BodyType::b2_staticBody, static_cast<float>(tile_pos.x), static_cast<float>(tile_pos.y));
		physics_engine->AddRectangleCollider(body, 
			static_cast<float>(asset_manager->tile_size.x * x_length), 
			static_cast<float>(asset_manager->tile_size.y * y_length), 
			false, Entity::ColliderFlag::Wall);
	};

	// chain length of wall tiles next to each other counted in tiles
	int32_t chain_length;

	int32_t height_idx, width_idx;

	// horizontal iteration
	for (height_idx = 0; height_idx < asset_manager->map_size.y; ++height_idx)
	{
		chain_length = 0;
		for (width_idx = 0; width_idx < asset_manager->map_size.x; ++width_idx)
		{
			int32_t index = asset_manager->map_size.x * height_idx + width_idx;

			// 0 means empty tile
			if (asset_manager->tilemap_walls[index] == 0)
			{
				if (chain_length > 0)
				{
					create_tile_hitbox(width_idx, height_idx, chain_length, 1, true);
					chain_length = 0;
				}
			}
			else
			{
				// if chain hasn't started
				if (chain_length == 0)
				{
					// if hitbox length would be > 1
					if (width_idx + 1 != asset_manager->map_size.x)
					{
						int32_t right_index = asset_manager->map_size.x * height_idx + width_idx + 1;
						if (asset_manager->tilemap_walls[right_index] != 0)
						{
							++chain_length;
							continue;
						}
					}

					// if there is a wall tile on top or on bottom from this tile, don't
					// create hitbox, because vertical iteration will create the hitbox
					bool top_or_bottom_tile_exists = false;

					if (height_idx != 0)
					{
						int32_t top_index = asset_manager->map_size.x * (height_idx - 1) + width_idx;
						if (asset_manager->tilemap_walls[top_index] != 0)
							top_or_bottom_tile_exists = true;
					}
					if (height_idx + 1 != asset_manager->map_size.y)
					{
						int32_t bottom_index = asset_manager->map_size.x * (height_idx + 1) + width_idx;
						if (asset_manager->tilemap_walls[bottom_index] != 0)
							top_or_bottom_tile_exists = true;
					}

					if (top_or_bottom_tile_exists == false)
						++chain_length;
				}
				else
				{
					++chain_length;
				}
			}
		}

		if (chain_length > 0)
		{
			create_tile_hitbox(width_idx, height_idx, chain_length, 1, true);
			chain_length = 0;
		}
	}

	// vertical iteration
	for (width_idx = 0; width_idx < asset_manager->map_size.x; ++width_idx)
	{
		chain_length = 0;
		for (height_idx = 0; height_idx < asset_manager->map_size.y; ++height_idx)
		{
			int32_t index = asset_manager->map_size.x * height_idx + width_idx;

			// 0 means empty tile
			if (asset_manager->tilemap_walls[index] == 0)
			{
				if (chain_length > 0)
				{
					create_tile_hitbox(width_idx, height_idx, 1, chain_length, false);
					chain_length = 0;
				}
			}
			else
			{
				// if chain hasn't started
				if (chain_length == 0)
				{
					// if hitbox length would be > 1
					if (height_idx + 1 != asset_manager->map_size.y)
					{
						int32_t bottom_index = asset_manager->map_size.x * (height_idx + 1) + width_idx;
						if (asset_manager->tilemap_walls[bottom_index] != 0)
							++chain_length;
					}
				}
				else
				{
					++chain_length;
				}
			}
		}

		if (chain_length > 0)
		{
			create_tile_hitbox(width_idx, height_idx, 1, chain_length, false);
			chain_length = 0;
		}
	}
}

void EntityManager::YsortEntities()
{
	auto comp_func = [](auto& left, auto& right)
	{
		return left->pos.y + left->animation->pos_offset.y < right->pos.y + right->animation->pos_offset.y;
	};

	for (size_t i = 1; i < entities.size(); ++i)
	{
		std::unique_ptr<Entity> temp;
		size_t j = i;
		for (; j >= 1; --j)
		{
			if (j == i)
			{
				if (comp_func(entities[j], entities[j - 1]))
				{
					temp = std::move(entities[j]);
					entities[j] = std::move(entities[j - 1]);
				}
				else // no insertions needs to be done
				{
					break;
				}
			}
			else if (comp_func(temp, entities[j - 1]))
			{
				entities[j] = std::move(entities[j - 1]);
			}
			else
			{
				entities[j] = std::move(temp);
				break;
			}
		}
		if (j == 0) // if insertion had to be done to the front 
		{
			entities[0] = std::move(temp);
		}
	}
}