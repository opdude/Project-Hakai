//
//	Parser.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _PARSER_H
#define _PARSER_H

#include <string>
#include <hgecolor.h>
#include <hgerect.h>
#include "HelperFunctions.h"
#include "Vector2.h"

using namespace std;

class Parser
{
public:
	static int ToInt(string str);
	static float ToFloat(string str);
	static double ToDouble(string str);
	static hgeColor ToColour(string str);
	static Vector2 ToVector2(string str);
	static hgeRect ToRectangle(string str);
	static bool ToBool(string str);
	static char ToChar(string str);

};

#endif
