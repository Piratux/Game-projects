#include "Textures.h"

void push_texture(Texture** tex, int idx, const char* path, SDL_Point pos)
{
	if(idx == 0)
		*tex = (Texture*)calloc(1, sizeof(Texture));
	else
		*tex = (Texture*)realloc(*tex, (idx+1) * sizeof(Texture));
	
	(*tex)[idx].image = IMG_Load(path);
	(*tex)[idx].pos = pos;
	
	if((*tex)[idx].image == NULL)
	{
		log_message(IMG_GetError());
		log_message("\n");
	}
}
Texture* parse_textures()
{
	Texture* tex;
	int idx = 0;
	push_texture(&tex, idx++, "Assets/Menu/Menu.png", (SDL_Point){0, 0});
	push_texture(&tex, idx++, "Assets/Menu/Select.png", (SDL_Point){3, 18});
	push_texture(&tex, idx++, "Assets/Menu/ScrollUp.png", (SDL_Point){26, 16});
	push_texture(&tex, idx++, "Assets/Menu/ScrollDown.png", (SDL_Point){26, 27});
	push_texture(&tex, idx++, "Assets/Menu/Page.png", (SDL_Point){0, 0});
	push_texture(&tex, idx++, "Assets/Game/GameEasy.png", (SDL_Point){0, 0});
	push_texture(&tex, idx++, "Assets/Game/GameNormal.png", (SDL_Point){0, 0});
	push_texture(&tex, idx++, "Assets/Game/GameHard.png", (SDL_Point){0, 0});
	push_texture(&tex, idx++, "Assets/Game/Add.png", (SDL_Point){11, 26});
	push_texture(&tex, idx++, "Assets/Symbols/Numbers.png", (SDL_Point){0, 0});
	push_texture(&tex, idx++, "Assets/Symbols/Letters.png", (SDL_Point){0, 0});
	
	for(int i = 0; i < idx; i++)
	{
		if(!tex[i].image)
		{
			log_message("Failed to load assets\n");
			log_program_end();
			return NULL;
		}
	}
	
	return tex;
}
