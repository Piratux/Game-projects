#pragma once

#include "Camera.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"

#include "AssetManager.h"
#include "PhysicsEngine.h"

class EntityManager
{
public:
	olc::PixelGameEngine* pge;
	AssetManager* asset_manager;
	PhysicsEngine* physics_engine;

	std::unique_ptr<Camera> camera;
	std::vector<std::unique_ptr<Entity>> entities;

public:
	EntityManager(olc::PixelGameEngine* pge, AssetManager* asset_manager, PhysicsEngine* physics_engine);
	void Update(float delta);
	void CreatePlayer();
	void CreateEnemies();
	
	template <typename T, typename... Args>
	void CreateEntity(Args... args)
	{
		entities.push_back(std::make_unique<T>(pge, this, args...));
	}

	void SetLastCreatedEntityAnimation(const std::string& spritesheet_name, Animation::Type type);
	void AddLastCreatedEntityRectangleCollider(b2BodyType body_type, float collider_size_x, float collider_size_y, float offset_x = 0.0f, float offset_y = 0.0f, uint16_t category = 0x0001, uint16_t mask = 0xFFFF);
	void AddLastCreatedEntityCircleCollider(b2BodyType body_type, float radius, float offset_x = 0.0f, float offset_y = 0.0f, uint16_t category = 0x0001, uint16_t mask = 0xFFFF);

private:
	void DrawEverything();
	void DrawTilemap(AssetManager::MapLayer layer);
	void DrawTile(olc::vi2d tile_pos, int32_t tile_idx);

	void AddMapWallColliders();
	// uses insertion sort
	void YsortEntities();
};

