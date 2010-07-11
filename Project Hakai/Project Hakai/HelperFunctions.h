//
//	HelperFunctions.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _HELPERFUNCTIONS_H
#define _HELPERFUNCTIONS_H

#include <string>
#include <boost/format.hpp>
#include <hgefont.h>
#include "Vector2.h"

#ifdef _DEBUG
	//Memory Leak
	#define _CRTDBG_MAP_ALLOC   
	#include <crtdbg.h> 

	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif

#define MAX_PLAYERS 4

using namespace std;

//String Functions
extern string toLower(string str);
extern string toString(float f);
extern string toString(DWORD d);

//Screen Functions
extern float GetScreenWidth();
extern float GetScreenHeight();

//Texture Functions
extern Vector2 GetTextureSize(HTEXTURE tex);
extern HTEXTURE LoadTex(char* path);
extern hgeFont LoadFont(char* path);

//Math Functions
extern float Clamp(float val, float min, float max);
extern float DegreeToRadian(float degree);

//Scale Functions
extern float VirtXToScr(float virtXPixel);
extern float VirtYToScr(float virtYPixel);
extern float ScrXToVirt(float scrXPixel);
extern float ScrYToVirt(float scrYPixel);
extern float FontXScale(hgeFont* font, float desiredWidth, char* charLength);
extern float FontYScale(hgeFont* font, float desiredHeight);

//Random Functions
extern int RandomInt(int min, int max);
extern float RandomFloat(float min, float max);
extern Vector2 RandomVector2(float min, float max);
extern Vector2 RandomVector2(float minX, float maxX, float minY, float maxY);


#endif