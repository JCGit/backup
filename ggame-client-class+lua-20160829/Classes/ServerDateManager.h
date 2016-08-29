#pragma once
#include "Singleton.h"
#include "IteratorWrapper.h"
#include "DataTableManager.h"
#include "Language.h"
#include "ResManager.h"
#include "json/json.h"
#include "GamePackets.h"
#include "CCBContainer.h"
#include "TimeCalculator.h"
#include <string>
#include <map>
#include <list>
#include <set>


class	LoginInfo{
public:
	//服务器ID
	int m_iPlayerID;
	//所选职业ID
	int m_iRoleItemID;
	//时间戳
	int m_iTimeStamp;

	LoginInfo(){
		m_iPlayerID=0;
		m_iRoleItemID =0;
		m_iTimeStamp =0;
	};

	void reset()
	{
		m_iPlayerID=0;
		m_iRoleItemID =0;
		m_iTimeStamp =0;
	}
};




class GameCheckInfo
{
public:
	unsigned int mCurrAdventureVersion;
	unsigned int mHeartRecruitDiscipleVersion;
	unsigned int mHeartAdventureVersion;
	bool mNeedForceSyncAdventure;
	bool isNeedSyncAdventureList(){return mNeedForceSyncAdventure || mHeartAdventureVersion>mCurrAdventureVersion;};
	void setNeedForceSyncAdventure(bool need) { mNeedForceSyncAdventure = need; };
	GameCheckInfo(){mCurrAdventureVersion = 0;};
};

typedef std::map<int64_t, EquipInfo > UserEquipInfoList;
typedef ConstMapIterator<UserEquipInfoList> UserEquipInfoListIterator;
class ServerDateManager : public Singleton<ServerDateManager>
{
public:
	ServerDateManager();
	static ServerDateManager* getInstance();

	//add by zhengxin at 2014-08-10 for从MainFrame退回LoadingFrame再重入MainFrame
	void reset(bool changedPlayer);

	//登录数据
	LoginInfo& getLoginInfo() {return mLoginInfo;};
	LoginInfo mLoginInfo;

	//用户基本数据
	PlayerInfo& getUserBasicInfo() {return mUserPlayerInfo;};
	std::string getUserBasicInfoForLua() {return mUserPlayerInfo.SerializeAsString();};
	PlayerInfo mUserPlayerInfo;

	//主角卡数据
	RoleInfo& getUserRoleInfo() {return mUserRoleInfo;};
	std::string getUserRoleInfoForLua() {return mUserRoleInfo.IsInitialized() ? mUserRoleInfo.SerializeAsString() : "";};
	RoleInfo mUserRoleInfo;


	//佣兵数据
	std::map<int,RoleInfo> mUserRoleInfoMap;
	bool hasRoleInfoById(int id );
	void insertRoleInfoInMap(RoleInfo equip);
	void removeRoleInfoById(int id);
	void clearRoleInfo();
	int getRoleInfoTotalSize(){return mUserRoleInfoMap.size();};
	RoleInfo	getRoleInfoById(int id);
	RoleInfo	getRoleInfoByIndex(int id);
	std::string	getRoleInfoByIdForLua(int id);
	std::string	getRoleInfoByIndexForLua(int index){return getRoleInfoByIndex(index).SerializeAsString();};

	//装备数据
	bool hasEquipInfoById(int64_t id );
	void insertEquipInfoInMap(EquipInfo equip);
	void removeEquipInfoById(int64_t id);
	void clearEquipInfo();
	int getEquipInfoTotalSize(){return mUserEquipInfoMap.size();};
	EquipInfo	getEquipInfoByIndex(int id);
	EquipInfo	getEquipInfoById(int64_t id);
	std::string	getEquipInfoByIdForLua(int64_t id);
	std::string	getEquipInfoByIndexForLua(int index);
	UserEquipInfoList mUserEquipInfoMap;

	//道具数据
	std::map<int,ItemInfo> mUserItemInfoMap;
	bool hasItemInfoById(int id );
	void insertItemInfoInMap(ItemInfo equip);
	void removeItemInfoById(int id);
	void clearItemInfo();
	int getItemInfoTotalSize(){return mUserItemInfoMap.size();};
	ItemInfo	getItemInfoById(int id);
	ItemInfo	getItemInfoByIndex(int id);
	std::string	getItemInfoByIdForLua(int id);
	std::string	getItemInfoByIndexForLua(int index){return getItemInfoByIndex(index).SerializeAsString();};


	//技能数据
	std::map<int,SkillInfo> mUserSkillInfoMap;
	bool hasSkillInfoById(int id );
	void insertSkillInfoInMap(SkillInfo equip);
	void removeSkillInfoById(int id);
	void clearSkillInfo();
	int getSkillInfoTotalSize(){return mUserSkillInfoMap.size();};
	SkillInfo	getSkillInfoById(int id);
	SkillInfo	getSkillInfoByIndex(int id);
	std::string	getSkillInfoByIdForLua(int id);
	std::string	getSkillInfoByIndexForLua(int index);

	HPPlayerStateSync mPlayerState;
	std::string getPlayerStateForLua() { return mPlayerState.SerializeAsString(); };
	

	//服务器静默停服，用于短暂停服，不显示提示文字，不转菊花，add by zhenhui 2014/9/2
	bool m_bSerQuietRestart;
	bool getServerWillRestart();
	void setServerWillRestart(bool flag){m_bSerQuietRestart = flag;};
	
};