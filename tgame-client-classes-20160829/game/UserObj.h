#ifndef UserObj_h__
#define UserObj_h__

#include "UserInfo.h"
#include "BasicTypes.h"


class UserObj
{
public:
	UserObj();
	~UserObj();

    //�������ݣ��Ӹ�const�޶�һ�°�,��Ҫ�޸����ݵ���getMutableUserInfo
    const UserInfo *getUserInfo() { 
        return &m_stUserInfo; 
    }
    UserInfo *getMutableUserInfo(){ 
        return &m_stUserInfo; 
    }
    void resetOnConnClose() {
        //����ʱ��������
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

    bool isUserEquip(const UINT64_t ulEquipGuid);  //ulEquipGuid�Ƿ�����ҵ�equip

    int checkGold(const unsigned int iNeedGold); //���gold�Ƿ�,����0��ʾOK
    int checkCoin(const UINT64_t iNeedCoin); //���coin�Ƿ�,����0��ʾOK

	void updateUserDetailInfo(const vmsg::CSPlayerDetailInfo& detailInfo);	//������Ҿ�����Ϣ

    unsigned int GetZoneID(uint32_t uiUin) const {
        return (uiUin >> 21) & 0x7FF;
    }

    unsigned int GetMyZoneID() const {
        return GetZoneID(m_stUserInfo.uin);
    }

    std::string getMyZoneNick(); //��ȡ�ҵĴ����ǳ�:XXX.s1

    std::string genZoneNick(const std::string& strNick);//����һ���Ĵ����ǳ�:XXX.s1
            
	void updateManual(const vmsg::CSPlayerManualNotify &stManualNotify); // ������������

	void updateWeapon(const vmsg::CSWeapon& stWeapon);	//����ǹе
	void updateWeaponInfo(const vmsg::CSWeapon& stWeapon);	//����ǹе��Ϣ

	int checkManual(const int iNeedManual);  //��������Ƿ�,����0��ʾOK

	void updateNewestWarMap(unsigned int warMapID);		//�������¹ؿ�id

	void updateNihilityInfo(const vmsg::CSNothinglandInfo &stInfo);		//��������֮�ص���Ϣ

    const UINT64_t getQuickLoginKey() {
        return m_uQuickLoginKey;
    }
    void setQuickLoginKey(const UINT64_t uNewKey);

    void updateDailyRegInfo(const vmsg::CSDailyRegInfo& stInfo);

	void updateMonthCardInfo(const vmsg::CSMonthCardInfo &stMonthCardInfo);

	//�����������������λ�ӳ���Ϣ
	void updateAddEff(unsigned int hpAdd, unsigned int atkAdd, unsigned int defAdd);

	//�����������������λ��Ϣ
	void updateWeaponArr(const vmsg::CSWeapon& stEquipWeapon, google::protobuf::RepeatedPtrField<vmsg::CSInbattleWeaponElmt> stWeaponArr);

	//������ҵĻ���ӳ���Ϣ
	void updateGenInfo(unsigned int totalAtkAdd, unsigned int totalDefAdd, unsigned int totalHpAdd);
private:
	UserInfo m_stUserInfo;
    UINT64_t m_uQuickLoginKey; //���ٵ�½key

	bool m_bLoginPrepare;
};

#define P_USER_OBJ CVLib::CVSingleton<UserObj>::Instance()

#endif // UserObj_h__
