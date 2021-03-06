//
//	Console.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "Console.h"
#include "CommandParser.h"
#include "CVar.h"
#include "DefaultCommands.h"
#include "DefaultCVars.h"
#include "GameManager.h"
#include "GameTime.h"
#include "HelperFunctions.h"
#include "KeyboardState.h"
#include "MouseState.h"
#include "ScreenManager.h"

Console* Console::m_pInstance;

Console* Console::Instance()
{
	if (m_pInstance == NULL)
		m_pInstance = new Console();

	return m_pInstance;
}

Console::Console()
{
    m_lConsoleText = vector<string>();
    m_lHistoryText = vector<string>();
    m_lLineColour = vector<hgeColor>();
    m_dCommands = map<string, Command*>();
    m_dCVars = map<string, CVar*>();
	m_bJustOpened = false;
	m_eUsageLevel = NORMAL;
	m_fKeyStrokeSpeed = 0.250f;
	m_bOutputToVS = true;
	m_bEnabled = false;

    //Setup Default Text Colours
    m_aTextColours[0] = hgeColor(ARGB(255,255, 0, 0));
    m_aTextColours[1] = hgeColor(ARGB(255,255, 255, 0));
    m_aTextColours[2] = hgeColor(ARGB(255,128, 128, 128));
    m_aTextColours[3] = hgeColor(ARGB(255,255, 255, 255));

    m_lFileBuffer = vector<string>();

	for (int i=0; i<3; i++)
		m_rConsoleTextureSource[i] = NULL;
}

Console::~Console()
{
	//Release Log
	if (m_LogWriter)
		m_LogWriter.close();

	for (int i=0; i<3; i++)
	{
		if (m_rConsoleTextureSource[i] != NULL)
			delete m_rConsoleTextureSource[i];
	}

	for (int i=0; i<3; i++)
	{
		if (m_rTextboxTextureSource[i] != NULL)
			delete m_rTextboxTextureSource[i];
	}

	//Delete Commands & CVars
	for each(pair<string, Command*> p in m_dCommands)
		delete p.second;
	for each(pair<string, CVar*> p in m_dCVars)
		delete p.second;

	delete consoleBackground;
	delete m_pLastKeyboardState;
	delete m_pKeyboardState;
	delete m_pLastMouseState;
	delete m_pMouseState;
}

/// <summary>
/// Initialise the console
/// </summary>
void Console::Initialise()
{
    m_sLog = "Debug.log";
    m_sConfig = "CVars.conf";
    m_sConfigFolder = "OpiConsole";
    m_sCommandText = "";
    m_iHistoryIndex = 0;
    m_iCursorPos = 0;
    m_iCurrentLine = 0;
    m_iConsoleLines = 10;
    m_iMaxLines = 250;
    m_bShowBackground = true;
    m_bShowCursor = true;
	m_bInitialised = true;

    //Load Resources
	m_fFont = GameManager::Instance()->GetScreenManager()->GetMonoFont();
    m_tBlankTexture = GameManager::Instance()->GetResourceManager()->GetTexture("Blank");
    m_tTextbox = GameManager::Instance()->GetResourceManager()->GetTexture("ConsoleTextBox");
    m_tConsole = GameManager::Instance()->GetResourceManager()->GetTexture("ConsoleBox");

    //Font Config
	m_fFont->SetSpacing(0);
    m_fFontHeight = m_fFont->GetScaledHeight();
	float tmpYPos = m_iConsoleLines * m_fFontHeight;

    m_vConsoleSize = Vector2(GetScreenWidth(), (float)(m_fFontHeight * m_iConsoleLines));
    consoleBackground = new hgeSprite(m_tBlankTexture, 0, 0, GetScreenWidth(), tmpYPos);
    consoleBackground->SetColor(ARGB(100, 0, 0, 0));

    //Size Textbox Source & Destinations (Idea From: xWinForms)
    m_rTextboxTextureSource[0] = new hgeSprite(m_tTextbox, 0, 0, GetTextureSize(m_tTextbox).x - 1, GetTextureSize(m_tTextbox).y);
    m_rTextboxTextureSource[1] = new hgeSprite(m_tTextbox, GetTextureSize(m_tTextbox).x - 1, 0, 0, GetTextureSize(m_tTextbox).y);
    m_rTextboxTextureSource[2] = new hgeSprite(m_tTextbox, GetTextureSize(m_tTextbox).x - 1, 0, -(GetTextureSize(m_tTextbox).x - 1), GetTextureSize(m_tTextbox).y);

    float midSize = m_vConsoleSize.x - (GetTextureSize(m_tTextbox).x - 1);
    m_rTextboxDestination[0] = hgeRect(0, tmpYPos, GetTextureSize(m_tTextbox).x - 1, tmpYPos + m_fFontHeight);
    m_rTextboxDestination[1] = hgeRect(GetTextureSize(m_tTextbox).x- 1, tmpYPos, midSize, tmpYPos + m_fFontHeight);
    m_rTextboxDestination[2] = hgeRect(midSize, tmpYPos, midSize + GetTextureSize(m_tTextbox).x - 1, tmpYPos + m_fFontHeight);
    
    //If console texture exists
    if (m_tConsole != NULL)
    {
		Vector2 texSize = GetTextureSize(m_tConsole);
        m_rConsoleTextureSource[0] = new hgeSprite(m_tConsole,0, 0, texSize.x - 1, texSize.y);
        m_rConsoleTextureSource[1] = new hgeSprite(m_tConsole,texSize.x - 1, 0, 0, texSize.y);
        m_rConsoleTextureSource[2] = new hgeSprite(m_tConsole,texSize.x - 1, 0, -(texSize.x - 1), texSize.y);
        m_rConsoleDestination[0] = hgeRect(0, 0, texSize.x - 1, tmpYPos);
        m_rConsoleDestination[1] = hgeRect(texSize.x - 1, 0, (texSize.x - 1) + (midSize - (texSize.x - 1)), tmpYPos);
        m_rConsoleDestination[2] = hgeRect(midSize, 0, (texSize.x - 1) + midSize, tmpYPos);
    }

	//Create Log
	remove(m_sLog.c_str());
	m_LogWriter.open(m_sLog.c_str(), std::ios::out|std::ios::app);

	//Setup Input
	m_pKeyboardState = new KeyboardState();
	m_pLastKeyboardState = new KeyboardState();
	m_pMouseState = new MouseState();
	m_pLastMouseState = new MouseState();

    AddDefaultCommands();
    AddDefaultCVars();

    Print("--- Intialised Console ---", DEBUG);
}

/// <summary>
/// Print a normal message to the console
/// </summary>
/// <param name="str">The message to print</param>
void Console::Print(string str)
{
    Print(str, NORM);
}

/// <summary>
/// Print a message to the console or just the textfile according to the ConsoleLevel
/// </summary>
/// <param name="str">The message to print</param>
/// <param name="level">The level of the message (indicates who and where the message is shown)</param>
void Console::Print(string str, ConsoleLevel level)
{
    //If we don't want to show debug just discard it
    if (level == DEBUG && m_eUsageLevel != DEBUGDEV)
        return;

    //If debug is active send all prints to the textfile
    if (m_eUsageLevel == DEBUGDEV)
    {
        if (m_LogWriter == NULL)
        {
            m_lFileBuffer.push_back(GetTime() + " " + str);
            m_lConsoleText.push_back(str);
            m_lLineColour.push_back(GetColour(level));
            return;
        }
        else
        {
            //Copies all the buffer to the file
            if (m_lFileBuffer.size() > 0)
            {
                for each (string tmpStr in m_lFileBuffer)
                    m_LogWriter << tmpStr << "\n";

                m_lFileBuffer.clear();
            }

            m_LogWriter << GetTime() << " " << str  << "\n";
			m_LogWriter.flush();
        }
    }

    //Textfile printing doesn't need to be added to the console
    if (level == TXTFILE)
        return;

    //Too Long for the line
    if (m_fFont->GetStringWidth(str.c_str()) > m_vConsoleSize.x)
    {
        //Split the line up
        vector<string> split = SplitString(str);

        for each (string splitStr in split)
        {
            m_lConsoleText.push_back(splitStr);
            m_lLineColour.push_back(GetColour(level));
        }
    }
    //Line is fine as it is just add it
    else
    {
        m_lConsoleText.push_back(str);
        m_lLineColour.push_back(GetColour(level));

        if (m_bOutputToVS)
        {
            OutputDebugString(str.c_str());
			OutputDebugString("\n");
        }
    }
    ResizeMessages();
}

/// <summary>
/// Split strings longer than the console window so that strings won't 
/// be drawn off the screen
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
vector<string> Console::SplitString(string str)
{
    vector<string> output = vector<string>();
    int maxChars = (int)m_vConsoleSize.x / (int)m_fFont->GetStringWidth("A");   

    while (m_fFont->GetStringWidth("A") > m_vConsoleSize.x)
    {
        //Check if theres a word at the split point
        char cBefore = str[maxChars-1];
        char cAfter = str[maxChars+1];

        if (isalnum(cBefore) && isalnum(cAfter))
        {
            //Check how long the word/number is by looking for the nearest space
            int pos=-1;
            int fSpace = -2;
            while (pos < maxChars-1 && pos != fSpace)
            {
                fSpace = pos;
                pos = str.find_first_of(' ', pos+1);
            }
            int lSpace = str.find_first_of(' ', maxChars + 1);

            //Word is too long we'll just have to cope with a broken word
            if (lSpace - fSpace > maxChars)
            {
                string tmpString = str.substr(0, maxChars);
                output.push_back(tmpString);
                str = str.erase(0, maxChars);
            }

            //Split the string at the beginning of the word
            else
            {
                string tmpString = str.substr(0, fSpace);
                output.push_back(tmpString);
                str = str.erase(0, fSpace);
            }
        }

        //Not a word/number so just split or its too long
        else
        {
            string tmpString = str.substr(0, maxChars);
            output.push_back(tmpString);
            str = str.erase(0, maxChars);
        }

    }
    output.push_back(str);
    return output;
}

/// <summary>
/// Outputs the current time, would like to move this to a GameTime but don't know how to extract the 
/// GameTime from a Game component
/// </summary>
/// <returns></returns>
string Console::GetTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime(buffer,80,"%H:%M:%S",timeinfo);

	return buffer;
}

/// <summary>
/// Handle command inputs, this is called when the return key is pressed
/// </summary>
/// <param name="cmd"></param>
void Console::HandleCommand(string cmd)
{
    string output = "User >> " + cmd;
    Print(output, TXTFILE);

    //Check to see if it parses
    CommandParser cmdParser = CommandParser(cmd);
    string commandName;
    vector<string> args;

    if (cmdParser.Parse(commandName, args))
    {
        if (!Dispatch(commandName, args))
            return;
    }
    else
    {
        output = "Command " + commandName + " has incorrect parameters.";
        Print(output, WARN);
    }
}

/// <summary>
/// Clear the console of all lines
/// </summary>
void Console::Clear()
{
    m_lConsoleText.clear();
    m_lLineColour.clear();
}

/// <summary>
/// Removes lines that won't be stored any longer
/// </summary>
void Console::ResizeMessages()
{
    float lineHeight = 0;
    float fontHeight = 0;
    if (CalculateLimits(lineHeight, fontHeight))
    {
        int messageCount = m_lConsoleText.size();
        if (messageCount > m_iMaxLines)
        {
            m_lConsoleText.erase(m_lConsoleText.begin(), m_lConsoleText.begin() + (messageCount - m_iMaxLines));
            m_lLineColour.erase(m_lLineColour.begin(), m_lLineColour.begin() + (messageCount - m_iMaxLines));
        }
    }

}

/// <summary>
/// Calculates the size of the fonts height and line size
/// </summary>
/// <param name="lineHeight"></param>
/// <param name="fontHeight"></param>
/// <returns></returns>
bool Console::CalculateLimits(float& lineHeight, float& fontHeight)
{
    if (m_fFont != NULL)
    {
        fontHeight = m_fFont->GetHeight();
        lineHeight = m_fFont->GetSpacing();

        return true;
    }

    lineHeight = 0;
    fontHeight = 0;
    return false;
}

/// <summary>
/// Adds an input to the history
/// </summary>
/// <param name="str"></param>
void Console::AddToHistory(string str)
{
    if (m_lHistoryText.size() >= HISTORY_SIZE)
    {
        m_lHistoryText.erase(m_lHistoryText.begin());
    }

    m_lHistoryText.push_back(str);
}

/// <summary>
/// Gets the colour matched with the console level given
/// </summary>
/// <param name="level"></param>
/// <returns></returns>
hgeColor Console::GetColour(ConsoleLevel level)
{
    switch (level)
    {
        case FAIL:
            return m_aTextColours[0];
        case WARN:
            return m_aTextColours[1];
        case DEBUG:
            return m_aTextColours[2];
        default:
            return m_aTextColours[3];
    }
}

/// <summary>
/// Sets the colour matched with the console level given
/// </summary>
/// <param name="level"></param>
/// <returns></returns>
void Console::SetTextColour(hgeColor colour, ConsoleLevel level)
{
    //Refresh Colours
    for (unsigned int i = 0; i < m_lLineColour.size(); i++)
    {
        switch (level)
        {
            case FAIL:
                if (m_lLineColour[i] == m_aTextColours[0])
                    m_lLineColour[i] = colour;
                break;
            case WARN:
                if (m_lLineColour[i] == m_aTextColours[1])
                    m_lLineColour[i] = colour;
                break;
            case DEBUG:
                if (m_lLineColour[i] == m_aTextColours[2])
                    m_lLineColour[i] = colour;
                break;
            case NORM:
                if (m_lLineColour[i] == m_aTextColours[3])
                    m_lLineColour[i] = colour;
                break;
        }
    }

    //Set new colour
    switch (level)
    {
        case FAIL:
            m_aTextColours[0] = colour;
            break;
        case WARN:
            m_aTextColours[1] = colour;
            break;
        case DEBUG:
            m_aTextColours[2] = colour;
            break;
        case NORM:
            m_aTextColours[3] = colour;
            break;
    }
}

/// <summary>
/// Updates the input for the console
/// </summary>
/// <param name="gameTime"></param>
void Console::Update(GameTime* gameTime)
{
    //if (!m_bEnabled || !m_bInitialised)
	if (!m_bInitialised || !GameManager::m_bGotFocus)
        return;

    //Check key input
	delete m_pLastKeyboardState;
	m_pLastKeyboardState = m_pKeyboardState;
    m_pKeyboardState = new KeyboardState();

	vector<int> keys = m_pKeyboardState->GetPressedKeys();
	
    //This allows us to use the Oem8 Key as a close and open without it closing in an instant
    if (m_dTimeOpened == 0)
        m_dTimeOpened = gameTime->GetTotalTime();
    else if (m_bJustOpened && gameTime->GetTotalTime() - m_dTimeOpened > m_fKeyStrokeSpeed)
        m_bJustOpened = false;

	for each (int key in keys)
	{
		double lastPress = gameTime->GetTotalTime() - m_lLastKeyPress;
		if (m_pLastKeyboardState->IsKeyUp(key) || lastPress > m_fKeyStrokeSpeed)
		{
			HandleKeyPress(key);
			UpdateKeyboard(gameTime);
		}
	}

    //If were still not drawing the console lets exit
    if (!m_bEnabled)
        return;

    m_vConsoleSize = Vector2(consoleBackground->GetWidth(), consoleBackground->GetHeight());

    //Mouse Scroll Wheel
	delete m_pLastMouseState;
	m_pLastMouseState = m_pMouseState;
    m_pMouseState = new MouseState();
    int wheelDiff = m_pLastMouseState->GetMouseWheelValue() - m_pMouseState->GetMouseWheelValue();
    if (wheelDiff < 0)
    {
        m_iCurrentLine -= -wheelDiff / 120;
        if (m_iCurrentLine < m_iConsoleLines - (int)m_lConsoleText.size())
            m_iCurrentLine = m_iConsoleLines - (int)m_lConsoleText.size();

        if (m_iCurrentLine > 0)
            m_iCurrentLine = 0;
    }
    if (wheelDiff > 0)
    {
        m_iCurrentLine += wheelDiff / 120;
        if (m_iCurrentLine > 0)
            m_iCurrentLine = 0;
    }
}

/// <summary>
/// Updates the keyboards last input
/// </summary>
/// <param name="gameTime"></param>
void Console::UpdateKeyboard(GameTime* gameTime)
{
    m_lLastKeyPress = gameTime->GetTotalTime();
}

/// <summary>
/// Handles any key press that occurs
/// </summary>
/// <param name="key"></param>
void Console::HandleKeyPress(int key)
{
	int normKey = key;
	key = KeyboardState::KeyToScanCode(key);
    //Open Console
    if (key == KEY_CONSOLE && !m_bEnabled)
    {
        m_bEnabled = true;
		m_bJustOpened = true;
    }

    //Console Not Open Don't take input
    if (!m_bEnabled)
        return;

    //Close Console
    if (key == KEY_CONSOLE && !m_bJustOpened || key == KEY_ESC && !m_bJustOpened)
    {
        m_bEnabled = false;
    }

    //Handle Command Input (Return)
    else if (key == KEY_RETURN)
    {
        if (m_sCommandText.length() > 0)
        {
            AddToHistory(m_sCommandText);
            HandleCommand(m_sCommandText);
            m_sCommandText = "";
            m_iCursorPos = 0;
            m_iHistoryIndex = 0;
        }
    }
    else if (key == KEY_BACKSPACE)
    {
        if (m_iCursorPos > 0)
        {
            m_sCommandText = m_sCommandText.erase(m_iCursorPos - 1, 1);
            m_iCursorPos--;
        }
    }
    else if (key == KEY_DEL)
    {
        //Delete a char
        if ((int)m_sCommandText.length() > m_iCursorPos)
            m_sCommandText = m_sCommandText.erase(m_iCursorPos, 1);
    }
    else if (key == KEY_LEFT && m_pKeyboardState->IsKeyDown(KEY_LSHIFT))
    {
        m_iCursorPos = 0;
    }
    else if (key == KEY_LEFT)
    {
        if (--m_iCursorPos < 0)
            m_iCursorPos = 0;
    }
    else if (key == KEY_RIGHT && m_pKeyboardState->IsKeyDown(KEY_LSHIFT))
    {
        m_iCursorPos = m_sCommandText.length();
    }
    else if (key == KEY_RIGHT)
    {
        if (++m_iCursorPos > (int)m_sCommandText.length())
            m_iCursorPos = m_sCommandText.length();
    }
    else if (key == KEY_HOME)
	{
        m_iCursorPos = 0;
    }
    else if (key == KEY_END)
    {
        m_iCursorPos = m_sCommandText.size();
    }
    else if (key == KEY_DOWN)
    {
        if (m_lHistoryText.size() > 0)
        {

            int index = m_iHistoryIndex + 1;
            if (index >= 0 && index < (int)m_lHistoryText.size())
            {
                m_iHistoryIndex++;
            }
            else
            {
                m_iHistoryIndex = 0;
            }
        }
        else
        {
            m_iHistoryIndex = 0;
        }

        if (m_lHistoryText.size() != 0)
        {
            m_sCommandText = m_lHistoryText[m_iHistoryIndex];
            m_iCursorPos = m_sCommandText.size();
        }
    }
    else if (key == KEY_UP)
    {
        if (m_lHistoryText.size() > 0)
        {

            int index = m_iHistoryIndex - 1;
            if (index >= 0 && index < (int)m_lHistoryText.size())
            {
                m_iHistoryIndex--;
            }
            else
            {
                m_iHistoryIndex = m_lHistoryText.size() - 1;
            }
        }
        else
        {
            m_iHistoryIndex = 0;
        }

        if (m_lHistoryText.size() != 0)
        {
            m_sCommandText = m_lHistoryText[m_iHistoryIndex];
            m_iCursorPos = m_sCommandText.size();
        }
    }
    else if (key == KEY_TAB)
    {
        AutoComplete();
    }
    else if (key == KEY_PGUP)
    {
        m_iCurrentLine--;
        if (m_iCurrentLine < m_iConsoleLines - (int)m_lConsoleText.size())
            m_iCurrentLine = m_iConsoleLines - m_lConsoleText.size();

        if (m_iCurrentLine > 0)
            m_iCurrentLine = 0;
    }
    else if (key == KEY_PGDOWN)
    {
        m_iCurrentLine++;
        if (m_iCurrentLine > 0)
            m_iCurrentLine = 0;
    }
    else
    {
		//Convert key to char
		char* tmpChar = KeyboardState::AsciiConvert(normKey);
        for (int i = 0; i < ALLOWED_INPUT_SIZE; i++)
		{

            if (!strcmp(tmpChar,m_aAInput[i]))
            {
                if (m_iCursorPos == 0)
                    m_sCommandText += m_aAInput[i];
                else
                    m_sCommandText = m_sCommandText.insert(m_iCursorPos, m_aAInput[i]);

                m_iCursorPos++;
                break;
            }
        }
		free(tmpChar);
    }

    //Check to see if the message is too long and trim it
    if (m_sCommandText.length() > INPUT_MAX)
    {
        m_sCommandText = m_sCommandText.substr(0, INPUT_MAX);
        if (m_iCursorPos > (int)m_sCommandText.length())
            m_iCursorPos = m_sCommandText.length();
    }
}

/// <summary>
/// Autocompletes a command if one is found, if there is more than
/// one command found then a list of commands will be printed,
/// and if all of those commands begin with the same characters
/// then those characters will be auto completed.
/// </summary>
void Console::AutoComplete()
{
    //We don't have anything to work with return
    if (m_sCommandText.size() == 0)
        return;

    vector<string> cmdNames;
    string tmpCommand = m_sCommandText;
    bool cvar = false;
    bool help = false;
    m_sCommandText = toLower(m_sCommandText);

    //If we are tabCompleting with cvar
    if (m_sCommandText.length() > 5 && m_sCommandText.substr(0, 5) == "cvar ")
    {
        tmpCommand = m_sCommandText.substr(5);
        m_sCommandText = toLower(m_sCommandText);
        cmdNames = GetCVars();
        cvar = true;
    }

    //If we are tabCompleting with help
    else if (m_sCommandText.length() > 5 && m_sCommandText.substr(0, 5) == "help ")
    {
        tmpCommand = m_sCommandText.substr(5);
        m_sCommandText = toLower(m_sCommandText);
        cmdNames = GetCommands();
        help = true;
    }

    //All other commands
    else              
        cmdNames = GetCommands();

    vector<string> output = vector<string>();

    for each (string str in cmdNames)
    {
        if (str.size() == tmpCommand.size())
        {
            //We already have the command!
            if (toLower(str) == toLower(tmpCommand))
                return;
        }
        else if (tmpCommand.size() < str.size())
        {
            string tmpStr;
            tmpStr = str;
            if (toLower(str.substr(0, tmpCommand.size())) == toLower(tmpCommand))
            {
                //If were doing a cvar
                if (cvar)
                {
                    if (GetCVar(str)->GetUsageLevel() < m_eUsageLevel)
                        continue;

                    tmpStr = "cvar " + str;
                }
                //If were doing help
                else if (help)
                {
                    if (GetCommand(str)->GetUsageLevel() < m_eUsageLevel)
                        continue;

                    tmpStr = "help " + str;
                }
                else
                {
                    if (GetCommand(str)->GetUsageLevel() < m_eUsageLevel)
                        continue;
                }
                output.push_back(tmpStr);
            }
        }
    }

    //We found none
    if (output.size() == 0)
        return;

    //If we have just one complete the command
    if (output.size() == 1)
    {
        m_sCommandText = output[0] + " ";
        m_iCursorPos = m_sCommandText.length();
        return;
    }
        
    //Sort Alphabetically: TODO
    //output.Sort();

    //Check for the smallest sized choice we have
    int smallestSize = 9999;
    for (unsigned int i = 0; i < output.size(); i++)
    {
        if ((int)output[i].length() < smallestSize)
            smallestSize = output[i].length();
    }

    //Now we find if the start substring of all the commands are the same
    string subStr = "";
    for (int i = 0; i < smallestSize; i++)
    {
        bool charDifferent = false;
        for (unsigned int y = 0; y < output.size(); y++)
        {
            if ((output[0][i] != output[y][i]))
            {
                charDifferent = true;
                break;
            }
        }
        if (charDifferent)
            break;
        else
            subStr += output[0][i];
    }

    //Adds the substring found in all the commands
    if (subStr.size() != m_sCommandText.length())
    {
        m_sCommandText = subStr;
        m_iCursorPos = m_sCommandText.length();
    }
    else
    {
        //Otherwise show the list of commands
        string msg = "Available Commands: ";
        for each (string str in output)
            msg += str + ",";
        msg = msg.erase(msg.size() -1, 1);
        Print(msg, NORM);
    }
}

/// <summary>
/// Draw the console
/// </summary>
/// <param name="gameTime"></param>
void Console::Draw(GameTime* gameTime)
{
    if (!m_bEnabled)
        return;

    //Draw the background
    if (m_bShowBackground)
    {
        //If console texture draw the texture
        if (m_tConsole != NULL)
        {
			m_rConsoleTextureSource[0]->RenderStretch(m_rConsoleDestination[0].x1, m_rConsoleDestination[0].y1, m_rConsoleDestination[0].x2, m_rConsoleDestination[0].y2);
			m_rConsoleTextureSource[1]->RenderStretch(m_rConsoleDestination[1].x1, m_rConsoleDestination[1].y1, m_rConsoleDestination[1].x2, m_rConsoleDestination[1].y2);
			m_rConsoleTextureSource[2]->RenderStretch(m_rConsoleDestination[2].x1, m_rConsoleDestination[2].y1, m_rConsoleDestination[2].x2, m_rConsoleDestination[2].y2);
        }
        else
            consoleBackground->Render(0,0);
    }

    //Used to draw the lines in the correct place
    Vector2 tmpPos = Vector2(GetTextureSize(m_tTextbox).x + 1,0);

	//Get first & last line (Starts drawing with the right line)
    int fLine = 0;
    int	lLine = m_lConsoleText.size();
    if ((int)m_lConsoleText.size() > m_iConsoleLines)
    {
        fLine = (m_iCurrentLine - m_iConsoleLines) + m_lConsoleText.size();
        lLine = fLine + m_iConsoleLines;
    }

    //Draw the console Text
    for (int i = fLine; i < lLine; i++)
    {
		m_fFont->SetColor(m_lLineColour[i].GetHWColor());
		m_fFont->Render(tmpPos.x, tmpPos.y, HGETEXT_LEFT, m_lConsoleText[i].c_str());
        tmpPos += Vector2(0,m_fFontHeight);
    }

    //Draw the Textbox
	m_rTextboxTextureSource[0]->RenderStretch(m_rTextboxDestination[0].x1, m_rTextboxDestination[0].y1, m_rTextboxDestination[0].x2, m_rTextboxDestination[0].y2);
	m_rTextboxTextureSource[1]->RenderStretch(m_rTextboxDestination[1].x1, m_rTextboxDestination[1].y1, m_rTextboxDestination[1].x2, m_rTextboxDestination[1].y2);
	m_rTextboxTextureSource[2]->RenderStretch(m_rTextboxDestination[2].x1, m_rTextboxDestination[2].y1, m_rTextboxDestination[2].x2, m_rTextboxDestination[2].y2);
    //m_pSpriteBatch.Draw(m_tTextbox, m_rTextboxDestination[0], m_rTextboxTextureSource[0], Color.White);
    //m_pSpriteBatch.Draw(m_tTextbox, m_rTextboxDestination[1], m_rTextboxTextureSource[1], Color.White);
    //m_pSpriteBatch.Draw(m_tTextbox, m_rTextboxDestination[2], m_rTextboxTextureSource[2], Color.White);

    //Draw the input text
    float charWidth = m_fFont->GetStringWidth("A");
    tmpPos = Vector2(charWidth, (float)m_iConsoleLines * m_fFontHeight);
    string tmpOutput;

    if (m_iCursorPos - (m_vConsoleSize.x / charWidth) < 0)
        tmpOutput = m_sCommandText;
    else
        tmpOutput = m_sCommandText.substr(m_iCursorPos - (int)(m_vConsoleSize.x / charWidth));

    //TODO: Cursor blink
	if ((int)(gameTime->GetTotalTime()*1000) % 20 == 0)
        m_bShowCursor = !m_bShowCursor;

	m_fFont->SetColor(m_aTextColours[3].GetHWColor());
	m_fFont->Render(tmpPos.x, tmpPos.y, HGETEXT_LEFT, tmpOutput.c_str());

    //Draw Cursor
    if (m_bShowCursor)
    {
        tmpPos = Vector2(charWidth * (float)(m_iCursorPos + 1) - 3,tmpPos.y);
		m_fFont->SetColor(m_aTextColours[3].GetHWColor());
		m_fFont->Render(tmpPos.x, tmpPos.y, HGETEXT_LEFT, "|");
    }
}

//*==========================*
//*=    Start of Commands   =*
//*==========================*

/// <summary>
/// Add the default commands to the console
/// </summary>
void Console::AddDefaultCommands()
{
	RegisterCommand(new Command_Init(this));
	RegisterCommand(new Command_CVar());
	RegisterCommand(new Command_CLS());
	RegisterCommand(new Command_Quit());
	RegisterCommand(new Command_Help());
	RegisterCommand(new Command_ListCommands());
	RegisterCommand(new Command_LoadConfig());
	RegisterCommand(new Command_SaveConfig());
}

/// <summary>
/// Register a command to the console if it doesn't already exist
/// </summary>
/// <param name="cmd"></param>
/// <returns></returns>
bool Console::RegisterCommand(Command* cmd)
{
    //If the command doesn't exist
    if (!HasCommand(cmd->GetName()))
    {
		m_dCommands[cmd->GetName()] = cmd;
        return true;
    }

    return false;
}

/// <summary>
/// Does the console have this command?
/// </summary>
/// <param name="cmd"></param>
/// <returns></returns>
bool Console::HasCommand(string cmd)
{
    if (m_dCommands.find(cmd) != m_dCommands.end())
        return true;

    return false;
}

/// <summary>
/// Run the command that has been supplied
/// </summary>
/// <param name="cmd"></param>
/// <param name="args"></param>
/// <returns></returns>
bool Console::Dispatch(string cmd, vector<string> args)
{
    if (m_dCommands.find(cmd) != m_dCommands.end())
    {
        Command* tmpCommand = m_dCommands[cmd];
        if (tmpCommand->GetUsageLevel() < m_eUsageLevel)
        {
            Print("Don't have the required permission to use the command: " + cmd, WARN);
            return false;
        }
        if (!tmpCommand->Invoke(args))
        {
            if (tmpCommand->GetName() == "cvar" && args.size() > 0)
            {
                //Show CVar Not Found
                string output = "CVar " + args[0] + " not found.";
                Print(output, WARN);
                return false;
            }

            //Show Help
            string outs = "Command " + cmd + " usage: " + tmpCommand->GetUsage();
            Print(outs, WARN);
            return false;
        }
    }
    else
    {
        string output = "Command " + cmd + " doesn't exist.";
        Print(output, WARN);
        return false;
    }

    return true;
}

/// <summary>
/// Returns a command if it exists
/// </summary>
/// <param name="cmdName"></param>
/// <returns></returns>
Command* Console::GetCommand(string cmdName)
{
	map<string,Command*>::iterator iter;
	if ((iter = m_dCommands.find(cmdName)) == m_dCommands.end())
        return NULL;
    else
        return iter->second;
}

/// <summary>
/// Returns all commands added to the console
/// </summary>
/// <returns></returns>
vector<string> Console::GetCommands()
{
    vector<string> output = vector<string>();

    for each (pair<string, Command*> keyPair in m_dCommands)
    {
        output.push_back(keyPair.first);
    }

    return output;
}

//*==========================*
//*=    End of Commands     =*
//*==========================*


//*==========================*
//*=    Start of CVars      =*
//*==========================*

/// <summary>
/// Add default CVars to the console
/// </summary>
void Console::AddDefaultCVars()
{
    RegisterCVar(new CVar_Init(this));
	RegisterCVar(new CVar_ScreenWidth());
	RegisterCVar(new CVar_ScreenHeight());
	RegisterCVar(new CVar_FullScreen());
	RegisterCVar(new CVar_Language());
	RegisterCVar(new CVar_AudioLevel());
}

/// <summary>
/// Register a CVar with the console
/// </summary>
/// <param name="cvar"></param>
/// <returns></returns>
bool Console::RegisterCVar(CVar* cvar)
{
    //If the command doesn't exist
    if (!HasCVar(cvar->GetName()))
    {
		m_dCVars[cvar->GetName()] = cvar;
        return true;
    }

    return false;
}

/// <summary>
/// Does the console have this CVar?
/// </summary>
/// <param name="cvar"></param>
/// <returns></returns>
bool Console::HasCVar(string cvar)
{
    if (m_dCVars.find(cvar) != m_dCVars.end())
        return true;

    return false;
}

/// <summary>
/// Set the current value of a CVar, requires a list of args.
/// </summary>
/// <param name="cvarName"></param>
/// <param name="args"></param>
/// <returns></returns>
bool Console::SetCVar(string cvarName, vector<string> args)
{
	map<string,CVar*>::iterator iter;

    if ((iter = m_dCVars.find(cvarName)) == m_dCVars.end())
        return false;
	
	CVar* tmpCVar = iter->second;
    if (tmpCVar != NULL)
    {
        if (tmpCVar->GetUsageLevel() < m_eUsageLevel)
        {
            Print("Don't have the required permission to use the CVar: " + cvarName, WARN);
            return false;
        }

        //If failure
        if (!tmpCVar->Invoke(args))
        {
            string output = "CVar " + cvarName;

            if (args.size() == 0)
            {
                output += " value: " + tmpCVar->ToString();
            }

            output += " usage: " + tmpCVar->GetUsage();
            output += " (default: " + tmpCVar->GetDefaultValue() + ")";
            Print(output, WARN);

            return false;
        }
    }
    else
    {
        Print("CVar " + cvarName + " doesn't exist.", WARN);
        return false;
    }

    return true;
}

/// <summary>
/// Set the current value of a CVar, this will also parse the 
/// arguments from a string
/// </summary>
/// <param name="cvarName"></param>
/// <param name="strArgs"></param>
/// <returns></returns>
bool Console::SetCVar(string cvarName, string strArgs)
{
    CommandParser parser = CommandParser(cvarName + " " + strArgs);
    vector<string> args;

    if (!parser.Parse(cvarName, args))
    {
        return false;
    }

	map<string,CVar*>::iterator iter;
	if ((iter = m_dCVars.find(cvarName)) == m_dCVars.end())
        return false;

	CVar* tmpCVar = iter->second;
    if (tmpCVar != NULL)
    {
        //If failure
        if (!tmpCVar->Invoke(args))
        {
            string output = "CVar " + cvarName;

            if (args.size() == 0)
            {
                output += " value: " + tmpCVar->ToString();
            }

            output += " usage: " + tmpCVar->GetUsage();
            output += " (default: " + tmpCVar->GetDefaultValue() + ")";
            Print(output, WARN);

            return false;
        }
    }
    else
    {
        Print("CVar " + cvarName + " doesn't exist.", WARN);
        return false;
    }

    return true;
}

/// <summary>
/// Returns a CVar if it exists
/// </summary>
/// <param name="cvarName"></param>
/// <returns></returns>
CVar* Console::GetCVar(string cvarName)
{
	map<string, CVar*>::iterator iter;
	if ((iter = m_dCVars.find(cvarName)) == m_dCVars.end())
        return NULL;
    else
        return iter->second;
}

/// <summary>
/// Get all CVars registered with the console
/// </summary>
/// <returns></returns>
vector<string> Console::GetCVars()
{
    vector<string> output = vector<string>();

    for each (pair<string, CVar*> keyPair in m_dCVars)
        output.push_back(keyPair.first);

    return output;
}

/// <summary>
/// Get all the CVars that are to be saved to an external file
/// </summary>
/// <returns></returns>
vector<CVar*> Console::GetSaveCVars(SaveType saveType)
{
    vector<CVar*> output = vector<CVar*>();

    for each (pair<string, CVar*> keyPair in m_dCVars)
    {
        if (keyPair.second->GetSaveToFile() == saveType)
            output.push_back(keyPair.second);
    }

    return output;
}

//*==========================*
//*=      End of CVars      =*
//*==========================*

char* Console::m_aAInput[] = 
{
	"a","b","c","d","e","f","g","h","i","j",
	"k","l","m","n","o","p","q","r","s","t",
	"u","v","w","x","y","z","A","B","C","D",
	"E","F","G","H","I","J","K","L","M","N",
	"O","P","Q","R","S","T","U","V","W","X",
	"Y","Z","1","2","3","4","5","6","7","8",
	"9","0","-","=","!","/","£","$","%","^",
	"&","*","(",")","-","{","}","[","]",":",
	";","@","'","~","#","<",",",">",".","?",
	"/","\\","_","\""," "
};