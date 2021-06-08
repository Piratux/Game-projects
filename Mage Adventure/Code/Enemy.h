#pragma once

#include "Entity.h"
#include "olcPixelGameEngine.h"

class Enemy : public Entity
{
public:
	enum class State
	{
		Idle, // out of combat
		Wander, // walk around
		Guard, // stay in place all the time
		Patrol, // walk in a specific path
		Retreat, // returning to spawn position
		Combat, // in combat
		Dead // state that doesn't require update
	};

private:
	olc::vf2d spawn_pos;
	Entity* target = nullptr;
	Enemy::State state = Enemy::State::Idle;

	// only relevant when wander_type = IdleType::Wander
	float wander_distance = 100.0f; // how far from spawn position enemy can wander
	float wander_time = 0.0f; // tracks when enemy has to switch IdleTypeState state
	float wander_walking_time = 0.0f; // how long enemy wanders in some direction
	olc::vf2d wander_direction{ 0.0f, 0.0f }; // direction enemy currently wanders to
	
	// only relevant when curr_state = State::Combat
	float chase_distance = 300.0f; // how far enemy will chase it's target until it retreats
	float combat_distance = 50.0f; // how close enemy needs to be to attack
	olc::vf2d chase_start_pos; // position where enemy was when it first saw target and started chasing

public:
	Enemy(olc::PixelGameEngine* pge, EntityManager* entity_manager, olc::vf2d pos);
	bool Update(float delta) override;

private:
	float CalculateIdleStayTime();
	float CalculateIdleWalkTime();
};