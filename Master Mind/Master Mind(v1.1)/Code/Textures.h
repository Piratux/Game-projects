#pragma once

#include "Libraries.h"
#include "Logs.h"

typedef struct Texture
{
	SDL_Surface* image;
	SDL_Point pos;
}Texture;
enum TextureID
{
	MenuScreenIMG, SelectIMG, ScrollUpIMG, ScrollDownIMG, PageIMG,
	GameEasyIMG, GameNormalIMG, GameHardIMG, AddIMG, NumbersIMG, SymbolsIMG
};

void push_texture(Texture** tex, int idx, const char* path, SDL_Point pos);
Texture* parse_textures();
