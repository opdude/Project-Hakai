//
//	LobbyMenuScreen.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "LobbyMenuScreen.h"
#include <vector>
#include "Button.h"
#include "Console.h"
#include "CVar.h"
#include "ChatBox.h"
#include "GameTime.h"
#include "KeyboardInputBox.h"
#include "MenuEntry.h"
#include "Panel.h"
#include "ScreenManager.h"
#include "GUITable.h"

#define READYBUTTON_ID 2
#define KEYBOARDINPUT_ID 3
#define TABLE_ID 4
#define CHATBOX_ID 5

LobbyMenuScreen::LobbyMenuScreen(RakNet::RakPeerInterface* netSession, bool server) : MenuScreen("Lobby")
{
	m_pNetSession = netSession;
	m_bServer = server;

	if (m_bServer)
	{
		MenuScreen::m_sTitle = "Lobby - Server";
		m_sGameData = GameData();
		m_sGameData.connectedPlayers = 1;
		m_sGameData.mapName = "Default Map";
		m_sGameData.playerNames[0] = Console::Instance()->GetCVar("PlayerName")->ToString();
		m_sGameData.playerReady[0] = false;
		m_sGameData.playerID[0] = m_pNetSession->GetMyGUID();
		m_sGameData.serverName = "Server Name";
	}

	//Attach Plugins
}
LobbyMenuScreen::~LobbyMenuScreen()
{
	//Destroy NetSession
	if (m_bServer)
	{
		for(int i=0; i < m_sGameData.connectedPlayers; i++)
			m_pNetSession->CloseConnection(m_pNetSession->GetSystemAddressFromIndex(i), true);
	}
	else
		m_pNetSession->CloseConnection(m_pNetSession->GetSystemAddressFromIndex(0), true);

	m_pNetSession->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(m_pNetSession);
}


void LobbyMenuScreen::LoadContent()
{
	vector<Column*> columns = vector<Column*>();
	columns.push_back(new Column("Player Name", 1000));
	columns.push_back(new Column("Colour", 1000));
	Column* ready = new Column("Ready", 1000);
	ready->Draw.connect(&LobbyMenuScreen::DrawReadyColumn);
	ready->SetDeletePointers(false);
	columns.push_back(ready);
	columns.push_back(new Column("Latency", 1000));
	GUITable* table = new GUITable(TABLE_ID, this, columns,0,0, m_pScreenManager->GetTableFont()->GetScaledHeight()*5);
	Vector2 tableSize = table->GetSize();
	Vector2 tablePos = Vector2((GetScreenWidth()/2) - (tableSize.x / 2), 1500);
	table->SetPosition(tablePos);

	Panel* panel = new Panel(this, 500,1500,3000,GetScreenHeight()-3000);

	Button* readyButton = new Button(this, READYBUTTON_ID, "Not Ready", 0,0);
	readyButton->rect.Move(panel->rect.x1 + ((panel->rect.x2 - panel->rect.x1)/2) - (readyButton->rect.x2 - readyButton->rect.x1)/2, 
		panel->rect.y2 - (readyButton->rect.y2 - readyButton->rect.y1) - 100);
	readyButton->SetText("Ready");
	readyButton->SetCenteredText(true);
	readyButton->Clicked.connect(&LobbyMenuScreen::ReadyButtonPressed);

	ChatBox* chatBox = new ChatBox(CHATBOX_ID, this, GetScreenWidth()-4500, GetScreenHeight()-2500, 4000, 1500);

	vector<char*> rejectedKeys = vector<char*>();
	KeyboardInputBox* keyboardInput = new KeyboardInputBox(KEYBOARDINPUT_ID, this, 255, chatBox->rect.x1, chatBox->rect.y2, 4000, false, rejectedKeys);
	keyboardInput->EnterPressed.connect(&LobbyMenuScreen::ChatBoxNewMessage);

	MenuEntry* backMenuEntry = new MenuEntry(this, "Back");
	backMenuEntry->Clicked.connect(&LobbyMenuScreen::BackMenuEntrySelected);
	backMenuEntry->SetDesiredPosition(Vector2(1000, GetScreenHeight() - 1000));

	m_pGUI->AddCtrl(panel);
	m_pGUI->AddCtrl(table);
	m_pGUI->AddCtrl(readyButton);
	m_pGUI->AddCtrl(chatBox);
	m_pGUI->AddCtrl(keyboardInput);
	m_pGUI->AddCtrl(backMenuEntry);

	MenuScreen::LoadContent();
}

void LobbyMenuScreen::Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	m_pGUI->Update(gameTime->GetTotalTime(), gameTime->GetDeltaTime(), otherScreenHasFocus);

	if (m_bServer)
	{
		//Update Server Info
		PongResponse pr = PongResponse();
		pr.connectedPlayers = m_sGameData.connectedPlayers;
		pr.mapID = 0;
		strcpy(pr.serverName, m_sGameData.serverName.c_str());
		m_pNetSession->SetOfflinePingResponse((char*)&pr, sizeof(PongResponse));
			
		ProcessServerPackets();
		ProcessClientPackets();
		SendServerPackets(gameTime);
	}
	else
	{
		ProcessClientPackets();
		SendClientPackets();
	}


	//Update Table
	GUITable* table = (GUITable*)m_pGUI->GetCtrl(TABLE_ID);
	table->ClearTable();

	for (int i=0; i < m_sGameData.connectedPlayers; i++)
	{
		vector<void*> row = vector<void*>();
		row.push_back(new string(m_sGameData.playerNames[i]));
		row.push_back(new string("Colour TODO"));
		row.push_back(reinterpret_cast<void *> (m_sGameData.playerReady[i]));

		if (i==0)
		{
			row.push_back(new string("0"));
		}
		else
		{
			char ping[10];
			sprintf(ping, "%i", m_pNetSession->GetLastPing(m_pNetSession->GetSystemAddressFromIndex(0)));
			row.push_back(new string(ping));
		}

		table->AddRow(row, NULL);
	}

	MenuScreen::Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);
}

void LobbyMenuScreen::HandleInput(InputState* inputState, GameTime* gameTime)
{
	((KeyboardInputBox*)m_pGUI->GetCtrl(KEYBOARDINPUT_ID))->HandleInput(inputState,gameTime);
}

void LobbyMenuScreen::Draw(HGE* hge, GameTime* gameTime)
{
	m_pGUI->Render();

	MenuScreen::Draw(hge, gameTime);
}

void LobbyMenuScreen::ProcessClientPackets()
{
	RakNet::Packet* packet;
	for (packet=m_pNetSession->Receive(); packet; m_pNetSession->DeallocatePacket(packet), packet=m_pNetSession->Receive())
	{
		switch(packet->data[0])
		{
		case ID_LOBBY_GAME_DATA:
			{
				Console::Instance()->Print("Received Lobby Packet", DEBUG);
				BitStream bs(packet->data+1, packet->length, false);
				bs.Read(m_sGameData.connectedPlayers);
				bs.Read(m_sGameData.mapName);
				bs.Read(m_sGameData.playerID);
				bs.Read(m_sGameData.playerColour);
				bs.Read(m_sGameData.playerNames);
				bs.Read(m_sGameData.playerReady);
				bs.Read(m_sGameData.serverName);
				break;
			}
		case ID_CLIENT_MESSAGE:
			{
#if _DEBUG
				char msg[55];
				char guid[20];
				packet->guid.ToString(guid);
				sprintf(msg, "Message Received (%s)", guid);
#endif

				BitStream bs(packet->data+1, packet->length, false);
				char text[255];
				bs.Read(text);
				((ChatBox*)m_pGUI->GetCtrl(CHATBOX_ID))->AddLine(text);

				break;
			}
		case ID_CONNECTION_LOST:
			Console::Instance()->Print("Server Shutdown.", DEBUG);
			//TODO: Add Message box
			ExitScreen();
			break;
		}
	}
}

void LobbyMenuScreen::SendClientPackets()
{

}

void LobbyMenuScreen::ProcessServerPackets()
{
	RakNet::Packet* packet;
	for (packet=m_pNetSession->Receive(); packet; m_pNetSession->DeallocatePacket(packet), packet=m_pNetSession->Receive())
	{
		switch(packet->data[0])
		{
		case ID_UNCONNECTED_PING:
			break;
		case ID_UNCONNECTED_PONG:
			RakNet::Time time;
			memcpy((char*)&time, packet->data+1, sizeof(RakNet::Time));
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			{
#if _DEBUG
				char msg[55];
				char guid[20];
				packet->guid.ToString(guid);
				sprintf(msg, "Player Connected (%s)", guid);
				Console::Instance()->Print(msg, DEBUG);
#endif
			}
			break;
		case ID_CLIENT_DATA:
			{
#if _DEBUG
				char msg[55];
				char guid[20];
				packet->guid.ToString(guid);
				sprintf(msg, "Client Data Received (%s)", guid);
#endif
				BitStream bs(packet->data+1, packet->length, false);
				char playerName[30];
				bs.Read(playerName);

				//Add Player
				int freeSlot = GetFreePlayerSlot();
				m_sGameData.playerNames[freeSlot] = string(playerName);
				m_sGameData.playerColour[freeSlot] = hgeColor(ARGB(255,255,0,0));
				m_sGameData.playerID[freeSlot] = packet->guid;
				m_sGameData.connectedPlayers++;

				//Send Message
				char text[50];
				sprintf(text,"%s Connected.", playerName);
				SendMessage(text);

				break;
			}
		case ID_DISCONNECTION_NOTIFICATION:
			{
#if _DEBUG
				char msg[55];
				char guid[20];
				packet->guid.ToString(guid);
				sprintf(msg, "Player Disconnected (%s)", guid);
				Console::Instance()->Print(msg, DEBUG);
#endif
				//Send Message
				char text[50];
				sprintf(text,"%s Disconnected.", m_sGameData.playerNames[GetPlayerID(packet->guid)]);
				SendMessage(text);

				//Remove Player
				int playerID = GetPlayerID(packet->guid);
				m_sGameData.playerNames[playerID] = "";
				m_sGameData.playerReady[playerID] = false;
				m_sGameData.connectedPlayers--;
				break;
			}
		case ID_CONNECTION_LOST:
			{
#if _DEBUG
				char msg[55];
				char guid[20];
				packet->guid.ToString(guid);
				sprintf(msg, "Player Disconnected (%s)", guid);
#endif
				Console::Instance()->Print(msg, DEBUG);
				int playerID = GetPlayerID(packet->guid);
				m_sGameData.playerNames[playerID] = "";
				m_sGameData.playerReady[playerID] = false;
				m_sGameData.connectedPlayers--;
			}
			break;
		case ID_CLIENT_READY_CHANGE:
			{
#if _DEBUG
				char msg[55];
				char guid[20];
				packet->guid.ToString(guid);
				sprintf(msg, "Client Ready Status Change (%s)", guid);
#endif
				int playerID = GetPlayerID(packet->guid);
				BitStream bs(packet->data+1, packet->length, false);
				bs.Read(m_sGameData.playerReady[playerID]);
				break;
			}

		case ID_CLIENT_MESSAGE:
			{
#if _DEBUG
				char msg[55];
				char guid[20];
				packet->guid.ToString(guid);
				sprintf(msg, "Message Received (%s)", guid);
#endif

				BitStream bs(packet->data+1, packet->length, false);
				char text[255];
				bs.Read(text);
				SendMessage(text);

				break;
			}
		}
	}

}

void LobbyMenuScreen::SendServerPackets(GameTime* gt)
{
	if (gt->GetTotalTime() - m_fLastPacketTime > 0.3)
	{
		//Send Lobby Data
		MessageID typeID = ID_LOBBY_GAME_DATA;
		BitStream bs;
		bs.Write(typeID);
		bs.Write(m_sGameData.connectedPlayers);
		bs.Write(m_sGameData.mapName);
		bs.Write(m_sGameData.playerID);
		bs.Write(m_sGameData.playerColour);
		bs.Write(m_sGameData.playerNames);
		bs.Write(m_sGameData.playerReady);
		bs.Write(m_sGameData.serverName);
		m_pNetSession->Send(&bs, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);

		m_fLastPacketTime = gt->GetTotalTime();
	}
}

//Server Only
void LobbyMenuScreen::SendMessage(char* msg)
{
	BitStream obs;
	obs.Write((MessageID)ID_CLIENT_MESSAGE);
	obs.Write(msg);
	m_pNetSession->Send(&obs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);

	//Add to Chat Box
	((ChatBox*)m_pGUI->GetCtrl(CHATBOX_ID))->AddLine(msg);	
}

void LobbyMenuScreen::ReadyButtonPressed(GameScreen* screen, hgeGUIObject* object)
{
	LobbyMenuScreen* mScr = ((LobbyMenuScreen*)screen);
	Button* b = (Button*)object;
	if (!strcmp(b->GetText(), "Ready"))
	{
		//Send Ready
		BitStream bs;
		bs.Write((MessageID)ID_CLIENT_READY_CHANGE);
		bs.Write(true);
		mScr->m_pNetSession->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, mScr->GetServerGUID(), false);
		b->SetText("Not Ready");
	}
	else
	{
		//Send Unready
		BitStream bs;
		bs.Write((MessageID)ID_CLIENT_READY_CHANGE);
		bs.Write(false);
		mScr->m_pNetSession->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, mScr->GetServerGUID(), false);
		b->SetText("Ready");
	}
}
void LobbyMenuScreen::ChatBoxNewMessage(GameScreen* screen, hgeGUIObject* object)
{
	LobbyMenuScreen* mScr = (LobbyMenuScreen*)screen;
	KeyboardInputBox* keyboard = (KeyboardInputBox*)object;

	//Can't send empty messages
	if (!strcmp(keyboard->GetText(), ""))
		return;

	//Format Message
	char msg[255];
	int id = mScr->GetPlayerID(mScr->m_pNetSession->GetMyGUID());
	sprintf(msg,"%s: %s",mScr->m_sGameData.playerNames[id].c_str(),keyboard->GetText());

	//Send Message
	BitStream bs;
	bs.Write((MessageID)ID_CLIENT_MESSAGE);
	bs.Write(msg);
	mScr->m_pNetSession->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, mScr->GetServerGUID(), false);
	keyboard->SetText("");
}

void LobbyMenuScreen::DrawReadyColumn(GameScreen* screen, Column* col, float currColumnWidth, float x, float y, GameTime* gt)
{
	int rowCount =1;
	for each(bool b in *col->GetRows())
	{
		screen->GetScreenManager()->GetFont()->SetColor(ARGB(255,255,255,255));
		if (b)
			screen->GetScreenManager()->GetFont()->Render(x+currColumnWidth, y+(screen->GetScreenManager()->GetFont()->GetScaledHeight() * rowCount), HGETEXT_LEFT, "true");
		else
			screen->GetScreenManager()->GetFont()->Render(x+currColumnWidth, y+(screen->GetScreenManager()->GetFont()->GetScaledHeight() * rowCount), HGETEXT_LEFT, "false");

		rowCount++;
	}
}

void LobbyMenuScreen::BackMenuEntrySelected(GameScreen* screen)
{
	screen->ExitScreen();
}