#pragma once

#include "Textures.h"

typedef struct Screen
{
	SDL_Color data[30][30];
	int w;
	int h;
	SDL_Color colours[10];
	int pixel_size;
	int fps;
}Screen;
enum States
{
	MenuState, GameDifficultyState, GameState, ScoreShowState, ScoreMenuState, ScoreDifficultyState, QuitState
};
enum Colours
{
	White, Yellow, Red, Green, Blue, Orange, Gray, Pink, Black
};

void init_screen(Screen* screen);
int clamp(int value, int min, int max);
SDL_Point get_mouse_pos(Screen* screen);
bool mouse_in_region(Screen* screen, int from_x, int to_x, int from_y, int to_y);
Uint32 get_pixel(SDL_Surface* surface, int x, int y);
SDL_Color shade_RGB_pixel(SDL_Color col, float shade);
Uint32 shade_Uint32_pixel(Uint32 data, float shade);
void draw_part_image(Screen* screen, Texture* tex, SDL_Point pos, float shade, SDL_Point from, SDL_Point size);
void draw_image(Screen* screen, Texture* tex, SDL_Point pos, float shade);
void draw_number(Screen* screen, Texture* tex, SDL_Point pos, float shade, int num);
void draw_word(Screen* screen, Texture* tex, SDL_Point pos, float shade, char* word);
void update_screen(SDL_Renderer* renderer, Screen* screen);
