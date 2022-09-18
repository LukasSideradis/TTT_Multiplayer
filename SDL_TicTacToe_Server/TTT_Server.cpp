#include <unordered_map>

#include "TTT_Common.h"

class GameServer : public olc::net::Server_Interface<GameMsg>
{
public:
	GameServer(uint16_t nPort) : olc::net::Server_Interface<GameMsg>(nPort)
	{

	}

public:
	std::unordered_map<uint32_t, sPlayerDescription> m_Players;
	int symbol = 1;
		
protected:
	bool OnClientConnect(std::shared_ptr < olc::net::Connection<GameMsg>> client) override
	{
		// allow all for now
		return true;
	}

	void OnClientValidated(std::shared_ptr < olc::net::Connection<GameMsg>> client) override
	{
		// client passed a validation check, so send them a message
		// informing them that they can continue to communicate
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Client_Accepted;
		client->Send(msg);
	}

	void OnClientDisconnect(std::shared_ptr < olc::net::Connection<GameMsg>> client) override
	{

	}

	void OnMessage(std::shared_ptr < olc::net::Connection<GameMsg>> client, olc::net::message<GameMsg>& msg) override
	{
		switch (msg.header.id)
		{
		case GameMsg::Client_RegisteredWithServer:
		{
			sPlayerDescription desc;
			msg >> desc;
			desc.nUniqueID = client->GetID();
			desc.symbol = symbol;
			symbol++;
			m_Players.insert_or_assign(desc.nUniqueID, desc);

			olc::net::message<GameMsg> msgSendID;
			msgSendID.header.id = GameMsg::Client_AssignId;
			msgSendID << desc.nUniqueID;
			MessageClient(client, msgSendID);

			olc::net::message<GameMsg> msgAddPlayer;
			msgAddPlayer.header.id = GameMsg::Game_AddPlayer;
			msgAddPlayer << desc;
			MessageClient(client, msgAddPlayer);

			for (const auto& player : m_Players)
			{
				olc::net::message<GameMsg> msgAddOtherPlayers;
				msgAddOtherPlayers.header.id = GameMsg::Game_AddPlayer;
				msgAddOtherPlayers << player.second;
				MessageClient(client, msgAddOtherPlayers);
			}

			break;
		}
		case GameMsg::Client_UnregisteredWithServer:
		{
			break;
		}
		case GameMsg::Game_UpdatePlayer:
		{
			// simply bounce update to everyone except client
			MessageAllClients(msg, client);
			break;
		}
		case GameMsg::Game_UpdateBoard:
		{
			// simply bounce update to everyone except client
			MessageAllClients(msg, client);
			break;
		}
		case GameMsg::Game_UpdateCurrentTurn:
		{
			// simply bounce update to everyone except client
			MessageAllClients(msg, client);
			break;
		}

		}
	}
};

int main()
{
	GameServer server(60000);
	server.Start();

	while (1)
	{

		server.Update(-1, true);
	}

	return 0;
}