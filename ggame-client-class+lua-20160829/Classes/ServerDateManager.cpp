
#include "stdafx.h"

#include "ServerDateManager.h"
#include "MessageBoxPage.h"
#include "BlackBoard.h"
#include "STLUtility.h"
#include "TimeCalculator.h"
#include "StringConverter.h"

ServerDateManager::ServerDateManager()
{
	m_bSerQuietRestart = false;
}

ServerDateManager* ServerDateManager::getInstance()
{
	return ServerDateManager::Get();
}

bool ServerDateManager::getServerWillRestart()
{
	//静默停服标志位
	bool QuietStartOpen = StringConverter::parseBool(VaribleManager::Get()->getSetting("QuietRestartServer", "", "false"),false);
	if (QuietStartOpen == false)
	{
		return false;
	}else{
		return m_bSerQuietRestart;
	}
	
}

void ServerDateManager::reset(bool changedPlayer)
{
	//
	mLoginInfo.reset();
	//
	mUserPlayerInfo.Clear();
	//
	mUserRoleInfo.Clear();
	//
	clearRoleInfo();
	//
	clearEquipInfo();
	//
	clearItemInfo();
	//
	clearSkillInfo();
	//
	mPlayerState.Clear();
	//
	//
	if (changedPlayer)
	{
		cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine()
			->executeGlobalFunctionByName("resetAllLuaDataAndScene", NULL, "ServerDateManager::reset(true)");
	}
	else
	{
		cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine()
			->executeGlobalFunctionByName("reloginScene", NULL, "ServerDateManager::reset(false)");
	}
}


bool ServerDateManager::hasEquipInfoById(int64_t id )
{	
	if(mUserEquipInfoMap.find(id) != mUserEquipInfoMap.end())
		return true;
	else
	{
		return false;
	}
}

void ServerDateManager::insertEquipInfoInMap(EquipInfo equip){
	if (equip.has_id())
	{
		mUserEquipInfoMap.insert(UserEquipInfoList::value_type(equip.id(),equip));
	}
}


void ServerDateManager::removeEquipInfoById( int64_t id)
{
	if(mUserEquipInfoMap.find(id) != mUserEquipInfoMap.end())
	{
		mUserEquipInfoMap.erase(mUserEquipInfoMap.find(id));
	}
}

void ServerDateManager::clearEquipInfo()
{
	mUserEquipInfoMap.clear();
}

EquipInfo ServerDateManager::getEquipInfoById( int64_t id )
{
	EquipInfo info;
	if(mUserEquipInfoMap.find(id) != mUserEquipInfoMap.end())
		info = mUserEquipInfoMap.find(id)->second;
	return info;
}

std::string	ServerDateManager::getEquipInfoByIdForLua(int64_t id){

	EquipInfo info = getEquipInfoById(id);
	std::string ret = info.SerializeAsString();
	return ret;
}

EquipInfo ServerDateManager::getEquipInfoByIndex( int index )
{
	EquipInfo info;
	if(index < 0 || index>=mUserEquipInfoMap.size())
	{
		CCLOG("@ServerDateManager::getEquipInfoByIndex-- index>=mUserEquipInfoMap.size()");
		return info;
	}

	UserEquipInfoList::iterator itr = mUserEquipInfoMap.begin();
	while(index>0)
	{
		++itr;
		--index;
	}

	return itr->second;
}

std::string ServerDateManager::getEquipInfoByIndexForLua(int index)
{
	return getEquipInfoByIndex(index).SerializeAsString();
}

bool ServerDateManager::hasItemInfoById(int id )
{	
	if(mUserItemInfoMap.find(id) != mUserItemInfoMap.end())
		return true;
	else
	{
		return false;
	}
}

void ServerDateManager::insertItemInfoInMap(ItemInfo equip){
	if (equip.has_id())
	{
		mUserItemInfoMap.insert(std::map<int,ItemInfo>::value_type(equip.id(),equip));
	}
}


void ServerDateManager::removeItemInfoById( int id)
{
	if(mUserItemInfoMap.find(id) != mUserItemInfoMap.end())
	{
		mUserItemInfoMap.erase(mUserItemInfoMap.find(id));
	}
}

void ServerDateManager::clearItemInfo()
{
	mUserItemInfoMap.clear();
}

ItemInfo ServerDateManager::getItemInfoById( int id )
{
	ItemInfo info;
	if(mUserItemInfoMap.find(id) != mUserItemInfoMap.end())
		info = mUserItemInfoMap.find(id)->second;
	return info;
}

std::string	ServerDateManager::getItemInfoByIdForLua(int id){

	ItemInfo info = getItemInfoById(id);
	std::string ret = info.SerializeAsString();
	return ret;
}

ItemInfo ServerDateManager::getItemInfoByIndex( int index )
{
	ItemInfo info;
	if(index>=mUserItemInfoMap.size())
	{
		CCLOG("@ServerDateManager::getItemInfoByIndex-- index>=mUserItemInfoMap.size()");
		return info;
	}

	std::map<int,ItemInfo>::iterator itr = mUserItemInfoMap.begin();
	while(index>0)
	{
		++itr;
		--index;
	}

	return itr->second;
}



bool ServerDateManager::hasSkillInfoById(int id )
{	
	if(mUserSkillInfoMap.find(id) != mUserSkillInfoMap.end())
		return true;
	else
	{
		return false;
	}
}

void ServerDateManager::insertSkillInfoInMap(SkillInfo equip){
	if (equip.has_id())
	{
		mUserSkillInfoMap.insert(std::map<int,SkillInfo>::value_type(equip.id(),equip));
	}
}


void ServerDateManager::removeSkillInfoById( int id)
{
	if(mUserSkillInfoMap.find(id) != mUserSkillInfoMap.end())
	{
		mUserSkillInfoMap.erase(mUserSkillInfoMap.find(id));
	}
}

void ServerDateManager::clearSkillInfo()
{
	mUserSkillInfoMap.clear();
}

SkillInfo ServerDateManager::getSkillInfoById( int id )
{
	SkillInfo info;
	if(mUserSkillInfoMap.find(id) != mUserSkillInfoMap.end())
		info = mUserSkillInfoMap.find(id)->second;
	return info;
}

std::string	ServerDateManager::getSkillInfoByIdForLua(int id){

	SkillInfo info = getSkillInfoById(id);
	std::string ret = info.SerializeAsString();
	return ret;
}

SkillInfo ServerDateManager::getSkillInfoByIndex( int index )
{
	SkillInfo info;
	if(index>=mUserSkillInfoMap.size())
	{
		CCLOG("@ServerDateManager::getSkillInfoByIndex-- index%d>=mUserSkillInfoMap.size()%d", index, mUserSkillInfoMap.size());
		return info;
	}

	std::map<int,SkillInfo>::iterator itr = mUserSkillInfoMap.begin();
	while(index>0)
	{
		++itr;
		--index;
	}

	return itr->second;
}



bool ServerDateManager::hasRoleInfoById(int id )
{	
	if(mUserRoleInfoMap.find(id) != mUserRoleInfoMap.end())
		return true;
	else
	{
		return false;
	}
}

void ServerDateManager::insertRoleInfoInMap(RoleInfo role){
	if (role.has_roleid())
	{
		mUserRoleInfoMap.insert(std::map<int,RoleInfo>::value_type(role.roleid(),role));
	}
}


void ServerDateManager::removeRoleInfoById( int id)
{
	if(mUserRoleInfoMap.find(id) != mUserRoleInfoMap.end())
	{
		mUserRoleInfoMap.erase(mUserRoleInfoMap.find(id));
	}
}

void ServerDateManager::clearRoleInfo()
{
	mUserRoleInfoMap.clear();
}

RoleInfo ServerDateManager::getRoleInfoById( int id )
{
	RoleInfo info;
	if(mUserRoleInfoMap.find(id) != mUserRoleInfoMap.end())
		info = mUserRoleInfoMap.find(id)->second;
	return info;
}

std::string	ServerDateManager::getRoleInfoByIdForLua(int id){

	RoleInfo info = getRoleInfoById(id);
	std::string ret = info.SerializeAsString(); 
	return ret;
}

RoleInfo ServerDateManager::getRoleInfoByIndex( int index )
{
	RoleInfo info;
	if(index>=mUserRoleInfoMap.size())
	{
		CCLOG("@ServerDateManager::getRoleInfoByIndex-- index>=mUserRoleInfoMap.size()");
		return info;
	}

	std::map<int,RoleInfo>::iterator itr = mUserRoleInfoMap.begin();
	while(index>0)
	{
		++itr;
		--index;
	}

	return itr->second;
}



std::string ServerDateManager::getSkillInfoByIndexForLua( int index )
{
	 SkillInfo si = getSkillInfoByIndex(index);
	 if (si.IsInitialized())
		 return si.SerializeAsString();
	 else
		 return "";
}



