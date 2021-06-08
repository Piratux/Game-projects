#include "Enemy.h"

// this include must be in .cpp file to avoid circular dependency
#include "EntityManager.h"

Enemy::Enemy(olc::PixelGameEngine* pge, EntityManager* entity_manager, olc::vf2d _pos)
	: Entity(pge, entity_manager, _pos, Entity::Type::Enemy),
	spawn_pos(_pos)
{
	// temporary
	speed = 50.0f;
}
bool Enemy::Update(float delta)
{
	Entity::Update(delta);

	switch (state)
	{
	case Enemy::State::Idle:
		wander_time -= delta;

		// if it's time to switch to walking
		if (wander_time < 0)
		{
			// if enemy wandered too far away, make it go back
			if ((pos - spawn_pos).mag() > wander_distance)
			{
				PlayAnimation(Animation::Name::Walk);
				state = Enemy::State::Retreat;
			}
			else // wander in any direction
			{
				wander_time = CalculateIdleWalkTime();

				// chooses random direction vector
				wander_direction.x = (rand()) / (RAND_MAX / (2.0f)) - 1.0f;
				wander_direction.y = (rand()) / (RAND_MAX / (2.0f)) - 1.0f;

				wander_direction = wander_direction.norm();

				// during lag spike, delta might be really high, so we don't want it to be too big
				wander_direction *= speed * std::min(delta, 0.02f) * 0.5f; // wandering speed is half as slow

				PlayAnimation(Animation::Name::Walk);
				state = Enemy::State::Wander;
			}

			// calculate sprite flip based on direction
			if (wander_direction.x > 0)
				animation->SetFlip(olc::Sprite::NONE);
			else
				animation->SetFlip(olc::Sprite::HORIZ);
		}
		break;

	case Enemy::State::Wander: 
		// TODO: change wandering to be distance based rather than time based
		body->SetLinearVelocity({ wander_direction.x, wander_direction.y });

		wander_time -= delta;

		// if it's time to switch to staying
		if (wander_time < 0)
		{
			wander_time = CalculateIdleStayTime();

			body->SetLinearVelocity({ 0.0f, 0.0f });

			PlayAnimation(Animation::Name::Idle);
			state = Enemy::State::Idle;
		}
		break;

	case Enemy::State::Guard:
		break;

	case Enemy::State::Patrol:
		break;

	case Enemy::State::Retreat:
		// we need to recalculate retreat direction in case enemy meets object in its path
		wander_direction = spawn_pos - pos;
		wander_direction = wander_direction.norm();
		wander_direction *= speed * std::min(delta, 0.02f) * 0.5f; // wandering speed is half as slow

		// calculate sprite flip based on direction
		if (wander_direction.x > 0)
			animation->SetFlip(olc::Sprite::NONE);
		else
			animation->SetFlip(olc::Sprite::HORIZ);

		body->SetLinearVelocity({ wander_direction.x, wander_direction.y });

		// when the enemy finally reached its spawn zone, make it wander again
		if ((spawn_pos - pos).mag() < 10.0f)
		{
			wander_time = CalculateIdleStayTime();

			body->SetLinearVelocity({ 0.0f, 0.0f });

			PlayAnimation(Animation::Name::Idle);
			state = Enemy::State::Idle;
		}
		break;

	case Enemy::State::Combat:
		
		break;

	case Enemy::State::Dead:
		break;

	default:
		throw std::logic_error("Enemy::Update: Unhandled enemy state\n");
	}

	return true;
}
float Enemy::CalculateIdleStayTime()
{
	return float{ (rand() % 4) + 2.0f };
}
float Enemy::CalculateIdleWalkTime()
{
	return float{ (rand() % 2) + 1.0f };
}