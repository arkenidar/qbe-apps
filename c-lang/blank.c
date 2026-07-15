/* blank starter — empty window, quits on close or ESC
 * copy this file to begin a new demo; fill in the PLACEHOLDER sections
 * build: gcc blank.c -o ../out/blank-c -lSDL2 -lm  (or: make blank) */
#include <SDL2/SDL.h>
#include <math.h>

int main(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *win = SDL_CreateWindow("blank (C)",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	/* PLACEHOLDER: state that persists across frames
	 * e.g.  int x = 400, y = 300;  double t = 0.0; */

	for (;;) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
			if (ev.type == SDL_QUIT ||
			    (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE))
				goto quit;

		/* PLACEHOLDER: per-frame state update
		 * e.g.  x += vx;  t += 0.02; */

		SDL_SetRenderDrawColor(ren, 16, 16, 32, 255);
		SDL_RenderClear(ren);

		/* PLACEHOLDER: drawing
		 * e.g.  SDL_SetRenderDrawColor(ren, 255, 80, 80, 255);
		 *       SDL_RenderDrawLine(ren, 0, 0, 800, 600); */

		SDL_RenderPresent(ren);
		SDL_Delay(16); /* ~60 fps */
	}
quit:
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
