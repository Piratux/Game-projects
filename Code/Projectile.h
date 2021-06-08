#pragma once

#include "Entity.h"
#include "olcPixelGameEngine.h"

class Projectile : public Entity
{
private:
	olc::vf2d direction = { 0.0f, 0.0f };
	float lifespan = 1.0f; // how many seconds projectile will stay alive until automatically destroyed
	//float damage = 20.0f;

public:
	Projectile(olc::PixelGameEngine* pge, EntityManager* entity_manager, olc::vf2d pos, olc::vf2d dir, float speed);
	bool Update(float delta) override;
};