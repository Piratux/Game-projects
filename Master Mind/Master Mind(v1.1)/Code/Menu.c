#include "Menu.h"

void init_menu(Menu* menu)
{
	menu->options = 4;
    menu->show_option = 0;
}
void draw_menu(Screen* screen, Texture* tex, Menu* menu)
{
	float shade = -0.5f;
	
	draw_image(screen, &tex[MenuScreenIMG], tex[MenuScreenIMG].pos, 0.0f);
	
	char menu_options[4][10] = {"play", "score", "rules", "quit"};
	
	// Menu options
	for(int i = 0; i < 2; i++)
	{
		float shading = 0.0f;
		if(mouse_in_region(screen, 7, 24, 17 + i * 6, 21 + i * 6))
		{
			shading = shade;
			draw_image(screen, &tex[SelectIMG], (SDL_Point){2, 18 + i * 6}, 0.0f);
		}
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){7, 17 + i * 6}, shading, menu_options[menu->show_option + i]);
	}
	
	// Scroll buttons
    if(menu->show_option > 0)
    {
    	if(mouse_in_region(screen, 26, 28, 16, 17))
    		draw_image(screen, &tex[ScrollUpIMG], tex[ScrollUpIMG].pos, shade);
		else
			draw_image(screen, &tex[ScrollUpIMG], tex[ScrollUpIMG].pos, 0.0f);
	}
	if(menu->show_option < menu->options - 2)
    {
    	if(mouse_in_region(screen, 26, 28, 27, 28))
    		draw_image(screen, &tex[ScrollDownIMG], tex[ScrollDownIMG].pos, shade);
		else
			draw_image(screen, &tex[ScrollDownIMG], tex[ScrollDownIMG].pos, 0.0f);
	}
}
int menu_mouse_pressed(Screen* screen, Menu* menu)
{
	SDL_Point mouse_pos = get_mouse_pos(screen);
	
	// Options
	if(mouse_in_region(screen, 8, 23, 17, 21) || mouse_in_region(screen, 8, 23, 23, 27))
	{
		switch(menu->show_option + mouse_in_region(screen, 8, 23, 23, 27))
		{
		case PlayOption:
			return GameDifficultyState;
		case ScoreOption:
			return ScoreMenuState;
		case RulesOption:
			system("start game_rules.txt");
			return MenuState;
		case QuitOption:
			return QuitState;
		default: //should never happen
			return MenuState;
		}
	}
	// Scroll
	if(menu->show_option > 0)
	{
		if(mouse_in_region(screen, 26, 28, 16, 17))
		{
			menu_scroll(menu, -1);
		}
	}
	if(menu->show_option < menu->options-2)
	{
		if(mouse_in_region(screen, 26, 28, 27, 28))
		{
			menu_scroll(menu, 1);
		}
	}
	
	return MenuState;
}
void menu_scroll(Menu* menu, int direction)
{
	menu->show_option += direction;
	
	if(menu->show_option > menu->options - 2)
		menu->show_option = menu->options - 2;
		
	if(menu->show_option < 0)
		menu->show_option = 0;
}
