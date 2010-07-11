//
//	Parser.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "Parser.h"

int Parser::ToInt(string str)
{
	return atoi(str.c_str());
}

float Parser::ToFloat(string str)
{
	return (float)atof(str.c_str());
}

double Parser::ToDouble(string str)
{
	return atof(str.c_str());
}

hgeColor Parser::ToColour(string str)
{
	float r=0, g=0, b=0, a=0;
	string tmpVal;

	tmpVal = str;

	int pos = tmpVal.find_first_of(' ');

	//Find Red
	if (pos < 0)
		return hgeColor(0,0,0,255);
	else
	{
		r = Parser::ToFloat(tmpVal.substr(0,3));
	}

	//Find Green
	pos = tmpVal.find_first_of(' ', pos + 1);
	if (pos < 0)
		return hgeColor(r,0,0,255);
	else
	{
		g = Parser::ToFloat(tmpVal.substr(0,3));
	}

	//Find Blue
	pos = tmpVal.find_first_of(' ', pos + 1);
	if (pos < 0)
		return hgeColor(r,g,0,255);
	else
	{
		b = Parser::ToFloat(tmpVal.substr(0,3));
	}

	//Find Alpha
	pos = tmpVal.find_first_of(' ', pos + 1);
	if (pos < 0)
		return hgeColor(r,g,b,255);
	else
	{
		a = Parser::ToFloat(tmpVal.substr(0,3));
	}

	return hgeColor(r,g,b,a);
}

Vector2 Parser::ToVector2(string str)
{
	float x=0, y=0;
	string tmpVal;

	tmpVal = str;

	int pos = tmpVal.find_first_of(' ');

	//Find X
	if (pos < 0)
		return Vector2(0,0);
	else
	{
		x = Parser::ToFloat(tmpVal.substr(0,3));
	}

	//Find Y
	pos = tmpVal.find_first_of(' ', pos + 1);
	if (pos < 0)
		return Vector2(x,0);
	else
	{
		y = Parser::ToFloat(tmpVal.substr(0,3));
	}

	return Vector2(x,y);
}

hgeRect Parser::ToRectangle(string str)
{
	float x=0, y=0, w=0, h=0;
	string tmpVal;

	tmpVal = str;

	int pos = tmpVal.find_first_of(' ');

	//Find X
	if (pos < 0)
		return hgeRect(0,0,0,0);
	else
	{
		x = Parser::ToFloat(tmpVal.substr(0,3));
	}

	//Find Y
	pos = tmpVal.find_first_of(' ', pos + 1);
	if (pos < 0)
		return hgeRect(x,0,0,0);
	else
	{
		y = Parser::ToFloat(tmpVal.substr(0,3));
	}

	//Find Width
	pos = tmpVal.find_first_of(' ', pos + 1);
	if (pos < 0)
		return hgeRect(x,y,0,0);
	else
	{
		w = Parser::ToFloat(tmpVal.substr(0,3));
	}

	//Find Height
	pos = tmpVal.find_first_of(' ', pos + 1);
	if (pos < 0)
		return hgeRect(x,y,w,0);
	else
	{
		h = Parser::ToFloat(tmpVal.substr(0,3));
	}

	return hgeRect(x,y,w,h);
}

bool Parser::ToBool(string str)
{
	if (!strcmp(toLower(str).c_str(), "true"))
		return true;
	else if (!strcmp(toLower(str).c_str(), "1"))
		return true;

	return false;
}

char Parser::ToChar(string str)
{
	//TODO
	return str[0];
}