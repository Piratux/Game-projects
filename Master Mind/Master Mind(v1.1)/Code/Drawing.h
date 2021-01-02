#pragma once

#include "Menu.h"
#include "GamePlay.h"
#include "Score.h"

void draw(int state, time_t delta, SDL_Renderer* renderer, Screen* screen, Texture* tex, Game* game, Menu* menu, Score* score);
