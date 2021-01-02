#include "EventHandler.h"

int handle_events(int state, Screen* screen, Game* game, Menu* menu, Score* score)
{
	SDL_Event event;
	int handler_state = state;
	while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            handler_state = QuitState;
        }
		if(event.type == SDL_MOUSEBUTTONDOWN && (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT))
		{
			switch(handler_state)
			{
			case MenuState:
				handler_state = menu_mouse_pressed(screen, menu);
				break;
			case GameDifficultyState:
				handler_state = choose_game_difficulty_mouse_pressed(screen, game);
				break;
			case GameState:
				handler_state = game_mouse_pressed(screen, game, score, event.button.button);
				break;
			case ScoreShowState:
				handler_state = score_show_mouse_pressed(screen, score);
				break;
			case ScoreMenuState:
				handler_state = score_menu_mouse_pressed(screen, score);
				break;
			case ScoreDifficultyState:
				handler_state = choose_score_difficulty_mouse_pressed(screen, score);
				break;
			}
		}
		if(event.type == SDL_MOUSEWHEEL)
		{
			switch(handler_state)
			{
			case MenuState:
				menu_scroll(menu, -event.wheel.y);
				break;
			}
		}
    }
    if(handler_state == QuitState)
    {
    	auto_save_score(score);
	}
    return handler_state;
}
