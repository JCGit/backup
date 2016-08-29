
#include "stdafx.h"

#include "BlackBoard.h"
#include "cocos2d.h"
#include "StringConverter.h"
#include "Language.h"


BlackBoard::BlackBoard( void )
{
	mVaribleMap.clear();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		PLATFORM_TYPE_FOR_LUA = 1;
	#endif
	#ifdef PROJECTAPPSTORE
		PLATFORM_TYPE_FOR_LUA = 2;
	#endif
}


bool BlackBoard::hasVarible( const std::string& key )
{
	std::map<std::string,std::string>::iterator itr = mVaribleMap.find(key);
	if(itr!=mVaribleMap.end())
		return true;
	return false;
}

bool BlackBoard::addVarible( const std::string& key, const std::string& value )
{
	std::map<std::string,std::string>::iterator itr = mVaribleMap.find(key);
	if(itr!=mVaribleMap.end())
	{
		char msg[256];
		sprintf(msg,"key %s is already exist in the map!",key.c_str());
		cocos2d::CCMessageBox(msg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
		return false;
	}
	else
	{
		mVaribleMap[key] = value;
		return true;
	}
}

bool BlackBoard::addVarible( const std::string& key, unsigned int value )
{
	return addVarible(key, StringConverter::toString(value));
}

bool BlackBoard::addVarible( const std::string& key, bool value )
{
	return addVarible(key, StringConverter::toString(value));
}

void BlackBoard::delVarible( const std::string& key )
{
	mVaribleMap.erase(key);
}

bool BlackBoard::setVarible( const std::string& key, const std::string& value )
{
	std::map<std::string,std::string>::iterator itr = mVaribleMap.find(key);
	if(itr!=mVaribleMap.end())
	{
		itr->second = value;
		return true;
	}
	else
	{
		char msg[256];
		sprintf(msg,"can not find key %s in map!",key.c_str());
		cocos2d::CCMessageBox(msg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
		return false;
	}
}

bool BlackBoard::setVarible( const std::string& key, bool value )
{
	return setVarible(key, StringConverter::toString(value));
}

bool BlackBoard::setVarible( const std::string& key, unsigned int value )
{
	return setVarible(key, StringConverter::toString(value));
}

std::string BlackBoard::fetchVarible( const std::string& key )
{
	std::map<std::string,std::string>::iterator itr = mVaribleMap.find(key);
	if(itr!=mVaribleMap.end())
	{
		std::string s( mVaribleMap.find(key)->second );
		mVaribleMap.erase(itr);
		return s;
	}
	else
	{
		char msg[256];
		sprintf(msg,"can not find key %s in map!",key.c_str());
		cocos2d::CCMessageBox(msg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
		return "";
	}
}

unsigned int BlackBoard::fetchVaribleAsUint( const std::string& key )
{
	return StringConverter::parseUnsignedInt(fetchVarible(key));
}

bool BlackBoard::fetchVaribleAsBool( const std::string& key )
{
	return StringConverter::parseBool(fetchVarible(key));
}

void BlackBoard::clearMap()
{
	mVaribleMap.clear();
}

std::string BlackBoard::getVarible( const std::string& key )
{
	std::map<std::string,std::string>::iterator itr = mVaribleMap.find(key);
	if(itr!=mVaribleMap.end())
	{
		return mVaribleMap.find(key)->second;
	}
	else
	{
		char msg[256];
		sprintf(msg,"can not find key %s in map!",key.c_str());
		cocos2d::CCMessageBox(msg,Language::Get()->getString("@ShowMsgBoxTitle").c_str());
		return "";
	}
}

unsigned int BlackBoard::getVaribleAsUint( const std::string& key )
{
	return StringConverter::parseUnsignedInt(getVarible(key));
}

bool BlackBoard::getVaribleAsBool( const std::string& key )
{
	return StringConverter::parseBool(getVarible(key));
}

BlackBoard* BlackBoard::getInstance()
{
	return BlackBoard::Get();
}
