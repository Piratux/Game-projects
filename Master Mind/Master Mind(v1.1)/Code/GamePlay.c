#include "GamePlay.h"

void init_game(Game* game)
{
	game->total_guesses = 0;
	game->state = GoState;
	
	for(int i = 0; i < 4; i++)
	{
		game->curr_guess[i] = 0;
	}
	
	game->timer = 0;
	game->second = 0;
}
void reset_game(Game* game)
{
	game->total_guesses = 0;
	game->state = GoState;
	
	for(int i = 0; i < 4; i++)
	{
		game->curr_guess[i] = 0;
	}
	
	game->timer = 0;
	game->second = 0;
}
void draw_game(Screen* screen, Texture* tex, Game* game, Uint32 delta)
{
	float shade = -0.5f;
	SDL_Point mouse_pos = get_mouse_pos(screen);
	
	draw_image(screen, &tex[GameEasyIMG + game->difficulty], (SDL_Point){0, 0}, 0.0f);
	draw_image(screen, &tex[AddIMG], tex[AddIMG].pos, 0.0f);
	switch(game->state)
	{
	case GoState:
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){18, 8}, 0.0f, "go");
		if(mouse_in_region(screen, 11, 13, 26, 28))
		{
			draw_image(screen, &tex[AddIMG], tex[AddIMG].pos, shade);
		}
		break;
	case WinState:
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){16, 8}, 0.0f, "win");
		for(int i = 0; i < 2 + game->difficulty; i++)
			screen->data[15][20 + i] = screen->colours[game->secret_code[i]];
		break;
	case LoseState:
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){15, 8}, 0.0f, "lost");
		for(int i = 0; i < 2 + game->difficulty; i++)
			screen->data[15][20 + i] = screen->colours[game->secret_code[i]];
		break;
	}
	draw_guesses(screen, game);
	draw_timer(screen, tex, game);
	
	if(mouse_in_region(screen, 15, 28, 24, 28))
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){15, 24}, shade, "new");
	else
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){15, 24}, 0.0f, "new");
	
	if(mouse_in_region(screen, 15, 28, 1, 5))
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){15, 1}, shade, "back");
	else
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){15, 1}, 0.0f, "back");
	
	if(game->state == GoState && game->timer < 599 && game->total_guesses > 0)
		game->second += delta;
    if(game->second >= 1000)
    {
    	game->second = 0;
    	game->timer++;
    }
}
void draw_choose_difficulty(Screen* screen, Texture* tex)
{
	float shade = -0.5f;
	
	draw_image(screen, &tex[PageIMG], tex[PageIMG].pos, 0.0f);
	
	char difficulties[][10] = {"easy", "normal", "hard"};
	
	// Difficulty options
	for(int i = 0; i < 3; i++)
	{
		float shading = 0.0f;
		if(mouse_in_region(screen, 3, 27, 2 + i * 7, 6 + i * 7))
		{
			shading = shade;
		}
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){2, 2 + i * 7}, shading, difficulties[i]);
	}
	
	if(mouse_in_region(screen, 8, 21, 24, 28))
	{
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){8, 24}, shade, "back");
	}
	else
	{
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){8, 24}, 0.0f, "back");
	}
}
void draw_timer(Screen* screen, Texture* tex, Game* game)
{
	time_t mins = game->timer / 60;
	time_t seconds = game->timer % 60;
	draw_number(screen, &tex[NumbersIMG], (SDL_Point){16, 18}, 0.0f, mins);
	draw_number(screen, &tex[NumbersIMG], (SDL_Point){22, 18}, 0.0f, floor(seconds / 10));
	draw_number(screen, &tex[NumbersIMG], (SDL_Point){26, 18}, 0.0f, seconds % 10);
	
	screen->data[19][20] = screen->colours[6];
	screen->data[21][20] = screen->colours[6];
}
void draw_guesses(Screen* screen, Game* game)
{
	for(int i = 0; i < 2 + game->difficulty; i++)
	{
		SDL_Point mouse_pos = get_mouse_pos(screen);
		
		// Draw shaded colour guess buttons if mouse is hovered
		if(game->state == GoState && (mouse_pos.x == 4 - game->difficulty + i * 2 && mouse_pos.y == 27))
		{
			screen->data[27][4 - game->difficulty + i * 2] = shade_RGB_pixel(screen->colours[game->curr_guess[i]], 0.3f);
		}	
		else
			screen->data[27][4 - game->difficulty + i * 2] = screen->colours[game->curr_guess[i]];
		
		// Draw previous guesses
		for(int j = 0; j < game->total_guesses; j++)
		{
			screen->data[24 - j * 2][4 - game->difficulty + i * 2] = screen->colours[game->guesses[j][i]];
			screen->data[24 - j * 2][12 - game->difficulty + i] = screen->colours[game->evaluation[j][i]];
		}
	}
}
int game_mouse_pressed(Screen* screen, Game* game, Score* score, Uint8 mouse_button)
{
	SDL_Point mouse_pos = get_mouse_pos(screen);
	
	// For debugging reasons only, no cheating :)
	//printf("%i %i %i %i\n", game->secret_code[0], game->secret_code[1], game->secret_code[2], game->secret_code[3]);
	
	// Back button
	if(mouse_in_region(screen, 15, 28, 1, 5))
	{
		return MenuState;
	}
	
	if(game->state == GoState)
	{
		// Colour guess buttons
		if(mouse_pos.y == 27)
		{
			int direction = (mouse_button == SDL_BUTTON_LEFT) ? 1 : -1;
			for(int i = 0; i < 2 + game->difficulty; i++)
			{
				if(mouse_pos.x == 4 - game->difficulty + i * 2)
				{
					change_guess(game, i, direction);
					break;
				}	
			}
		}
		// Add colour guess
		if(mouse_in_region(screen, 11, 13, 26, 28))
		{
			// Prevents instantly guessing code
			if(game->total_guesses == 0)
			{
				generate_code(game);
			}
			add_guess(game, score);
		}
	}
	
	// New game
	if(mouse_in_region(screen, 15, 28, 24, 28))
	{
		reset_game(game);
	}
	return GameState;
}
int choose_game_difficulty_mouse_pressed(Screen* screen, Game* game)
{
	reset_game(game);
	for(int i = 0; i < 3; i++)
	{
		if(mouse_in_region(screen, 3, 27, 2 + i * 7, 6 + i * 7))
		{
			game->difficulty = i;
			return GameState;
		}
	}
	if(mouse_in_region(screen, 8, 21, 24, 28))
	{
		return MenuState;
	}
	return GameDifficultyState;
}
void generate_code(Game* game)
{
	int total_same;
	while(1)
	{
		total_same = 0;
		for(int i = 0; i < 2 + game->difficulty; i++)
		{
			game->secret_code[i] = rand()%8;
		}
		for(int i = 0; i < 2 + game->difficulty; i++)
		{
			total_same += (game->secret_code[i] == game->curr_guess[i]);
		}
		if(total_same < 2 + game->difficulty)
		{
			break;
		}
	}
}
void change_guess(Game* game, int idx, int direction)
{
	game->curr_guess[idx] += direction;
	if(game->curr_guess[idx] > 7)
	{
		game->curr_guess[idx] = 0;
	}
	if(game->curr_guess[idx] < 0)
	{
		game->curr_guess[idx] = 7;
	}
}
void add_guess(Game* game, Score* score)
{
	for(int i = 0; i < 2 + game->difficulty; i++)
	{
		game->guesses[game->total_guesses][i] = game->curr_guess[i];
	}
	evaluate_guess(game, score);
	game->total_guesses++;
}
void evaluate_guess(Game* game, Score* score)
{
	int white = 0, red = 0;
	for(int i = 0; i < 2 + game->difficulty; i++)
	{
		if(game->guesses[game->total_guesses][i] == game->secret_code[i])
		{
			red++;
		}
		else
		{
			for(int j = 0; j < 2 + game->difficulty; j++)
			{
				if(i != j)
				{
					if(game->guesses[game->total_guesses][j] == game->secret_code[i])
					{
						white++;
						break;
					}
				}
			}
		}
	}
	int guessed = 0;
	for(int i = guessed; i < red; i++)
	{
		game->evaluation[game->total_guesses][guessed++] = Red;
	}
	for(int i = guessed; i < red + white; i++)
	{
		game->evaluation[game->total_guesses][guessed++] = White;
	}
	for(int i = red + white; i < 2 + game->difficulty; i++)
	{
		game->evaluation[game->total_guesses][i] = Black;
	}
	
	if(red == 2 + game->difficulty)
	{
		game->state = WinState;
		add_score(score, game->difficulty, game->timer);
	}
	else if(game->total_guesses == 11)
		game->state = LoseState;
}
