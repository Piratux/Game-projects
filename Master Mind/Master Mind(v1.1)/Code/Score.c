#include "Score.h"

void init_score(Score* score, char* directory)
{
	for(int i = strlen(directory)-1; i >= 0; i--)
	{
		if(directory[i] == '\\')
		{
			directory[i] = '\0';
			strcpy(score->save_dir, directory);
			strcat(score->save_dir, "\\GameData");
			break;
		}
	}
	
	// if score was never saved
	if(auto_load_score(score) == -1)
	{
		for(int i = 0; i < 3; i++)
		{
			score->total_scores[i] = 0;
		}
	}
}
void draw_score_show(Screen* screen, Texture* tex, Score* score)
{
	float shade = -0.5f;
	SDL_Point mouse_pos = get_mouse_pos(screen);
	
	draw_image(screen, &tex[PageIMG], tex[PageIMG].pos, 0.0f);
	if(mouse_in_region(screen, 8, 21, 24, 28))
	{
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){8, 24}, shade, "back");
	}
	else
	{
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){8, 24}, 0.0f, "back");
	}
	
	for(int i = 0; i < 3; i++)
	{
		if(i < score->total_scores[score->check_difficulty])
		{
			time_t mins = score->scores[score->check_difficulty][i] / 60;
			time_t seconds = score->scores[score->check_difficulty][i] % 60;
			draw_number(screen, &tex[NumbersIMG], (SDL_Point){11, 2 + i * 7}, 0.0f, mins);
			draw_number(screen, &tex[NumbersIMG], (SDL_Point){17, 2 + i * 7}, 0.0f, floor(seconds / 10));
			draw_number(screen, &tex[NumbersIMG], (SDL_Point){21, 2 + i * 7}, 0.0f, seconds % 10);
			
			screen->data[3 + i * 7][15] = screen->colours[Gray];
			screen->data[5 + i * 7][15] = screen->colours[Gray];
		}
		draw_number(screen, &tex[NumbersIMG], (SDL_Point){3, 2 + i * 7}, 0.0f, i+1);
		screen->data[6 + i * 7][7] = screen->colours[Gray];
	}
}
void draw_score_menu(Screen* screen, Texture* tex)
{
	float shade = -0.5f;
	
	draw_image(screen, &tex[PageIMG], tex[PageIMG].pos, 0.0f);
	
	char options[][10] = {"check", "save", "load"};
	
	// Options
	for(int i = 0; i < 3; i++)
	{
		float shading = 0.0f;
		if(mouse_in_region(screen, 3, 27, 2 + i * 7, 6 + i * 7))
		{
			shading = shade;
		}
		draw_word(screen, &tex[SymbolsIMG], (SDL_Point){2, 2 + i * 7}, shading, options[i]);
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
int score_show_mouse_pressed(Screen* screen, Score* score)
{
	// back button
	if(mouse_in_region(screen, 8, 21, 24, 28))
	{
		return ScoreDifficultyState;
	}
	return ScoreShowState;
}
int choose_score_difficulty_mouse_pressed(Screen* screen, Score* score)
{
	// options
	for(int i = 0; i < 3; i++)
	{
		if(mouse_in_region(screen, 3, 27, 2 + i * 7, 6 + i * 7))
		{
			score->check_difficulty = i;
			return ScoreShowState;
		}
	}
	// back button
	if(mouse_in_region(screen, 8, 21, 24, 28))
	{
		return ScoreMenuState;
	}
	return ScoreDifficultyState;
}
int score_menu_mouse_pressed(Screen* screen, Score* score)
{
	// options
	for(int i = 0; i < 3; i++)
	{
		if(mouse_in_region(screen, 3, 27, 2 + i * 7, 6 + i * 7))
		{
			switch(i)
			{
			case 0:
				return ScoreDifficultyState;
				break;
			case 1:
				save_score(score);
				break;
			case 2:
				load_score(score);
				break;
			}
		}
	}
	// back button
	if(mouse_in_region(screen, 8, 21, 24, 28))
	{
		return MenuState;
	}
	return ScoreMenuState;
}
void add_score(Score* score, int difficulty, time_t timer)
{
	int idx = difficulty;
	score->scores[idx][score->total_scores[idx]++] = timer;
	
	for(int j = score->total_scores[idx]-1; j > 0; j--)
	{
		if(score->scores[idx][j] > score->scores[idx][j-1])
		{
			break;
		}
		// swap scores
		score->scores[idx][j] ^= score->scores[idx][j-1] ^= score->scores[idx][j] ^= score->scores[idx][j-1];
	}
	
	if(score->total_scores[idx] == 4)
	{
		score->total_scores[idx]--;
	}
}
void save_score(Score* score)
{
	char dir[1024];
	getcwd(dir, sizeof(dir));
	strcat(dir, "\\mysave.bin");
	
	OPENFILENAMEA ofn;
	
	memset(&ofn, 0, sizeof(OPENFILENAMEA));
	
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.lpstrFilter = "Binary Files (*.bin);\0*.bin\0\0";
	ofn.lpstrFile = dir;
	ofn.nMaxFile = sizeof (dir) - 1;
	ofn.Flags = (OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT);
	
	int status;
	if(status = GetSaveFileNameA(&ofn))
	{
		// if file doesn't end with bin append it to file
		if(strcmp(ofn.lpstrFile + strlen(ofn.lpstrFile) - 4, ".bin") != 0)
		{
			strcat(ofn.lpstrFile, ".bin");
		}
		FILE *fp;
		fp = fopen(ofn.lpstrFile, "wb");
		fwrite(&score->total_scores, sizeof(score->total_scores), 1, fp);
		fwrite(&score->scores, sizeof(score->scores), 1, fp);
		fclose(fp);
		printf("Score saved succesfully\n");
	}
	else
	{
		if(status != 0)
		{
			printf("error: %x\n", CommDlgExtendedError());
		}
	}
}
void load_score(Score* score)
{
	char dir[1024];
	getcwd(dir, sizeof(dir));
	strcat(dir, "\\mysave.bin");
	
	OPENFILENAMEA ofn;
	memset(&ofn, 0, sizeof(OPENFILENAMEA));
	
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.lpstrFilter = "Binary Files (*.bin);\0*.bin\0\0";
	ofn.lpstrFile = dir;
	ofn.nMaxFile = sizeof (dir) - 1;
	ofn.Flags = (OFN_OVERWRITEPROMPT);
	
	int status;
	if(status = GetOpenFileNameA(&ofn))
	{
		// if file doesn't end with '.bin' append it to file
		if(strcmp(ofn.lpstrFile + strlen(ofn.lpstrFile) - 4, ".bin") != 0)
		{
			strcat(ofn.lpstrFile, ".bin");
		}
		FILE *fp;
		fp = fopen(ofn.lpstrFile, "rb");
		fread(&score->total_scores, sizeof(score->total_scores), 1, fp);
		fread(&score->scores, sizeof(score->scores), 1, fp);
		fclose(fp);
		printf("Score loaded succesfully\n");
	}
	else
	{
		if(status != 0)
		{
			printf("error: %x\n", CommDlgExtendedError());
		}
	}
}
void auto_save_score(Score* score)
{
	struct stat st = {0};
	
	char dir[1024];
	strcpy(dir, score->save_dir);
	
	if(stat(dir, &st) == -1)
	{
		mkdir(dir);
	}
	strcat(dir, "\\autosave.bin");
	FILE *fp;
	fp = fopen(dir, "wb");
	fwrite(&score->total_scores, sizeof(score->total_scores), 1, fp);
	fwrite(&score->scores, sizeof(score->scores), 1, fp);
	fclose(fp);
	printf("autosaved\n");
}
int auto_load_score(Score* score)
{
	FILE *fp;
	if(fp = fopen("GameData\\autosave.bin", "rb"))
	{
		fread(&score->total_scores, sizeof(score->total_scores), 1, fp);
		fread(&score->scores, sizeof(score->scores), 1, fp);
		fclose(fp);
		printf("autoloaded\n");
		return 1;
	}
	return -1;
}
