#pragma once

#include "Screen.h"
#include "Score.h"

typedef struct Game
{
	int secret_code[4];
	int evaluation[12][4];
	int guesses[12][4];
	int curr_guess[4];
	int total_guesses;
	int state;
	int difficulty;
	time_t second;
	time_t timer;
}Game;
enum GameStates
{
	GoState, WinState, LoseState
};

void init_game(Game* game);
void reset_game(Game* game);
void draw_game(Screen* screen, Texture* tex, Game* game, Uint32 delta);
void draw_choose_difficulty(Screen* screen, Texture* tex);
void draw_timer(Screen* screen, Texture* tex, Game* game);
void draw_guesses(Screen* screen, Game* game);
int game_mouse_pressed(Screen* screen, Game* game, Score* score, Uint8 mouse_button);
int choose_game_difficulty_mouse_pressed(Screen* screen, Game* game);
void generate_code(Game* game);
void change_guess(Game* game, int idx, int direction);
void add_guess(Game* game, Score* score);
void evaluate_guess(Game* game, Score* score);
