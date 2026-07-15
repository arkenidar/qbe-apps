/* bouncing ball — C twin of ssa/bounce.ssa
 * build: gcc bounce.c -o ../out/bounce-c -lSDL2 -lm */
#include <SDL2/SDL.h>
#include <math.h>

static void fillcircle(SDL_Renderer *ren, int cx, int cy, int r)
{
	for (int dy = -r; dy <= r; dy++) {
		int dx = (int)sqrt((double)(r * r - dy * dy));
		SDL_RenderDrawLine(ren, cx - dx, cy + dy, cx + dx, cy + dy);
	}
}

int main(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *win = SDL_CreateWindow("bounce (C)",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	int x = 400, y = 300, vx = 4, vy = 3, r = 30;

	for (;;) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
			if (ev.type == SDL_QUIT ||
			    (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE))
				goto quit;

		x += vx;
		y += vy;
		if (x - r < 0 || x + r >= 800)
			vx = -vx;
		if (y - r < 0 || y + r >= 600)
			vy = -vy;

		SDL_SetRenderDrawColor(ren, 16, 16, 32, 255);
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 255, 80, 80, 255);
		fillcircle(ren, x, y, r);
		SDL_RenderPresent(ren);
		SDL_Delay(16);
	}
quit:
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
