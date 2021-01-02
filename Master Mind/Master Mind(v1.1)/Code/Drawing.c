#include "Drawing.h"

void draw(int state, time_t delta, SDL_Renderer* renderer, Screen* screen, Texture* tex, Game* game, Menu* menu, Score* score)
{
	switch(state)
	{
	case MenuState:
		draw_menu(screen, tex, menu);
		break;
	case GameDifficultyState:
		draw_choose_difficulty(screen, tex);
		break;
	case GameState:
		draw_game(screen, tex, game, delta);
		break;
	case ScoreShowState:
		draw_score_show(screen, tex, score);
		break;
	case ScoreMenuState:
		draw_score_menu(screen, tex);
		break;
	case ScoreDifficultyState:
		draw_choose_difficulty(screen, tex);
		break;
	default: //should never happen
		draw_menu(screen, tex, menu);
	}
	
	SDL_RenderClear(renderer);
    update_screen(renderer, screen);
	SDL_RenderPresent(renderer);
}
