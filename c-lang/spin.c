/* rotating triangle (left) + rotating wireframe cube (right)
 * C twin of ssa/spin.ssa
 * build: gcc spin.c -o ../out/spin-c -lSDL2 -lm */
#include <SDL2/SDL.h>
#include <math.h>

int main(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *win = SDL_CreateWindow("spin (C)",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	/* center explicitly: the CENTERED hint at creation is unreliable
	 * under some window managers (Termux:X11 notably) */
	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) == 0)
		SDL_SetWindowPosition(win, (dm.w - 800) / 2, (dm.h - 600) / 2);

	double a = 0.0;

	for (;;) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
			if (ev.type == SDL_QUIT ||
			    (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE))
				goto quit;

		SDL_SetRenderDrawColor(ren, 16, 16, 32, 255);
		SDL_RenderClear(ren);

		/* --- triangle: 3 points on a circle of radius 120 around (200,300) --- */
		SDL_SetRenderDrawColor(ren, 80, 255, 120, 255);
		int tx[3], ty[3];
		for (int k = 0; k < 3; k++) {
			double ang = a + k * 2.0943951; /* 2*pi/3 */
			tx[k] = 200 + (int)(120.0 * cos(ang));
			ty[k] = 300 + (int)(120.0 * sin(ang));
		}
		for (int k = 0; k < 3; k++)
			SDL_RenderDrawLine(ren, tx[k], ty[k], tx[(k + 1) % 3], ty[(k + 1) % 3]);

		/* --- cube: vertices (+-1,+-1,+-1), rotate Y by a then X by a*0.7,
		 * perspective-project onto right half around (600,300) --- */
		SDL_SetRenderDrawColor(ren, 120, 180, 255, 255);
		double cA = cos(a), sA = sin(a);
		double cB = cos(a * 0.7), sB = sin(a * 0.7);
		int px[8], py[8];
		for (int i = 0; i < 8; i++) {
			double x = (i & 1) ? 1.0 : -1.0;
			double y = (i & 2) ? 1.0 : -1.0;
			double z = (i & 4) ? 1.0 : -1.0;
			/* rotate around Y */
			double x1 = x * cA + z * sA;
			double z1 = -x * sA + z * cA;
			/* rotate around X */
			double y2 = y * cB - z1 * sB;
			double z2 = y * sB + z1 * cB;
			/* perspective */
			double s = 250.0 / (z2 + 3.5);
			px[i] = 600 + (int)(x1 * s);
			py[i] = 300 - (int)(y2 * s);
		}
		for (int i = 0; i < 8; i++)
			for (int bit = 1; bit <= 4; bit <<= 1)
				if (!(i & bit)) {
					int j = i | bit;
					SDL_RenderDrawLine(ren, px[i], py[i], px[j], py[j]);
				}

		SDL_RenderPresent(ren);
		SDL_Delay(16);
		a += 0.02;
	}
quit:
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
