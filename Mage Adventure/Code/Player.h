#pragma once

#include "olcPixelGameEngine.h"
#include "Entity.h"
#include "Projectile.h"
#include "AssetManager.h"

class Player : public Entity
{
public:
	Player(olc::PixelGameEngine* pge, EntityManager* entity_manager, olc::vf2d pos);
	bool Update(float delta) override;

private:
	void GetInput(float delta);
	void CreateProjectile(olc::vf2d pos, olc::vf2d dir, float speed);
};