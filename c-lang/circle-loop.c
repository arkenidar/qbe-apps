/* checkerboard 800x600 (40px cells) + red filled circle at center
 * C twin of ssa-lang/circle-loop.ssa
 * build: gcc circle-loop.c -o ../out/circle-loop-c -lSDL2 -lm */
#include <SDL2/SDL.h>
#include <math.h>

int main(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *win = SDL_CreateWindow("qbe circle (C)",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	/* center explicitly: the CENTERED hint at creation is unreliable
	 * under some window managers (Termux:X11 notably) */
	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) == 0)
		SDL_SetWindowPosition(win, (dm.w - 800) / 2, (dm.h - 600) / 2);

	/* --- checkerboard --- */
	SDL_Rect rect = { 0, 0, 40, 40 };
	for (int y = 0; y < 600; y += 40) {
		for (int x = 0; x < 800; x += 40) {
			/* parity of (x/40 + y/40) */
			if ((x / 40 + y / 40) & 1)
				SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			else
				SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
			rect.x = x;
			rect.y = y;
			SDL_RenderFillRect(ren, &rect);
		}
	}

	/* --- red filled circle, center (400,300), radius 100, scanlines --- */
	SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
	for (int dy = -100; dy <= 100; dy++) {
		int dx = (int)sqrt((double)(10000 - dy * dy));
		SDL_RenderDrawLine(ren, 400 - dx, 300 + dy, 400 + dx, 300 + dy);
	}

	SDL_RenderPresent(ren);

	/* --- event loop: exit on window close or ESC --- */
	SDL_Event ev;
	while (SDL_WaitEvent(&ev))
		if (ev.type == SDL_QUIT ||
		    (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE))
			break;

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
