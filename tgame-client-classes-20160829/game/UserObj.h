#ifndef UserObj_h__
#define UserObj_h__

#include "UserInfo.h"
#include "BasicTypes.h"


class UserObj
{
public:
	UserObj();
	~UserObj();

    //保护数据，加个const限定一下吧,需要修改数据的用getMutableUserInfo
    const UserInfo *getUserInfo() { 
        return &m_stUserInfo; 
    }
    UserInfo *getMutableUserInfo(){ 
        return &m_stUserInfo; 
    }
    void resetOnConnClose() {
        //断线时重置数据
        CCLOG("[trace]UserObj,reset data on connection close.");
        int iPreZoneID = m_stUserInfo.zoneid;
        std::string strPreZoneName = m_stUserInfo.zoneName;
        UserInfo stUserInfo;
        m_stUserInfo = stUserInfo;
        m_stUserInfo.zoneid = iPreZoneID;
        m_stUserInfo.zoneName = strPreZoneName;
    }
	void setLoginPrepare(bool var);
	bool getLoginPrepare();

    void setUin(const UIN_t uUin);
	void updateUserInfo(const vmsg::CSPlayerInfo &playerInfo);
    void updateGold(const unsigned int iGold, const unsigned int uTotalGoldByMoney);
    void updateCoin(const UINT64_t iCoin);
    void updateLvExp(const int lv, const int exp);

	void updateVIPLv(const unsigned int uVIPLv);

	void updateFightPower(const UINT64_t fightPower);

    bool isUserEquip(const UINT64_t ulEquipGuid);  //ulEquipGuid是否是玩家的equip

    int checkGold(const unsigned int iNeedGold); //检查gold是否够,返回0表示OK
    int checkCoin(const UINT64_t iNeedCoin); //检查coin是否够,返回0表示OK

	void updateUserDetailInfo(const vmsg::CSPlayerDetailInfo& detailInfo);	//更新玩家具体信息

    unsigned int GetZoneID(uint32_t uiUin) const {
        return (uiUin >> 21) & 0x7FF;
    }

    unsigned int GetMyZoneID() const {
        return GetZoneID(m_stUserInfo.uin);
    }

    std::string getMyZoneNick(); //获取我的大区昵称:XXX.s1

    std::string genZoneNick(const std::string& strNick);//生成一个的大区昵称:XXX.s1
            
	void updateManual(const vmsg::CSPlayerManualNotify &stManualNotify); // 单独更新体力

	void updateWeapon(const vmsg::CSWeapon& stWeapon);	//更换枪械
	void updateWeaponInfo(const vmsg::CSWeapon& stWeapon);	//更新枪械信息

	int checkManual(const int iNeedManual);  //检查体力是否够,返回0表示OK

	void updateNewestWarMap(unsigned int warMapID);		//更新最新关卡id

	void updateNihilityInfo(const vmsg::CSNothinglandInfo &stInfo);		//更新虚无之地的信息

    const UINT64_t getQuickLoginKey() {
        return m_uQuickLoginKey;
    }
    void setQuickLoginKey(const UINT64_t uNewKey);

    void updateDailyRegInfo(const vmsg::CSDailyRegInfo& stInfo);

	void updateMonthCardInfo(const vmsg::CSMonthCardInfo &stMonthCardInfo);

	//更新玩家身上武器阵位加成信息
	void updateAddEff(unsigned int hpAdd, unsigned int atkAdd, unsigned int defAdd);

	//更新玩家身上武器阵位信息
	void updateWeaponArr(const vmsg::CSWeapon& stEquipWeapon, google::protobuf::RepeatedPtrField<vmsg::CSInbattleWeaponElmt> stWeaponArr);

	//更新玩家的基因加成信息
	void updateGenInfo(unsigned int totalAtkAdd, unsigned int totalDefAdd, unsigned int totalHpAdd);
private:
	UserInfo m_stUserInfo;
    UINT64_t m_uQuickLoginKey; //快速登陆key

	bool m_bLoginPrepare;
};

#define P_USER_OBJ CVLib::CVSingleton<UserObj>::Instance()

#endif // UserObj_h__
