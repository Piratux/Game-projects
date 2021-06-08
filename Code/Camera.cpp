#include "Camera.h"

Camera::Camera(olc::vi2d size, olc::vf2d pos)
	: size(size),
	pos(pos)
{
}
void Camera::Update()
{
	if (follow == nullptr)
		return;

	auto lerp = [](olc::vf2d from, olc::vf2d to, float rate) -> olc::vf2d
	{
		return from + (to - from) * rate;
	};

	// makes the camera follow entity smoothier
	pos = lerp(pos, follow->pos - size / 2, lerp_speed);

	if (pos.x < 0.0f)
		pos.x = 0.0f;
	if (pos.y < 0.0f)
		pos.y = 0.0f;
}