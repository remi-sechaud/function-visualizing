/*g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include -I C:/SDL2/include && g++ *.o -o bin/debug/main -L C:/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/debug/main -mwindows*/

#include <SDL.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <windows.h>

using namespace std;

SDL_Window *window = NULL;
SDL_Surface* surface = NULL;
int s_width = 1920;
int s_height = 1080;

void drawLine(int x1, int x2, int y1, int y2);
void DrawPixel(unsigned int x, unsigned int y);
void drawAxes(int* center,int zoom);
void drawFunction(int* center,int zoom);
bool onScreen(int x, int y);
float function(float x);

int main(int argc, char* argv[])
{
	int zoom = 10;
	bool quit = false;

	int center[2] = {0,0};

	int mouse_pos[2] = {0,0};
	int last_mouse_pos[2] = {0,0};
	int add[2] = {0,0};

	bool moving = false;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Function", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, s_width, s_height, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);


	drawAxes(center,zoom);
	drawFunction(center,zoom);

	SDL_UpdateWindowSurface(window);


	SDL_Event e;

	while (!quit)
	{
		SDL_GetMouseState(&mouse_pos[0],&mouse_pos[1]);

		while (SDL_PollEvent(&e) != 0) 
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN)
			{
				if(e.button.button == SDL_BUTTON_LEFT && moving == false)
				{
					moving = true;
					last_mouse_pos[0] = mouse_pos[0];
					last_mouse_pos[1] = mouse_pos[1];
				}
			}
			else if(e.type == SDL_MOUSEWHEEL)
			{

				if(e.wheel.y > 0)
				{
					zoom++;
				}
				else if(e.wheel.y < 0)
				{
					if(zoom > 1)
					{
						zoom--;
					}
				}
				SDL_FillRect(surface, NULL, 0x00);
				drawAxes(center,zoom);
				drawFunction(center,zoom);
				SDL_UpdateWindowSurface(window);

			}
			else if(e.type == SDL_MOUSEBUTTONUP)
			{
				if(e.button.button == SDL_BUTTON_LEFT)
				{
					moving = false;
					add[0] = center[0];
					add[1] = center[1];
				}
				
			}
			else if(e.type == SDL_MOUSEBUTTONUP)
			{
				moving = false;
				add[0] = center[0];
				add[1] = center[1];
			}
			
		}

		if(moving == true)
		{

			center[0] = add[0]-last_mouse_pos[0]+mouse_pos[0];
			center[1] = add[1]-last_mouse_pos[1]+mouse_pos[1];
			
			SDL_FillRect(surface, NULL, 0x00);
			drawAxes(center,zoom);
			drawFunction(center,zoom);
			SDL_UpdateWindowSurface(window);
		}
	}


	SDL_DestroyWindow(window); 

	SDL_Quit();

	return 0;
}


void DrawPixel(unsigned int x, unsigned int y)
{
	if(onScreen(x,y))
	{
		Uint8* pixel_ptr = (Uint8*)surface->pixels + (y*s_width + x)*4;
		*(pixel_ptr) = 0xFF;
	}
	
}

void drawAxes(int* center, int zoom)
{

		for(int i = 0; i < s_height; i++)
		{
			DrawPixel(s_width/2+center[0],i);
			if((s_width/2+center[1]-i)%zoom == 0)
			{
				DrawPixel(s_width/2+center[0]+1,i);
				DrawPixel(s_width/2+center[0]-1,i);
			}
		}
	


		for(int j = 0; j < s_width; j++)
		{
			DrawPixel(j,s_height/2+center[1]);
			if((s_height/2+center[0]-j)%zoom == 0)
			{
				DrawPixel(j,s_height/2+center[1]+1);
				DrawPixel(j,s_height/2+center[1]-1);
			}
		}
	
}

void drawFunction(int* center,int zoom)
{
	float l_pixel[2] = {0,0};
	float pixel[2];

	for(int i = -(s_width/2+center[0]); i < s_width-(s_width/2+center[0]); i++)
	{

		pixel[0] = s_width/2+center[0]+i;
		pixel[1] = -function(i/(float)zoom)*zoom+s_height/2+center[1];

		DrawPixel(pixel[0],pixel[1]);

		drawLine(pixel[0],l_pixel[0],pixel[1],l_pixel[1]);

		l_pixel[0] = pixel[0];
		l_pixel[1] = pixel[1];
		
	}
}

void drawLine(int x1, int x2, int y1, int y2)
{
	float x = x2-x1;
	float y = y2-y1;

	float d = pow(x*x+y*y,0.5);

	for (float i = 1; i < d; i++)
	{
		DrawPixel(x1+x/d*i,y1+y/d*i);
	}

}

bool onScreen(int x,int y)
{
	if(-1 < x && x < s_width)
	{
		if(-1 < y && y < s_height)
		{
			return true;
		}
	}
	return false;
}

float function(float x)
{
	if(x == 1)
	{
		return 0;
	}
	else if(x > 0) 
	{
		return x/log(x);
	}
	else 
	{
		return 0;
	}
}