#include "ServerSearchScreen.h"
#include <MessageIdentifiers.h>
#include <RakNetTime.h>
#include "Console.h"
#include "CVar.h"
#include "GUITable.h"
#include "HelperFunctions.h"
#include "LobbyMenuScreen.h"
#include "MenuEntry.h"
#include "ScreenManager.h"


#define TABLE_ID 2


ServerSearchScreen::ServerSearchScreen() : MenuScreen("Server Search")
{
	m_pNetSession = RakNet::RakPeerInterface::GetInstance();
	m_pNetSession->Startup(1, &RakNet::SocketDescriptor(), 1);
	m_bConnectedToGame = false;
}

ServerSearchScreen::~ServerSearchScreen()
{
	if (!m_bConnectedToGame)
		RakNet::RakPeerInterface::DestroyInstance(m_pNetSession);
}

void ServerSearchScreen::LoadContent()
{
	vector<Column*> columns = vector<Column*>();
	columns.push_back(new Column("Server Name", 2000));
	columns.push_back(new Column("Map", 2000));
	columns.push_back(new Column("Players", 1000));
	columns.push_back(new Column("Latency", 1000));
	GUITable* table = new GUITable(TABLE_ID, this, columns,0,0, 7000);
	table->EntryClicked.connect(&ServerSearchScreen::GameClicked);
	Vector2 tableSize = table->GetSize();
	Vector2 tablePos = Vector2((GetScreenWidth()/2) - (tableSize.x / 2), 1500);
	table->SetPosition(tablePos);

	MenuEntry* backMenuEntry = new MenuEntry(this, "Back");
	backMenuEntry->Clicked.connect(&ServerSearchScreen::BackMenuEntrySelected);
	backMenuEntry->SetDesiredPosition(Vector2(1000, GetScreenHeight() - 1000));

	m_pGUI->AddCtrl(table);
	m_pGUI->AddCtrl(backMenuEntry);

	//Search for local games
	SearchForGames();

	MenuScreen::LoadContent();
}

void ServerSearchScreen::UnloadContent()
{
	MenuScreen::UnloadContent();
}

void ServerSearchScreen::Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	RakNet::Packet* packet;
	for (packet=m_pNetSession->Receive(); packet; m_pNetSession->DeallocatePacket(packet), packet=m_pNetSession->Receive())
	{
		switch(packet->data[0])
		{
		case ID_UNCONNECTED_PONG:
			{
				size_t timeSize = sizeof(RakNet::TimeMS);
				size_t pongSize = sizeof(PongResponse);
				if (packet->length != timeSize+pongSize+1)
					continue;
				
				RakNet::Time time;
				memcpy((char*)&time, packet->data+1, timeSize);

				PongResponse pr;
				memcpy((char*)&pr, packet->data+1+timeSize,pongSize);

				//String Data
				char players[5];
				char ping[10];
				sprintf(players, "%i/%i", pr.connectedPlayers, MAX_PLAYERS);
				sprintf(ping, "%i", m_pNetSession->GetLastPing(packet->systemAddress));
				char* tmp = players;

				//Add Table Line
				vector<void*> row = vector<void*>();
				row.push_back(new string(pr.serverName));
				row.push_back(new string("MAP ENUM TODO"));
				row.push_back(new string(players));
				row.push_back(new string(ping));

				((GUITable*)m_pGUI->GetCtrl(TABLE_ID))->AddRow(row, &packet->systemAddress);

				break;
			}

		case ID_CONNECTION_REQUEST_ACCEPTED:
			//Send Player Data
			BitStream bs;
			bs.Write((MessageID)ID_CLIENT_DATA);
			bs.Write(Console::Instance()->GetCVar("PlayerName")->ToString().c_str());		
			m_pNetSession->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false);

			m_pScreenManager->AddScreen(new LobbyMenuScreen(m_pNetSession, false));
			m_bConnectedToGame = true;
			ExitScreen();
			break;
		}
	}

	MenuScreen::Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);
}

void ServerSearchScreen::Draw(HGE *hge, GameTime *gameTime)
{
	MenuScreen::Draw(hge, gameTime);
}

void ServerSearchScreen::SearchForGames()
{
	//Search Local Games

	m_pNetSession->Ping("255.255.255.255", Console::Instance()->GetCVar("Port")->ToInt(), true);
}

void ServerSearchScreen::GameClicked(GameScreen* screen, int rowCount, void* object)
{
	if (object != NULL)
	{
		RakNet::SystemAddress* address = ((RakNet::SystemAddress*)(object));
		const char* ip = address->ToString(false);
		((ServerSearchScreen*)screen)->m_pNetSession->Connect(ip, Console::Instance()->GetCVar("Port")->ToInt(),0,0);
	}
}

void ServerSearchScreen::BackMenuEntrySelected(GameScreen *screen)
{
	screen->ExitScreen();
}

