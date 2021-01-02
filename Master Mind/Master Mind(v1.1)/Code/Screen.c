#include "Screen.h"

void init_screen(Screen* screen)
{
	screen->pixel_size = 20;
	screen->fps = 60;
	
	FILE* fp;
	if(fp = fopen("GameData//Options.txt", "r"))
	{
		log_message("Initialising options\n");
		char buff[100];
		while(fgets(buff, sizeof(buff), fp))
		{
			if(strstr(buff, "PIXEL_SIZE: "))
			{
				screen->pixel_size = atoi(strchr(buff, ' '));
				sprintf(buff, "Pixel size: %i\n", screen->pixel_size);
				log_message(buff);
			}
			else if(strstr(buff, "FPS: "))
			{
				screen->fps = atoi(strchr(buff, ' '));
				sprintf(buff, "FPS: %i\n", screen->fps);
				log_message(buff);
			}
			else
			{
				log_message("Couldn't identify option: ");
				log_message(buff);
			}
		}
		log_message("Option initialisation finished\n");
	}
	else
	{
		log_message("Failed to open Options.txt\n");
	}
	fclose(fp);
	
	screen->w = 30;
	screen->h = 30;
	screen->colours[0] = (SDL_Color){255, 255, 255, 255}; //White
	screen->colours[1] = (SDL_Color){245, 255, 60 , 255}; //Yellow
	screen->colours[2] = (SDL_Color){195, 24 , 24 , 255}; //Red
	screen->colours[3] = (SDL_Color){99 , 255, 60 , 255}; //Green
	screen->colours[4] = (SDL_Color){32 , 22 , 225, 255}; //Blue
	screen->colours[5] = (SDL_Color){223, 104, 28 , 255}; //Orange
	screen->colours[6] = (SDL_Color){120, 120, 120, 255}; //Gray
	screen->colours[7] = (SDL_Color){244, 99 , 255, 255}; //Pink
	screen->colours[8] = (SDL_Color){0  , 0  , 0  , 255}; //Black
}
int clamp(int value, int min, int max)
{
	if(value < min)
		return min;
	if(value > max)
		return max;
	return value;
}
SDL_Point get_mouse_pos(Screen* screen)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return (SDL_Point){x / screen->pixel_size, y / screen->pixel_size};
}
bool mouse_in_region(Screen* screen, int from_x, int to_x, int from_y, int to_y)
{
	SDL_Point mouse_pos = get_mouse_pos(screen);
	return ((mouse_pos.x >= from_x && mouse_pos.x <= to_x) && (mouse_pos.y >= from_y && mouse_pos.y <= to_y));
}
Uint32 get_pixel(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to retrieve
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    return *(Uint32 *)p;
}
SDL_Color shade_RGB_pixel(SDL_Color col, float shade)
{
	col.r = clamp(col.r * (1 - shade), 0, 255);
	col.g = clamp(col.g * (1 - shade), 0, 255);
	col.b = clamp(col.b * (1 - shade), 0, 255);
	return col;
}
Uint32 shade_Uint32_pixel(Uint32 data, float shade)
{
	Uint8 r, g, b;
	r = data & 0xff;
	g = data >> 8 & 0xff;
	b = data >> 16 & 0xff;
	SDL_Color color = (SDL_Color){r, g, b, 0xff};
	color = shade_RGB_pixel(color, shade);
	
	return color.r | color.g << 8 | color.b << 16;
}
void draw_part_image(Screen* screen, Texture* tex, SDL_Point pos, float shade, SDL_Point from, SDL_Point size)
{
	for(int i = from.y; i < from.y + size.y; i++)
    {
    	for(int j = from.x; j < from.x + size.x; j++)
    	{
    		Uint32 data = get_pixel(tex->image, clamp(j, 0, tex->image->w - 1), clamp(i, 0, tex->image->h - 1));
    		if(data == 0) //transparent pixel
    			continue;
    		data = shade_Uint32_pixel(data, shade);
    		int idx_y = clamp(i - from.y + pos.y, 0, screen->w - 1);
    		int idx_x = clamp(j - from.x + pos.x, 0, screen->h - 1);
			SDL_GetRGB(data, tex->image->format, 
					   &screen->data[idx_y][idx_x].r, 
					   &screen->data[idx_y][idx_x].g, 
					   &screen->data[idx_y][idx_x].b);
		}
	}
}
void draw_image(Screen* screen, Texture* tex, SDL_Point pos, float shade)
{
	draw_part_image(screen, tex, pos, shade, (SDL_Point){0, 0}, (SDL_Point){tex->image->w, tex->image->h});
}
void draw_number(Screen* screen, Texture* tex, SDL_Point pos, float shade, int num)
{
	draw_part_image(screen, tex, pos, shade, (SDL_Point){num * 3, 0}, (SDL_Point){3, 5});
}
void draw_word(Screen* screen, Texture* tex, SDL_Point pos, float shade, char* word)
{
	// These values were written looking at ./Assets/Symbols/Letters.png
	int letter_offset[] = {0, 4, 8, 11, 15, 19, 23, 28, 32, 34, 38, 42, 45, 51, 56, 60, 64, 69, 73, 77, 81, 85, 89, 95, 99, 103};
	int letter_width[] = {3, 3, 2, 3, 3, 3, 4, 3, 1, 3, 3, 2, 5, 4, 3, 3, 4, 3, 3, 3, 3, 3, 5, 3, 3, 3};
	
	int pos_offset_x = 0;
	
	for(int i = 0; i < strlen(word); i++)
	{
		int letter_idx = word[i] - 'a';
		draw_part_image(screen, tex, (SDL_Point){pos.x + pos_offset_x, pos.y}, shade,
						(SDL_Point){letter_offset[letter_idx], 0}, (SDL_Point){letter_width[letter_idx], 5});
		pos_offset_x += letter_width[letter_idx] + 1;
	}
}
void update_screen(SDL_Renderer* renderer, Screen* screen)
{
	for(int i = 0; i < screen->h; i++)
	{
		for(int j = 0; j < screen->w; j++)
		{
			SDL_Color col = screen->data[i][j];
			SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
			SDL_Rect rect = {j * screen->pixel_size, i * screen->pixel_size, 
							 screen->pixel_size, screen->pixel_size};
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

