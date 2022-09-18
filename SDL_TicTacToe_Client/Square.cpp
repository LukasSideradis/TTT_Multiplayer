#include "Square.h"

Square::Square(SDL_Renderer* rend, int pos_x, int pos_y, const std::vector<SDL_Texture*>& textures) {
	//Loading textures
	texture_empty = textures.at(0);
	size_x = 48;
	size_y = 48;
	texture_cross = textures.at(1);
	texture_circle = textures.at(2);
	texture_cross_win = textures.at(3);
	texture_circle_win = textures.at(4);

	this->pos_x = pos_x;
	this->pos_y = pos_y;

	state = 0;
	currentTexture = texture_empty;
}

Square::~Square() {}

void Square::Draw(SDL_Renderer* rend) {
	SDL_Rect drawRect = { pos_x, pos_y, size_x, size_y };
	SDL_RenderCopy(rend, currentTexture, NULL, &drawRect);
}

void Square::SetState(int state) {
	this->state = state;
	if (state == 1) {
		currentTexture = texture_cross;
	}
	else {
		currentTexture = texture_circle;
	}
}

const int& Square::GetState(){
	return state;
}

void Square::SetTextureWin() {
	if (currentTexture == texture_cross) {
		currentTexture = texture_cross_win;
	}
	else if (currentTexture == texture_circle) {
		currentTexture = texture_circle_win;
	}
}