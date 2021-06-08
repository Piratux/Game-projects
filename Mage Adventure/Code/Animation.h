#pragma once
#include "olcPixelGameEngine.h"

#include <map>

class Animation
{
public:
	// animations in spritesheet
	enum class Name
	{
		Idle, Walk, Attack, Special, Death
	};

	// different animation setups of spritesheets to allow reusing animation collection
	enum class Type
	{
		Hero_10x5, Knight_5x12, Icicle_8x1
	};

	struct SingleAnimation
	{
		std::vector<int> frame_indexes;
		int animation_speed = 8; // animation frames per second
		Animation::Name next_animation = Animation::Name::Idle; // if loop == false, tells which animation to play next
		bool loop = true; // if true, animation repeats infinitely
		bool play_next_enabled = false; // if true and loop == false, after animation played once, transitions to next animation

	public:
		// default constructor needed for map in Premade struct
		SingleAnimation();
		SingleAnimation(const std::vector<int>& frame_indexes, int animation_speed = 8, bool loop = true);
	};

	struct Premade
	{
		std::map<Animation::Name, SingleAnimation> animations; // animation frame indexes
		olc::vi2d spritesheet_size; // total frames in x and y direction
		Animation::Name default_animation; // animation that plays when object is created

	public:
		Premade(const olc::vi2d& spritesheet_size, Animation::Name default_animation);
		void AddAnimation(Animation::Name key, const std::vector<int>& frame_indexes, int animation_speed = 8, bool loop = true);
		// only has effect if loop == false of animation you want to set transition
		void SetTransitionalAnimation(Animation::Name from, Animation::Name to);
	};

public:
	olc::Renderable* spritesheet;
	Premade* animation_sheet;
	
	olc::vi2d frame_size; // frame size in spritesheet
	olc::vf2d pos_offset; // stores sprite center position
	olc::vi2d sprite_pos; // in spritesheet
	olc::vf2d scale = { 1.0f, 1.0f };
	
	int curr_frame = 0;
	float frame_length; // how long frame has to play to switch to next one
	float frame_uptime = 0.0f; // how long frame has played

	Animation::Name curr_animation_name;
	olc::Sprite::Flip flip = olc::Sprite::Flip::NONE;
	bool has_animation;
	
public:
	Animation(olc::Renderable* spritesheet, Premade* animation_sheet);
	void Update(float delta);
	void PlayAnimation(Animation::Name name);
	void DrawAnimation(olc::PixelGameEngine* pge, olc::vf2d pos);

	void SetFlip(olc::Sprite::Flip flip);

private:
	// calculates sprite position in spritesheet
	void UpdateSpritePosition();
	// returns animation struct that's currently playing
	Animation::SingleAnimation GetCurrAnimation();
};

