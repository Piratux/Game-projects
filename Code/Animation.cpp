#include "Animation.h"

Animation::SingleAnimation::SingleAnimation()
{
}
Animation::SingleAnimation::SingleAnimation(const std::vector<int>& frame_indexes, int animation_speed, bool loop)
	: frame_indexes(frame_indexes),
	animation_speed(animation_speed),
	loop(loop)
{
}

Animation::Premade::Premade(const olc::vi2d& spritesheet_size, Animation::Name default_animation)
	: spritesheet_size(spritesheet_size),
	default_animation(default_animation)
{
}
void Animation::Premade::AddAnimation(Animation::Name key, const std::vector<int>& frame_indexes, int animation_speed, bool loop)
{
	animations[key] = SingleAnimation{ frame_indexes, animation_speed, loop };
}
void Animation::Premade::SetTransitionalAnimation(Animation::Name from, Animation::Name to)
{
	animations[from].play_next_enabled = true;
	animations[from].next_animation = to;
}


Animation::Animation(olc::Renderable* spritesheet, Premade* animation_sheet) 
	: spritesheet(spritesheet), // TODO: if nullptr (sprite not found) set sprite to "sprite not found.png"
	animation_sheet(animation_sheet), // TODO: if nullptr (animation not found) set sprite to "sprite not found.png"
	curr_animation_name(animation_sheet->default_animation),
	has_animation((animation_sheet->spritesheet_size.x * animation_sheet->spritesheet_size.y == 1) ? false : true),
	frame_length(1.0f / (float)animation_sheet->animations[animation_sheet->default_animation].animation_speed),
	frame_size(olc::vi2d{ spritesheet->Sprite()->width, spritesheet->Sprite()->height } / animation_sheet->spritesheet_size),
	pos_offset(frame_size / 2)
{
	UpdateSpritePosition();
}
void Animation::Update(float delta)
{
	if (has_animation)
	{
		frame_uptime += delta;
		// if it's time to switch animation frame
		if (frame_uptime >= frame_length)
		{
			frame_uptime = 0.0f;
			curr_frame++;

			// if last animation frame was played
			if (curr_frame == GetCurrAnimation().frame_indexes.size())
			{
				// if animation is looping
				if (GetCurrAnimation().loop == true)
				{
					curr_frame = 0;
				}
				else // animation doesn't loop
				{
					// need to reset the frame increase
					curr_frame--;

					// if current animation has transition animation
					if (GetCurrAnimation().play_next_enabled == true)
					{
						PlayAnimation(GetCurrAnimation().next_animation);
					}
				}
			}
			UpdateSpritePosition();
		}
	}
}
void Animation::PlayAnimation(Animation::Name name)
{
	// TODO: check if animation_sheet has this animation
	if (name != curr_animation_name)
	{
		curr_animation_name = name;
		curr_frame = 0;
		frame_uptime = 0.0f;
		frame_length = 1.0f / (float)GetCurrAnimation().animation_speed;
		UpdateSpritePosition();
	}
}
void Animation::DrawAnimation(olc::PixelGameEngine* pge, olc::vf2d pos)
{
	olc::vf2d screen_pos;
	if (flip == olc::Sprite::NONE)
		screen_pos = pos - pos_offset;
	else if (flip == olc::Sprite::HORIZ)
		screen_pos = pos + olc::vf2d{ pos_offset.x, -pos_offset.y };
	else if (flip == olc::Sprite::VERT)
		screen_pos = pos + olc::vf2d{ -pos_offset.x, pos_offset.y };


	pge->DrawPartialRotatedDecal(
		pge->NormaliseToScreenSpace(screen_pos), // render pos
		spritesheet->Decal(), // decal ptr
		0, // angle
		olc::vf2d{ 0.0f, 0.0f }, // rotation center (and also acts as -offset)
		sprite_pos, // source pos
		frame_size, // source size
		scale); // scale
}
void Animation::SetFlip(olc::Sprite::Flip flip)
{
	this->flip = flip;

	if (flip == olc::Sprite::NONE)
		scale = { 1.0f, 1.0f };
	else if (flip == olc::Sprite::HORIZ)
		scale = { -1.0f, 1.0f };
	else if (flip == olc::Sprite::VERT)
		scale = { 1.0f, -1.0f };
}

void Animation::UpdateSpritePosition()
{
	sprite_pos.x = (GetCurrAnimation().frame_indexes[curr_frame] % animation_sheet->spritesheet_size.x) * frame_size.x;
	sprite_pos.y = (GetCurrAnimation().frame_indexes[curr_frame] / animation_sheet->spritesheet_size.x) * frame_size.y;
}
Animation::SingleAnimation Animation::GetCurrAnimation()
{
	return animation_sheet->animations[curr_animation_name];
}