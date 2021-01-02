#include "EventHandler.h"
#include "Drawing.h"
#include "Logs.h"

int main(int argc, char** argv)
{
	log_program_start();
	
	Texture* textures = parse_textures();
    if(!textures)
    {
    	return 0;
	}
	
	Menu menu;
    init_menu(&menu);
    
    Game game;
    init_game(&game);
    
    Score score;
    init_score(&score, argv[0]);
    
	Screen screen;
	init_screen(&screen);
	
	srand (time(NULL));

    SDL_Init(SDL_INIT_VIDEO);
	
    SDL_Window* window = SDL_CreateWindow("Master Mind", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          30 * screen.pixel_size, 30 * screen.pixel_size, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    int state = MenuState;
    bool running = true;
    Uint32 previous_time = 0, curr_time = 0, delta;
    while (running)
    {
	    state = handle_events(state, &screen, &game, &menu, &score);
    	if(state == QuitState)
    	{
    		running = false;
		}
    	
    	curr_time = SDL_GetTicks();
    	delta = curr_time - previous_time;
    	
    	if(1000 / screen.fps <= delta)
    	{
    		previous_time = curr_time;
    		draw(state, delta, renderer, &screen, textures, &game, &menu, &score);
		}
    }
    
    free(textures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	
	log_program_end();
	
    return 0;
}

