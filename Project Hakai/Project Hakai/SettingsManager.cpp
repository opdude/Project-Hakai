//
//	SettingsManager.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "SettingsManager.h"
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include "Console.h"
#include "CVar.h"
#include "GameManager.h"

#define ENCKEY 1337

void SettingsManager::LoadCVars()
{
	GameManager::Instance()->GetHGE()->System_SetState(HGE_INIFILE, "config.ini");

	vector<CVar*> cvars = Console::Instance()->GetSaveCVars(SAVE_INI);

	for each(CVar* cvar in cvars)
	{
		Console::Instance()->SetCVar(cvar->GetName(), GameManager::Instance()->GetHGE()->Ini_GetString("CVars",cvar->GetName().c_str(),cvar->GetDefaultValue().c_str()));
	}

	//Load Serialised CVars
	try
	{
		ifstream ifs("Profile.sve", std::ios_base::binary);
		string tmp;
		while (ifs.good())
			tmp += ifs.get();

		//Simple De-Cryption
		for(unsigned int i=0; i < tmp.size(); i++)
			tmp[i] -= ENCKEY;

		istringstream is(tmp);
		
		SerialisedCVars sCvars;
		{
			boost::archive::text_iarchive ia(is);
			ia >> sCvars;
		}
		map<std::string,std::string> cvarMap = sCvars.m_mSerialisedCVars;
		for each(pair<std::string,std::string> pair in cvarMap)
		{
			Console::Instance()->SetCVar(pair.first,pair.second);
		} 
	}
	catch(...)
	{
		//Read Exception Caught
	}

}

void SettingsManager::SaveCVars()
{
	GameManager::Instance()->GetHGE()->System_SetState(HGE_INIFILE, "config.ini");
	vector<CVar*> cvars = Console::Instance()->GetSaveCVars(SAVE_INI);

	for each(CVar* cvar in cvars)
	{
		GameManager::Instance()->GetHGE()->Ini_SetString("CVars",cvar->GetName().c_str(),cvar->ToString().c_str());
	}

	cvars = Console::Instance()->GetSaveCVars(SAVE_SERIALISE);

	map<std::string,std::string> cvarMap = map<std::string,std::string>();
	
	for each(CVar* c in cvars)
	{
		cvarMap[c->GetName()] =c->ToString();
	}

	//Serialise & Write
	ostringstream oss;
	SerialisedCVars sCvars(cvarMap);
	{
		boost::archive::text_oarchive oa(oss);
		oa << sCvars;
	}

	//Simple Encryption
	string tmp =  oss.str();
	for(unsigned int i=0; i < tmp.size(); i++)
		tmp[i] += ENCKEY;
	
	fstream fs("Profile.sve", std::ios_base::binary | fstream::out);
	fs << tmp.c_str();
	fs.close();
}