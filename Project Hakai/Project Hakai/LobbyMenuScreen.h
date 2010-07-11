//
//	LobbyMenuScreen.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _LOBBYMENUSCREEN_H
#define _LOBBYMENUSCREEN_H

#include <BitStream.h>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <StringCompressor.h>
#include <hgecolor.h>
#include "MenuScreen.h"

class GameTime;
class GUITable;
class Column;
struct GameData;

using namespace RakNet;

enum
{
	ID_LOBBY_GAME_DATA = ID_USER_PACKET_ENUM,
	ID_CLIENT_DATA,
	ID_CLIENT_READY_CHANGE,
	ID_CLIENT_MESSAGE
};

struct PongResponse 
{
	char serverName[30];
	int mapID;
	int connectedPlayers;
};

struct GameData
{
	string serverName;
	string mapName;
	int connectedPlayers;
	RakNet::RakNetGUID playerID[4];
	string playerNames[4];
	bool playerReady[4];
	hgeColor playerColour[4];
};

class LobbyMenuScreen : public MenuScreen
{
public:
	LobbyMenuScreen(RakNet::RakPeerInterface* netSession, bool server);
	~LobbyMenuScreen();

	virtual void LoadContent();
	virtual void Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
	virtual void Draw(HGE* hge, GameTime* gameTime);
	virtual void HandleInput(InputState* inputState, GameTime* gameTime);

private:
	//Network
	void ProcessClientPackets();
	void SendClientPackets();
	void ProcessServerPackets();
	void SendServerPackets(GameTime* gt);

	void SendMessage(char* msg);

	static void BackMenuEntrySelected(GameScreen* screen);
	static void ChatBoxNewMessage(GameScreen* screen, hgeGUIObject* object);
	static void ReadyButtonPressed(GameScreen* screen, hgeGUIObject* object);
	static void DrawReadyColumn(GameScreen* screen, Column* col, float currColumnWidth, float x, float y, GameTime* gt);

	int GetFreePlayerSlot()
	{
		for (int i=0; i < 4; i++)
			if (!strcmp(m_sGameData.playerNames[i].c_str(),""))
				return i;
	}

	int GetPlayerID(RakNetGUID guid)
	{
		for (int i=0; i < 4; i++)
			if (guid == m_sGameData.playerID[i])
				return i;
	}

	RakNetGUID GetServerGUID()
	{
		if (m_bServer)
			return m_pNetSession->GetMyGUID();
		else
			return m_pNetSession->GetGUIDFromIndex(0);
	}

	//Network
	RakNet::RakPeerInterface* m_pNetSession;
	GameData m_sGameData;
	bool m_bServer;
	float m_fLastPacketTime;
	
	//Plugins
};

#endif