#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	const int WIN_WIDTH = 640;
	const int WIN_HEIGHT = 480;
	SDL_Window *window = SDL_CreateWindow("REEEEEEE!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, 0);

	SDL_Surface *winSurf = SDL_GetWindowSurface(window);
	Uint32 winColor = SDL_MapRGB(winSurf->format, 0x44, 0x44, 0x44);
	SDL_FillRect(winSurf, NULL, winColor);

	SDL_Surface *pepeSurf = IMG_Load("pepe.png");
	SDL_Rect pepeRect = (SDL_Rect) { .x = 0, .y = 0, .w = pepeSurf->w, .h = pepeSurf->h };
	SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(winSurf);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x88, 0x22, 0x00);
	SDL_BlitSurface(pepeSurf, NULL, winSurf, &pepeRect);

	srand(time(NULL));
	int r;

	const int nbLines = WIN_HEIGHT / pepeRect.h;
	const int nbCols = WIN_WIDTH / pepeRect.w;
	int visited[nbLines][nbCols];
	const int maxVisits = 1;
	int i, j;
	for (i = 0; i < nbLines; i++) for (j = 0; j < nbCols; j++) visited[i][j] = 0;
	int pepeLine, pepeCol;
	SDL_Rect oldPepeRect = pepeRect;
	SDL_Event e;
	int quit = 0;
	while (!quit) {
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
			quit = 1;
		} else if (e.type == SDL_KEYDOWN) {
			SDL_FillRect(winSurf, &pepeRect, winColor);
			SDL_RenderDrawLine(renderer, oldPepeRect.x + oldPepeRect.w / 2, oldPepeRect.y + oldPepeRect.h / 2, pepeRect.x + pepeRect.w / 2, pepeRect.y + pepeRect.h / 2);

			pepeLine = pepeRect.y / pepeRect.h;
			pepeCol = pepeRect.x / pepeRect.w;
			visited[pepeLine][pepeCol]++;

			SDL_Keycode key = e.key.keysym.sym;
			r = rand() % 10;
			//key = r==0 ? SDLK_t : r==1 ? SDLK_s : r==2 ? SDLK_r : r==3 ? SDLK_n : key;
			switch (key) {
			case SDLK_t:
				if (pepeCol > 0 && visited[pepeLine][pepeCol - 1] < maxVisits) {
					oldPepeRect = pepeRect;
					pepeRect.x -= pepeRect.w;
				}
				break;
			case SDLK_s:
				if (pepeLine < nbLines - 1 && visited[pepeLine + 1][pepeCol] < maxVisits) {
					oldPepeRect = pepeRect;
					pepeRect.y += pepeRect.h;
				}
				break;
			case SDLK_r:
				if (pepeLine > 0 && visited[pepeLine - 1][pepeCol] < maxVisits) {
					oldPepeRect = pepeRect;
					pepeRect.y -= pepeRect.h;
				}
				break;
			case SDLK_n:
				if (pepeCol < nbCols - 1 && visited[pepeLine][pepeCol + 1] < maxVisits) {
					oldPepeRect = pepeRect;
					pepeRect.x += pepeRect.w;
				}
				break;
			}
			SDL_RenderDrawLine(renderer, oldPepeRect.x + oldPepeRect.w / 2, oldPepeRect.y + oldPepeRect.h / 2, pepeRect.x + pepeRect.w / 2, pepeRect.y + pepeRect.h / 2);
			SDL_RenderPresent(renderer);
			SDL_BlitSurface(pepeSurf, NULL, winSurf, &pepeRect);
		}
		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(pepeSurf);
	SDL_FreeSurface(winSurf);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}

