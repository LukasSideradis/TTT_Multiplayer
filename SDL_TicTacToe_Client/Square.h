#pragma once

#include <vector>

#include <SDL.h>
#include <SDL_image.h>

class Square {
public:
	Square(SDL_Renderer* rend, int pos_x, int pos_y, const std::vector<SDL_Texture*>& textures);
	~Square();
	void Draw(SDL_Renderer* rend);
	void SetState(int state);
	const int& GetState();
	void SetTextureWin();

private:
	int pos_x, pos_y;
	int size_x, size_y;
	int state; //0 = empty, 1 = cross, 2 = circle
	// Possibly just one texture being swapped around, load them elsewhere
	SDL_Texture* texture_empty;
	SDL_Texture* texture_cross;
	SDL_Texture* texture_circle;
	SDL_Texture* texture_cross_win;
	SDL_Texture* texture_circle_win;
	SDL_Texture* currentTexture;
};