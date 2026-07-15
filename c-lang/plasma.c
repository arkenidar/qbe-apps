/* old-school plasma — C twin of ssa/plasma.ssa
 * build: gcc plasma.c -o ../out/plasma-c -lSDL2 -lm */
#include <SDL2/SDL.h>
#include <math.h>

#define CELL 8

int main(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *win = SDL_CreateWindow("plasma (C)",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	double t = 0.0;

	for (;;) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
			if (ev.type == SDL_QUIT ||
			    (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE))
				goto quit;

		for (int y = 0; y < 600 / CELL; y++) {
			for (int x = 0; x < 800 / CELL; x++) {
				double v = sin(x * 0.11 + t)
					 + sin(y * 0.13 + t * 1.3)
					 + sin((x + y) * 0.07 + t * 0.7);
				int r = (int)((sin(v * 3.14159) + 1.0) * 127.5);
				int g = (int)((sin(v * 3.14159 + 2.094) + 1.0) * 127.5);
				int b = (int)((sin(v * 3.14159 + 4.188) + 1.0) * 127.5);
				SDL_Rect rc = { x * CELL, y * CELL, CELL, CELL };
				SDL_SetRenderDrawColor(ren, r, g, b, 255);
				SDL_RenderFillRect(ren, &rc);
			}
		}
		SDL_RenderPresent(ren);
		SDL_Delay(16);
		t += 0.04;
	}
quit:
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
