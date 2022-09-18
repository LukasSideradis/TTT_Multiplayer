#include "Game.h"

Game::Game() {}

Game::~Game() 
{
	// Clean up textures
	//for (SDL_Texture* texture : textures) {
	//	if (texture) {
	//		SDL_DestroyTexture(texture);
	//		texture = nullptr;
	//	}
	//}
}

void Game::Process()
{
	for (int i = 0; i < boardSize * boardSize; i++)
	{
		if (board.at(i).GetState() != descBoard.board.at(i))
		{
			board.at(i).SetState(descBoard.board.at(i));
		}
	}
	currentTurn = descBoard.currentTurn;
}

void Game::ProcessInput(bool& quit)
{
	int x, y;

	const Uint8* kb = SDL_GetKeyboardState(NULL);
	const Uint32 mouse = SDL_GetMouseState(&x, &y);

	if (inputDelay == 0)
	{
		if (kb[SDL_SCANCODE_Q]) {
			quit = true;
		}

		if ((mouse & SDL_BUTTON_LMASK) != 0)
		{
			if (x < boardSize * 48)
			{
				if (currentTurn == descPlayer.symbol)
				{
					if (board.at((x / 48) + (y / 48) * boardSize).GetState() == 0) {
						board.at((x / 48) + (y / 48) * boardSize).SetState(descPlayer.symbol);
						quit = EvaluateBoard();
						numOfTotalTurns++;
						inputDelay = 15;

						// set board description that is sent
						descBoard.board.at((x / 48) + (y / 48) * boardSize) = descPlayer.symbol;
						if (currentTurn == 1)
						{
							descBoard.currentTurn++;
						}
						else
						{
							descBoard.currentTurn--;
						}
					}
				}
			}
		}
	}
	else
	{
		inputDelay--;
	}

	if (numOfTotalTurns == boardSize * boardSize)
	{
		std::cout << "Draw!" << std::endl;
		quit = true;
	}
}

void Game::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mainRenderer);

	for (auto& square : board)
	{
		square.Draw(mainRenderer);
	}

	SDL_RenderPresent(mainRenderer);
}

void Game::InitializeBoard(int boardSize, const std::vector<SDL_Texture*>& textures) 
{
	for (int i = 0; i < boardSize * boardSize; i++) 
	{
		board.emplace_back(Square(mainRenderer, (i % boardSize) * 48, (i / boardSize) * 48, textures));
	}
}

/**
* Evaluates a single square, propagating further in the specified direction if conditions are met.
* Uses a one dimensional vector and integer arithmetics to move through it.
*
* @param num – Index of the currently evaluated square
* @param numOfConsecutiveSquares – Current amount of consecutive squares
* @param dir – Direction being evaluated, as defined in DIRECTIONS
* @param win – When true, changes the texture of winning squares
*
*/
void Game::EvaluateSquare(int num, int& numOfConsecutiveSquares, int dir, bool win) 
{
	if (board.at(num).GetState() != 0) 
	{
		if (win) 
		{
			board.at(num).SetTextureWin();
		}
		switch (dir) 
		{
		case 0: // RIGHT
			// The idea is explained for this direction only, 
			// the others are virtually the same with slight numerical differences

			// Check for out of bounds access
			if ((num + 1) > board.size() - 1) 
			{
				return;
			}
			// First line checks whether the symbol on the next square in this direction is the same as this one
			// Second line compares the distance between the current row and the row of the next square,
			// ensuring that a square at the end of a row doesn't get compared with the beginning of a new row
			// and other edge cases for the other directions
			if ((board.at(num).GetState() == board.at(num + 1).GetState()) &&
				(abs((num / boardSize) - ((num + 1) / boardSize)) == 0)) 
			{
				numOfConsecutiveSquares++;
				EvaluateSquare(num + 1, numOfConsecutiveSquares, dir, win);
			}
			break;
		case 1: // BOTTOM RIGHT
			if ((num + boardSize + 1) > board.size() - 1) 
			{
				return;
			}
			if ((board.at(num).GetState() == board.at(num + boardSize + 1).GetState())
				&& (abs((num / boardSize) - ((num + boardSize + 1) / boardSize)) == 1)) 
			{
				numOfConsecutiveSquares++;
				EvaluateSquare(num + boardSize + 1, numOfConsecutiveSquares, dir, win);
			}
			break;
		case 2: // BOTTOM
			if ((num + boardSize) > board.size() - 1) 
			{
				return;
			}
			if ((board.at(num).GetState() == board.at(num + boardSize).GetState())
				&& (abs((num / boardSize) - ((num + boardSize) / boardSize)) == 1)) 
			{
				numOfConsecutiveSquares++;
				EvaluateSquare(num + boardSize, numOfConsecutiveSquares, dir, win);
			}
			break;
		case 3: // BOTTOM LEFT
			if ((num + boardSize - 1) > board.size() - 1) 
			{
				return;
			}
			if ((board.at(num).GetState() == board.at(num + boardSize - 1).GetState())
				&& (abs((num / boardSize) - ((num + boardSize - 1) / boardSize)) == 1)) 
			{
				numOfConsecutiveSquares++;
				EvaluateSquare(num + boardSize - 1, numOfConsecutiveSquares, dir, win);
			}
			break;
		case 4: // LEFT
			if ((num - 1) < 0) 
			{
				return;
			}
			if ((board.at(num).GetState() == board.at(num - 1).GetState())
				&& (abs((num / boardSize) - ((num - 1) / boardSize)) == 0)) 
			{
				numOfConsecutiveSquares++;
				EvaluateSquare(num - 1, numOfConsecutiveSquares, dir, win);
			}
			break;
		case 5: // TOP LEFT
			if ((num - boardSize - 1) < 0) 
			{
				return;
			}
			if ((board.at(num).GetState() == board.at(num - boardSize - 1).GetState())
				&& (abs((num / boardSize) - ((num - boardSize - 1) / boardSize)) == 1)) 
			{
				numOfConsecutiveSquares++;
				EvaluateSquare(num - boardSize - 1, numOfConsecutiveSquares, dir, win);
			}
			break;
		case 6: // TOP
			if ((num - boardSize) < 0) 
			{
				return;
			}
			if ((board.at(num).GetState() == board.at(num - boardSize).GetState())
				&& (abs((num / boardSize) - ((num - boardSize) / boardSize)) == 1)) 
			{
				numOfConsecutiveSquares++;
				EvaluateSquare(num - boardSize, numOfConsecutiveSquares, dir, win);
			}
			break;
		case 7: // TOP RIGHT
			if ((num - boardSize + 1) < 0) 
			{
				return;
			}
			if ((board.at(num).GetState() == board.at(num - boardSize + 1).GetState())
				&& (abs((num / boardSize) - ((num - boardSize + 1) / boardSize)) == 1))
			{
				numOfConsecutiveSquares++;
				EvaluateSquare(num - boardSize + 1, numOfConsecutiveSquares, dir, win);
			}
			break;
		}
	}
	return;
}

bool Game::EvaluateBoard() 
{
	// There always is at least one square to start from
	int numOfConsecutive = 1;
	// Iterate through the entire board
	for (int i = 0; i < board.size(); i++) 
	{
		// Pass the condition if a square is non-empty
		if (board.at(i).GetState() != 0) 
		{
			// Iterate through all 8 directions, starting with right and going clockwise
			for (int j = 0; j < 8; j++) 
			{
				EvaluateSquare(i, numOfConsecutive, directions[j], false);
				if (numOfConsecutive >= 5) 
				{
					// If a win occurs, cycle through the winning squares one last time
					// to set the winning texture
					EvaluateSquare(i, numOfConsecutive, directions[j], true);
					if (board.at(i).GetState() == 1) 
					{
						std::cout << "Crosses win!";
					}
					else 
					{
						std::cout << "Circles win!";
					}
					return true;
				}
				// Reset if a win doesn't occur
				numOfConsecutive = 1;
			}
		}
	}
	return false;
}

bool Game::OnUserCreate(int boardSize)
{
	mainWindow = SDL_CreateWindow("Tic Tac Toe", 40, 40, boardSize * 48 + 288, boardSize * 48, SDL_WINDOW_SHOWN);
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	this->boardSize = boardSize;

	// Load textures
	SDL_Surface* tempTexture = IMG_Load("../resources/img/empty.png"); // 0
	textures.emplace_back(SDL_CreateTextureFromSurface(mainRenderer, tempTexture));
	tempTexture = IMG_Load("../resources/img/cross.png"); // 1
	textures.emplace_back(SDL_CreateTextureFromSurface(mainRenderer, tempTexture));
	tempTexture = IMG_Load("../resources/img/circle.png"); // 2
	textures.emplace_back(SDL_CreateTextureFromSurface(mainRenderer, tempTexture));
	tempTexture = IMG_Load("../resources/img/cross_win.png"); // 3
	textures.emplace_back(SDL_CreateTextureFromSurface(mainRenderer, tempTexture));
	tempTexture = IMG_Load("../resources/img/circle_win.png"); // 4
	textures.emplace_back(SDL_CreateTextureFromSurface(mainRenderer, tempTexture));
	SDL_FreeSurface(tempTexture);

	InitializeBoard(boardSize, textures);
	currentTurn = 1;
	inputDelay = 0;
	numOfTotalTurns = 0;

	descBoard.currentTurn = 1;

	descBoard.board.reserve(boardSize * boardSize);
	for (int i = 0; i < boardSize * boardSize; i++)
	{
		descBoard.board.emplace_back(0);
	}

	if (Connect("127.0.0.1", 60000))
	{
		return true;
	}

	return false;
}

bool Game::OnUserUpdate(bool& quit)
{
	if (IsConnected())
	{
		while (!Incoming().empty())
		{
			auto msg = Incoming().pop_front().msg;

			switch (msg.header.id)
			{
			case(GameMsg::Client_Accepted):
			{
				std::cout << "Server accepted client - you're in!" << std::endl;
				olc::net::message<GameMsg> msg;
				msg.header.id = GameMsg::Client_RegisteredWithServer;
				msg << descPlayer;
				Send(msg);
				break;
			}
			case(GameMsg::Client_AssignId):
			{
				msg >> nPlayerID;
				std::cout << "Assigned client ID = " << nPlayerID << std::endl;
				break;
			}
			case(GameMsg::Game_AddPlayer):
			{
				sPlayerDescription pDesc;
				msg >> pDesc;
				players.insert_or_assign(pDesc.nUniqueID, pDesc);

				if (pDesc.nUniqueID == nPlayerID)
				{
					descPlayer.symbol = pDesc.symbol;

					bWaitingForConnection = false;
				}

				std::cout << "Player " << pDesc.nUniqueID << " with symbol " << pDesc.symbol << " added." << std::endl;

				break;
			}
			case(GameMsg::Game_RemovePlayer):
			{
				uint32_t nRemovalID = 0;
				msg >> nRemovalID;
				players.erase(nRemovalID);
				break;
			}
			case(GameMsg::Game_UpdatePlayer):
			{
				sPlayerDescription pDesc;
				msg >> pDesc;
				players.insert_or_assign(pDesc.nUniqueID, pDesc);
				break;
			}
			case(GameMsg::Game_UpdateBoard):
			{
				msg >> descBoard.board;
				break;
			}
			case(GameMsg::Game_UpdateCurrentTurn):
			{
				msg >> descBoard.currentTurn;
				break;
			}

			}
		}
	}

	if (bWaitingForConnection)
	{
		SDL_SetRenderDrawColor(mainRenderer, 0, 0, 255, 255);
		SDL_RenderClear(mainRenderer);
		SDL_RenderPresent(mainRenderer);
		return true;
	}

	Process();
	ProcessInput(quit);
	Draw();

	olc::net::message<GameMsg> msg;
	msg.header.id = GameMsg::Game_UpdatePlayer;
	msg << players[nPlayerID];
	Send(msg);

	olc::net::message<GameMsg> msgBoard;
	msgBoard.header.id = GameMsg::Game_UpdateBoard;
	msgBoard << descBoard.board;
	Send(msgBoard);

	olc::net::message<GameMsg> msgCurrentTurn;
	msgCurrentTurn.header.id = GameMsg::Game_UpdateCurrentTurn;
	msgCurrentTurn << descBoard.currentTurn;
	Send(msgCurrentTurn);

	return true;
}