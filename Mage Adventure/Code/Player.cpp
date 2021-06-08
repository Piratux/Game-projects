#include "Player.h"

// this include must be in .cpp file to avoid circular dependency
#include "EntityManager.h"

Player::Player(olc::PixelGameEngine* pge, EntityManager* entity_manager, olc::vf2d _pos)
	: Entity(pge, entity_manager, _pos, Entity::Type::Player)
{
}
bool Player::Update(float delta)
{
	Entity::Update(delta);

	GetInput(delta);

	return true;
}
void Player::GetInput(float delta)
{
	// movement
	olc::vf2d dir = { 0,0 };
	if (pge->GetKey(olc::W).bHeld || pge->GetKey(olc::UP).bHeld)
		dir.y -= 1;
	if (pge->GetKey(olc::S).bHeld || pge->GetKey(olc::DOWN).bHeld)
		dir.y += 1;
	if (pge->GetKey(olc::A).bHeld || pge->GetKey(olc::LEFT).bHeld)
		dir.x -= 1;
	if (pge->GetKey(olc::D).bHeld || pge->GetKey(olc::RIGHT).bHeld)
		dir.x += 1;

	// if player moved in any direction
	if (abs(dir.x) > 0.0f || abs(dir.y) > 0.0f)
	{
		dir = dir.norm();
		dir *= delta * speed;

		// only switch animation if player wasn't busy
		if (animation->curr_animation_name == Animation::Name::Idle || animation->curr_animation_name == Animation::Name::Walk)
		{
			// calculate sprite direction based on current velocity or if entity stopped, use his last velocity as sprite direction
			if (dir.x > 0.0f)
				animation->SetFlip(olc::Sprite::Flip::NONE);
			else if (dir.x < 0.0f)
				animation->SetFlip(olc::Sprite::Flip::HORIZ);

			PlayAnimation(Animation::Name::Walk);
		}
	}
	else
	{
		// if entity was moving and now he isn't, play idle
		if (animation->curr_animation_name == Animation::Name::Idle || animation->curr_animation_name == Animation::Name::Walk)
			PlayAnimation(Animation::Name::Idle);
	}

	body->SetLinearVelocity({ dir.x, dir.y });

	// temporary
	if (pge->GetMouse(0).bPressed) // left mouse button 
	{
		PlayAnimation(Animation::Name::Attack);
		
		if (animation->flip == olc::Sprite::NONE)
			CreateProjectile({ pos.x + 10.0f, pos.y }, { +1.0f, 0.0f }, 100);
		else if (animation->flip == olc::Sprite::HORIZ)
			CreateProjectile({ pos.x - 10.0f, pos.y }, { -1.0f, 0.0f }, 100);
	}
	if (pge->GetMouse(1).bPressed) // right mouse button 
		PlayAnimation(Animation::Name::Special);
	if (pge->GetMouse(2).bPressed) // scroll button 
		std::cout << pos << '\n';
	if (pge->GetKey(olc::K5).bPressed)
		PlayAnimation(Animation::Name::Death);
}
void Player::CreateProjectile(olc::vf2d _pos, olc::vf2d _dir, float _speed)
{
	entity_manager->CreateEntity<Projectile>(_pos, _dir, _speed);
	entity_manager->SetLastCreatedEntityAnimation("icicle", Animation::Type::Icicle_8x1);

	uint16_t category = Entity::ColliderFlag::PlayerProjectile;
	uint16_t mask = Entity::ColliderFlag::Enemy | Entity::ColliderFlag::Wall;
	entity_manager->AddLastCreatedEntityCircleCollider(b2BodyType::b2_dynamicBody, 10, 0, 0, category, mask);
}