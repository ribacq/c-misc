/* A Hilbert Curve in C with SDL
 * Copyright © Quentin RIBAC (2017)
 * Distributed under zlib free software license
 */

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITER 4

void rotate(int rot, int *dx, int *dy) {
	*dx = *dx ^ *dy;
	*dy = *dx ^ *dy;
	*dx = *dx ^ *dy;
	if (rot) {
		//turn x --> y
		*dx = -*dx;
	} else {
		//turn y --> x
		*dy = -*dy;
	}
}

int main(int argc, char **argv) {
	int maxIter = MAX_ITER;
	if (argc > 1) {
		char *c;
		strtol(argv[1], &c, 10);
		if (*c != argv[1][0])
			maxIter = (int) strtol(argv[1], NULL, 10);
	}

	SDL_Init(SDL_INIT_VIDEO);
	char *winTitle = (char *) calloc(256, sizeof(char));
	SDL_Window *win = SDL_CreateWindow("The Hilbert Curve", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 650, 650, 0);
	SDL_Surface *surf = SDL_GetWindowSurface(win);
	SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surf);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(win);
	
	int lenRotSide = 1;
	int lenMustRot = 2;
	int *rotSide = (int *) calloc(lenRotSide, sizeof(int));
	int *mustRot = (int *) calloc(lenMustRot, sizeof(int));
	int *next;
	rotSide[0] = 0;
	mustRot[0] = 1; mustRot[1] = 1;
	for (int iter = 0; iter < maxIter; iter++) {
		//w --> !w w w !w
		next = (int *) calloc(lenRotSide * 4, sizeof(int));
		for (int i = 0; i < lenRotSide; i++) {
			next[lenRotSide * 0 + i] = next[lenRotSide * 3 + i] = !rotSide[i];
			next[lenRotSide * 1 + i] = next[lenRotSide * 2 + i] = rotSide[i];
		}
		free(rotSide);
		rotSide = next;
		lenRotSide *= 4;

		//w --> w !t t w !t !t w t !t w, with t = iter % 2
		next = (int *) calloc(lenMustRot * 4 + 6, sizeof(int));
		memcpy(next + 0 * lenMustRot + 0, mustRot, lenMustRot * sizeof(int));
		memcpy(next + 1 * lenMustRot + 2, mustRot, lenMustRot * sizeof(int));
		memcpy(next + 2 * lenMustRot + 4, mustRot, lenMustRot * sizeof(int));
		memcpy(next + 3 * lenMustRot + 6, mustRot, lenMustRot * sizeof(int));
		next[1 * lenMustRot + 0] = next[3 * lenMustRot + 5] = !(iter % 2);
		next[1 * lenMustRot + 1] = next[3 * lenMustRot + 4] = (iter % 2);
		next[2 * lenMustRot + 2] = next[2 * lenMustRot + 3] = !(iter % 2);
		free(mustRot);
		mustRot = next;
		lenMustRot = lenMustRot * 4 + 6;
		/*/
		printf("rotSide: ");
		for (int i = 0; i < lenRotSide; i++)
			printf("%d%c", rotSide[i], (i%4==3 ? ' ' : '\0'));
		printf("\n");
		printf("mustRot: ");
		for (int i = 0; i < lenMustRot; i++)
			printf("%d%c", mustRot[i], (i%4==2 ? ' ' : '\0'));
		printf("\n");
		//*/
		
		//generate points from this data
		int lenPoints = lenMustRot + 2;
		SDL_Point *points = (SDL_Point *) calloc(lenPoints + 1, sizeof(SDL_Point));
		points[0] = (SDL_Point) { .x = 1, .y = 1 };
		int dx = rotSide[0] ? 0 : 2;
		int dy = rotSide[0] ? 2 : 0;
		sprintf(winTitle, "The Hilbert Curve (%d)", iter + 1);
		SDL_SetWindowTitle(win, winTitle);
		SDL_RenderSetLogicalSize(renderer, sqrt(lenPoints) * 2 + 1, sqrt(lenPoints) * 2 + 1);
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x88);
		SDL_RenderFillRect(renderer, NULL);
		for (int i = 0; i < lenPoints - 1; i++) {
			if ((i == 0) || mustRot[i - 1])
				rotate(rotSide[i / 4] ^ ((i % 4 == 0) || (i % 4 == 3)), &dx, &dy);
			points[i + 1] = (SDL_Point) { .x = points[i].x + dx, .y = points[i].y + dy };

			//show on screen
			SDL_SetRenderDrawColor(renderer, 0xff * ((float) (i % maxIter) / maxIter), 0x66 * ((float) iter / maxIter), 0x22 * (1 - (float) ((i % (2 * maxIter)) / (2 * maxIter))), 0xcc);
			SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
		}
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(win);
		free(points);

		SDL_Event e;
		do {
			SDL_FlushEvent(SDL_KEYUP);
			SDL_WaitEvent(&e);
		} while (e.type != SDL_KEYUP);
	}

	//clean up and exit
	free(winTitle);
	free(rotSide);
	free(mustRot);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(surf);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return EXIT_SUCCESS;
}

