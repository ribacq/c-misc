#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_OGG);

	Mix_OpenAudio(22500, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Music *ree = Mix_LoadMUS("ree.ogg");
	Mix_VolumeMusic(128);

	SDL_Rect screenBounds;
	SDL_GetDisplayUsableBounds(0, &screenBounds);
	SDL_Window *win = SDL_CreateWindow("REEEEEEEEEEE!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenBounds.h * .9, screenBounds.h * .9, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0x44, 0xff, 0x44, 0xff);
	SDL_RenderClear(renderer);

	SDL_Surface *pepeSurf = IMG_Load("pepe.png");
	SDL_Texture *pepeTex = SDL_CreateTextureFromSurface(renderer, pepeSurf);
	SDL_FreeSurface(pepeSurf);
	SDL_Rect pepeRect = (SDL_Rect) { .x = 0, .y = 0, .w = pepeSurf->w, .h = pepeSurf->h };
	SDL_RenderCopy(renderer, pepeTex, NULL, &pepeRect);

	SDL_RenderPresent(renderer);
	
	/*/
	srand(time(NULL));
	int r;
	//*/
	
	int nbLines;
	int nbCols;
	SDL_GetWindowSize(win, &nbCols, &nbLines);
	nbLines /= pepeRect.h;
	nbCols /= pepeRect.w;
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
			SDL_SetRenderDrawColor(renderer, 0x44, 0xff, 0x44, 0xff);
			SDL_RenderFillRect(renderer, &pepeRect);

			SDL_SetRenderDrawColor(renderer, 0x00, 0x88, 0x22, 0xff);
			SDL_RenderDrawLine(renderer, oldPepeRect.x + oldPepeRect.w / 2, oldPepeRect.y + oldPepeRect.h / 2, pepeRect.x + pepeRect.w / 2, pepeRect.y + pepeRect.h / 2);

			pepeLine = pepeRect.y / pepeRect.h;
			pepeCol = pepeRect.x / pepeRect.w;
			visited[pepeLine][pepeCol]++;

			SDL_Keycode key = e.key.keysym.sym;
			/*/
			r = rand() % 10;
			key = r==0 ? SDLK_t : r==1 ? SDLK_s : r==2 ? SDLK_r : r==3 ? SDLK_n : key;
			//*/
			switch (key) {
			case SDLK_t:
				if (pepeCol > 0 && visited[pepeLine][pepeCol - 1] < maxVisits) {
					oldPepeRect = pepeRect;
					pepeRect.x -= pepeRect.w;
				} else {
					Mix_PlayMusic(ree, 1);
				}
				break;
			case SDLK_s:
				if (pepeLine < nbLines - 1 && visited[pepeLine + 1][pepeCol] < maxVisits) {
					oldPepeRect = pepeRect;
					pepeRect.y += pepeRect.h;
				} else {
					Mix_PlayMusic(ree, 1);
				}
				break;
			case SDLK_r:
				if (pepeLine > 0 && visited[pepeLine - 1][pepeCol] < maxVisits) {
					oldPepeRect = pepeRect;
					pepeRect.y -= pepeRect.h;
				} else {
					Mix_PlayMusic(ree, 1);
				}
				break;
			case SDLK_n:
				if (pepeCol < nbCols - 1 && visited[pepeLine][pepeCol + 1] < maxVisits) {
					oldPepeRect = pepeRect;
					pepeRect.x += pepeRect.w;
				} else {
					Mix_PlayMusic(ree, 1);
				}
				break;
			}

			SDL_RenderDrawLine(renderer, oldPepeRect.x + oldPepeRect.w / 2, oldPepeRect.y + oldPepeRect.h / 2, pepeRect.x + pepeRect.w / 2, pepeRect.y + pepeRect.h / 2);
			SDL_RenderCopy(renderer, pepeTex, NULL, &pepeRect);
			SDL_RenderPresent(renderer);
		}
	}

	Mix_FreeMusic(ree);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(pepeTex);
	SDL_DestroyWindow(win);
	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();

	return 0;
}

