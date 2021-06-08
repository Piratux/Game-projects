#pragma once
#include "olcPixelGameEngine.h"
#include "Entity.h"

class Camera
{
public:
	olc::vi2d size; // should be the same as screen size
	olc::vf2d pos; // top left of the screen

	float lerp_speed = 0.2f; // entity follow delay (1 means no delay)

	Entity* follow = nullptr;

public:
	Camera(olc::vi2d size, olc::vf2d pos = { 0,0 });
	void Update();
};

