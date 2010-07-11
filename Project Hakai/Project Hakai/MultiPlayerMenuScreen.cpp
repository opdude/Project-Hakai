#include "MultiPlayerMenuScreen.h"
#include <RakPeerInterface.h>
#include "Console.h"
#include "CVar.h"
#include "HelperFunctions.h"
#include "MenuEntry.h"
#include "LobbyMenuScreen.h"
#include "PopupInputScreen.h"
#include "ScreenManager.h"
#include "ServerSearchScreen.h"

#define JOIN_IP_SERVER_ID 3

MultiPlayerMenuScreen::MultiPlayerMenuScreen() : MenuScreen("Multi-Player")
{

}

MultiPlayerMenuScreen::~MultiPlayerMenuScreen()
{
	MenuScreen::~MenuScreen();
}

void MultiPlayerMenuScreen::LoadContent()
{
	MenuEntry* createServerMenuEntry = new MenuEntry(this, "Create Server");
	createServerMenuEntry->Clicked.connect(&MultiPlayerMenuScreen::CreateServerMenuEntryPushed);

	MenuEntry* searchServerMenuEntry = new MenuEntry(this, "Search for Local Server");
	searchServerMenuEntry->Clicked.connect(&MultiPlayerMenuScreen::SearchLocalServerMenuEntryPushed);

	MenuEntry* joinIPServerMenuEntry = new MenuEntry(this, "Enter Server IP");
	joinIPServerMenuEntry->Clicked.connect(&MultiPlayerMenuScreen::JoinServerIPMenuEntryPushed);

	MenuEntry* backMenuEntry = new MenuEntry(this, "Back");
	backMenuEntry->Clicked.connect(&MultiPlayerMenuScreen::BackMenuEntrySelected);

	//Add Controls
	AddMenuEntry(createServerMenuEntry);
	AddMenuEntry(searchServerMenuEntry);
	AddMenuEntry(joinIPServerMenuEntry);
	AddMenuEntry(backMenuEntry);

	MenuScreen::LoadContent();
}

void MultiPlayerMenuScreen::CreateServerMenuEntryPushed(GameScreen* screen)
{
	//Create Server
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	peer->Startup(MAX_PLAYERS, &RakNet::SocketDescriptor(Console::Instance()->GetCVar("Port")->ToInt(),0), 1);
	peer->SetMaximumIncomingConnections(MAX_PLAYERS-1);

	screen->GetScreenManager()->AddScreen(new LobbyMenuScreen(peer, true));
}

void MultiPlayerMenuScreen::SearchLocalServerMenuEntryPushed(GameScreen* screen)
{
	screen->GetScreenManager()->AddScreen(new ServerSearchScreen());
}

void MultiPlayerMenuScreen::JoinServerIPMenuEntryPushed(GameScreen* screen)
{
	((MenuEntry*)((MultiPlayerMenuScreen*)screen)->m_pGUI->GetCtrl(JOIN_IP_SERVER_ID))->SetHighlighted(false);
	PopupInputScreen* pScreen = new PopupInputScreen("Enter IP");
	pScreen->AddRejectedKey("£");
	pScreen->AddRejectedKey(" ");
	pScreen->InputAccepted.connect(&MultiPlayerMenuScreen::IPEntered);
	screen->GetScreenManager()->AddScreen(pScreen);
}

void MultiPlayerMenuScreen::IPEntered(GameScreen* screen, const char* text)
{

}

void MultiPlayerMenuScreen::BackMenuEntrySelected(GameScreen* screen)
{
	screen->ExitScreen();
}

