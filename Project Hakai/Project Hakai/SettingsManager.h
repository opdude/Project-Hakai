#ifndef _SETTINGSMANAGER_H
#define _SETTINGSMANAGER_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <fstream>
#include <map> 
#include <vector>
#include <hge.h>

class CVar;

using namespace std;

class SerialisedCVars
{
public:
	SerialisedCVars() {};
	SerialisedCVars(map<std::string,std::string> serialisedCVars)
	{
		m_mSerialisedCVars = serialisedCVars;
	}
	~SerialisedCVars(){};

	map<std::string,std::string> m_mSerialisedCVars;

private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & m_mSerialisedCVars;
	}
};

class SettingsManager
{
public:
	static void LoadCVars();
	static void SaveCVars();
};

#endif