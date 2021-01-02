#pragma once

#include "Screen.h"

typedef struct Menu
{
	int options;
	int show_option;
	int menu_state;
}Menu;
enum MenuOptions
{
	PlayOption, ScoreOption, RulesOption, QuitOption
};

void init_menu(Menu* menu);
void draw_menu(Screen* screen, Texture* tex, Menu* menu);
int menu_mouse_pressed(Screen* screen, Menu* menu);
void menu_scroll(Menu* menu, int direction);
