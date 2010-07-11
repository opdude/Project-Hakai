#ifndef _DEFAULTCVARS_H
#define _DEFAULTCVARS_H

#include <string>
#include <vector>
#include "CVar.h"

using namespace std;

class CVar_ScreenWidth : public CVar
{
public:
	CVar_ScreenWidth();
	virtual bool Invoke(vector<string> args);
};

class CVar_ScreenHeight : public CVar
{
public:
	CVar_ScreenHeight();
	virtual bool Invoke(vector<string> args);
};

class CVar_FullScreen : public CVar
{
public:
	CVar_FullScreen();
	virtual bool Invoke(vector<string> args);
};

class CVar_Language : public CVar
{
public:
	CVar_Language();
	virtual bool Invoke(vector<string> args);
};

class CVar_AudioLevel : public CVar
{
public:
	CVar_AudioLevel();
	virtual bool Invoke(vector<string> args);
};

#endif