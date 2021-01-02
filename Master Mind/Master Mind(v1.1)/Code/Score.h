#pragma once

#include "Screen.h"
#include "Logs.h"

typedef struct Score
{
	time_t scores[3][4];
	int total_scores[3]; // 3 different difficulties
	int check_difficulty;
	char save_dir[100];
}Score;

void init_score(Score* score, char* directory);
void draw_score_show(Screen* screen, Texture* tex, Score* score);
void draw_score_menu(Screen* screen, Texture* tex);
int score_show_mouse_pressed(Screen* screen, Score* score);
int choose_score_difficulty_mouse_pressed(Screen* screen, Score* score);
int score_menu_mouse_pressed(Screen* screen, Score* score);
void add_score(Score* score, int difficulty, time_t timer);
void save_score(Score* score);
void load_score(Score* score);
void auto_save_score(Score* score);
int auto_load_score(Score* score);
