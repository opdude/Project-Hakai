#include "HelperFunctions.h"
#include "GameManager.h"
#include "GameScreen.h"
#include "ScreenManager.h"


string toLower(string str)
{
	for(unsigned int i=0; i < str.size(); i++)
		str[i] = tolower(str[i]);

	return str;
}

string toString(float f)
{
	string output;
	char c[255];
	sprintf(c,"%f",f);
	output += c;
	return output;
}

string toString(DWORD d)
{
	string output;
	char c[255];
	sprintf(c,"%i",d);
	output += c;
	return output;
}

float GetScreenWidth()
{
	if (GameManager::Instance()->GetHGE()->System_GetState(HGE_SCREENWIDTH)/
		GameManager::Instance()->GetHGE()->System_GetState(HGE_SCREENHEIGHT) == 1.6)
		return (float)16800;
	else
		return 14000;
}

float GetScreenHeight()
{
	return (float)10500;
}

Vector2 GetTextureSize(HTEXTURE tex)
{
	if (tex == NULL)
		return Vector2(0,0);

	return Vector2((float)GameManager::Instance()->GetHGE()->Texture_GetWidth(tex),
		(float)GameManager::Instance()->GetHGE()->Texture_GetHeight(tex));
}

HTEXTURE LoadTex(char* path)
{
	return GameManager::Instance()->GetHGE()->Texture_Load(path);
}

float Clamp(float val, float min, float max)
{
	if (val < min)
		val = min;
	else if (val > max)
		val = max;
	return val;
}

float DegreeToRadian(float degree)
{
	return degree * (3.14f / 180);
}

float VirtXToScr(float virtXPixel)
{
	int width = GameManager::Instance()->GetHGE()->System_GetState(HGE_SCREENWIDTH);
	return virtXPixel * width / GetScreenWidth();
}

float VirtYToScr(float virtYPixel)
{
	int height = GameManager::Instance()->GetHGE()->System_GetState(HGE_SCREENHEIGHT);
	return virtYPixel * height / GetScreenHeight();
}

float ScrXToVirt(float scrXPixel)
{
	int width = GameManager::Instance()->GetHGE()->System_GetState(HGE_SCREENWIDTH);
	return scrXPixel * GetScreenWidth() / width;
}

float ScrYToVirt(float scrYPixel)
{
	int height = GameManager::Instance()->GetHGE()->System_GetState(HGE_SCREENHEIGHT);
	return scrYPixel * GetScreenHeight() / height;
}

float FontXScale(hgeFont* font, float desiredWidth, char* text)
{
	return desiredWidth / GameManager::Instance()->GetScreenManager()->GetMenuFont()->GetStringWidth(text);
}

extern float FontYScale(hgeFont* font, float desiredHeight)
{
	return desiredHeight / ScrYToVirt(GameManager::Instance()->GetScreenManager()->GetMenuFont()->GetHeight());
}

extern int RandomInt(int min, int max)
{
	return GameManager::Instance()->GetHGE()->Random_Int(min,max);
}

float RandomFloat(float min, float max)
{
	return GameManager::Instance()->GetHGE()->Random_Float(min,max);
}

Vector2 RandomVector2(float min, float max)
{
	return RandomVector2(min,max,min,max);
}

Vector2 RandomVector2(float minX, float maxX, float minY, float maxY)
{
	return Vector2(GameManager::Instance()->GetHGE()->Random_Float(minX,maxX),
		GameManager::Instance()->GetHGE()->Random_Float(minY,maxY));
}