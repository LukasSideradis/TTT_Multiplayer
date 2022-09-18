#pragma once

#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

#include "Square.h"
#include "../SDL_TicTacToe_Server/TTT_Common.h"

class Game : olc::net::Client_Interface<GameMsg>
{
public:
	Game();
	~Game();

public:
	bool OnUserCreate(int boardSize);
	bool OnUserUpdate(bool& quit);

private:
	void Process();
	void ProcessInput(bool& quit);
	void Draw();

	void InitializeBoard(int boardSize, const std::vector<SDL_Texture*>& textures);
	void EvaluateSquare(int num, int& numOfConsecutiveSquares, int dir, bool win);
	bool EvaluateBoard();

private:
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;

	std::vector<SDL_Texture*> textures;
	std::vector<Square> board;
	short directions[8] = { 0, 1, 2, 3, 4, 5, 6, 7 }; // 0 = right, 1 = bottom right, ..., 7 = top right, clockwise order
	short boardSize;
	int currentTurn;
	int inputDelay;
	short numOfTotalTurns;

private:
	std::unordered_map<uint32_t, sPlayerDescription> players;
	uint32_t nPlayerID = 0;
	sPlayerDescription descPlayer;
	sBoardDescription descBoard;

	bool bWaitingForConnection = true;
};