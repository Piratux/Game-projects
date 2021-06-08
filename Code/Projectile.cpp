#include "Projectile.h"

// this include must be in .cpp file to avoid circular dependency
#include "EntityManager.h"

Projectile::Projectile(olc::PixelGameEngine* pge, EntityManager* entity_manager, olc::vf2d _pos, olc::vf2d dir, float _speed)
	: Entity(pge, entity_manager, _pos, Entity::Type::Projectile),
	direction(dir)
{
}
bool Projectile::Update(float delta)
{
	Entity::Update(delta);

	// temporary
	if (direction.x > 0)
		animation->SetFlip(olc::Sprite::NONE);
	if (direction.x < 0)
		animation->SetFlip(olc::Sprite::HORIZ);


	olc::vf2d dir{ direction };
	dir *= delta * speed;

	if (HasBody())
		body->SetLinearVelocity({ dir.x, dir.y });

	lifespan -= delta;
	if (lifespan < 0.0f)
		return false;

	return true;
}