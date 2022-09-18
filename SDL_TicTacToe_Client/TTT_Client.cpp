#include <iostream>

#include "Game.h"

int main(int argc, char* argv[]) {
	int boardSize = 8;
	/*std::cout << "Enter a board size: ";
	std::cin >> boardSize;
	std::cout << std::endl;*/

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	// Initialize the board
	Game game;

	game.OnUserCreate(boardSize);
	
	bool quit = false;

	while (!quit) {
		game.OnUserUpdate(quit);

		SDL_PumpEvents();
		SDL_Delay(1000.0 / 60.0);

		if (quit) {
			SDL_Delay(3000.0);
		}
	}

	return 0;
}