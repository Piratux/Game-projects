#pragma once
#include "olcPixelGameEngine.h"
#include "Animation.h"
#include "AssetManager.h"
#include "PhysicsEngine.h"

class EntityManager;

class Entity
{
public:
	enum class Type
	{
		Player, Enemy, Npc, Projectile
	};
	enum ColliderFlag : uint16_t
	{
		Wall = 0x0001,
		Player = 0x0002,
		Enemy = 0x0004,
		PlayerProjectile = 0x0008,
		EnemyProjectile = 0x0010
	};

public:
	olc::PixelGameEngine* pge;
	EntityManager* entity_manager;
	std::unique_ptr<Animation> animation;
	Entity::Type entity_type;

	olc::vf2d pos = { 0,0 }; // middle of the sprite
	float speed = 100.0f;

	b2Body* body = nullptr;

	// temporary
	olc::vf2d collider_offset;
	olc::vf2d collider_size;

	//olc::vf2d hitbox_offset;
	//olc::vf2d hitbox_size;
	
public:
	Entity(olc::PixelGameEngine* pge, EntityManager* entity_manager, olc::vf2d pos, Entity::Type entity_type);
	virtual ~Entity();

	// if returns false, entity "wants" to be deleted
	virtual bool Update(float delta);

	void SetAnimation(olc::Renderable* spritesheet, Animation::Premade* animation);
	void PlayAnimation(Animation::Name name);

	void DrawEntity(olc::vf2d camera_pos);
	
	// collider is in the middle of the entity by default
	void AddRectangleCollider(PhysicsEngine* physics_engine, b2BodyType body_type, float collider_size_x, float collider_size_y, float offset_x = 0.0f, float offset_y = 0.0f, uint16_t category = 0x0001, uint16_t mask = 0xFFFF);
	void AddCircleCollider(PhysicsEngine* physics_engine, b2BodyType body_type, float radius, float offset_x = 0.0f, float offset_y = 0.0f, uint16_t category = 0x0001, uint16_t mask = 0xFFFF);
	//void SetRectangleHitbox(PhysicsEngine* physics_engine, float collider_size_x, float collider_size_y, float offset_x = 0.0f, float offset_y = 0.0f, uint16_t category = 0x0001, uint16_t mask = 0xFFFF);
	//void SetCircleHitbox(PhysicsEngine* physics_engine, float radius, float offset_x = 0.0f, float offset_y = 0.0f, uint16_t category = 0x0001, uint16_t mask = 0xFFFF);

	bool HasBody();
	bool HasAnimation();
};

