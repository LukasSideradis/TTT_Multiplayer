#pragma once

#define TTT_GAME_NETWORK
#include "olc_net.h"

enum class GameMsg : uint32_t
{
	Server_GetStatus,
	Server_GetPing,

	Client_Accepted,
	Client_AssignId,
	Client_RegisteredWithServer,
	Client_UnregisteredWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,

	Game_UpdateBoard,
	Game_UpdateCurrentTurn,
};

struct sPlayerDescription
{
	uint32_t nUniqueID = 0;
	int symbol = 0;
};

struct sBoardDescription
{
	//int board[64];
	std::vector<int> board;
	int currentTurn;
};