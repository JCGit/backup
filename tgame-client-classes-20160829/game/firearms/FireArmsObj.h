#ifndef FireArmsObj_h__
#define FireArmsObj_h__

#include "vlib/base/CVSingleton.h"
#include "CSWeaponMsg.pb.h"
#include "CSBagMsg.pb.h"
#include "BasicTypes.h"

using namespace std;

/*
enum {
	STATE_EQUIP = 1,
	STATE_CHOOSE = 2,
};
*/

class CFireArmsObj
{
public:
    enum 
    {
        LV_UP_MAT_MAX_SIZE = 6, //升级材料最多6个
        LV_UP_MAT_SIZE_WEAPON = 2, //枪械占2个
        LV_UP_MAT_SIZE_ITEM = 1, //其他占1个
    };
	CFireArmsObj();
	~CFireArmsObj();

	void setWeaponBag(const vmsg::CSWeaponBag& info);
	const vmsg::CSWeaponBag& getWeaponBag();

    bool isWeaponBagFull();
    int getWeaponIdx(UNIT64_t weaponGUID);

	void setMaterialBag(const vmsg::CSBagInfo& info);
	const vector<vmsg::CSBagCellInfo>& getMaterialBag();
	bool static compareMaterial(const vmsg::CSBagCellInfo& first, const vmsg::CSBagCellInfo& second);

	void setAllWeaponBag(const vmsg::CSWeaponBag& info);
	const vector<vmsg::CSWeapon>& getAllWeaponBag();
	const vector<vmsg::CSWeapon>& getChangeWeaponLst();

    const vmsg::CSWeapon& getOneWeaponByGUID(UNIT64_t uID);
    const vmsg::CSWeapon* getWeaponByGUID(UNIT64_t uID);
    int getWeaponNumByCfgID(const int iWeaponCfgID, UNIT64_t iIgnoreWeaponGuid = 0); //我背包里拥有几个该iWeaponCfgID的枪械（不算主角使用的）

	void setPreWeaponGUID(UNIT64_t guid);
	UNIT64_t getPreWeaponGUID();

	void updateWeaponBag(const vmsg::CSWeapon& info);

	//升级
	//升级选中的枪械
	void resetUpWeapon();
	void setUpWeapon(const vmsg::CSWeapon& info);
	const vmsg::CSWeapon& getUpWeapon();

	//升级选中的材料
	void resetUpMaterials();
	void addUpMaterial(UNIT64_t uID, unsigned int type, unsigned int idx);
	void removeUpMaterial(UNIT64_t uID, unsigned int idx);
    const map<UINT64_t, vmsg::CSWeaponLvUpConsume>& getUpMaterials();
    int getUpMatLogicSize(); //获取升级材料有多少个(枪械占2个，材料占1个，最多6个)
    bool canAddUpMat(const int iType); //是否有空位添加一个该类型的材料
	bool isInUpMaterial(unsigned int uIdx);		//该索引是否为选中的升级材料
    int getFullLvNeedExp(const int iWeaponCfgID, const int iWeaponLv); //升到满级时需要多少经验

	const vector<vmsg::CSWeaponLvUpConsume> getSortUpMaterials();
	const vector<vmsg::CSBagCellInfo>& getLvUpMaterials();

    //进阶
	bool checkCanStarWeapon(unsigned int uCfgID, UINT64_t uWeaponGUID = 0);

	void getWeaponsInCfg(vector<int>& stAllWeaponInCfg, const unsigned int type, const vector<vmsg::CSWeapon>& stMyWeapons);	//获取配置中去掉背包中已有的所有枪械

	void getWeaponsInBagByType(const unsigned int type, vector<vmsg::CSWeapon>& stWeapons);  //获取背包中此类型的所有枪械

    //获取2个等级的累计经验(不包括toLev, toLev > fromLev, uID用于计算该阶段经验的倍数
    unsigned int getExpByTwoLevels(unsigned int fromLev, unsigned int toLev, unsigned int uID);

    //通过当前等级、经验，以及增加的经验获取提升多少等级, uID用于计算该阶段经验的倍数
    unsigned int getLevelsByExp(const vmsg::CSWeapon& stWeapon, unsigned int addExp);

    //通过当前等级、经验，以及增加的经验获取升级后的当前等级现有的经验, uID用于计算该阶段经验的倍数
    unsigned int getUpLeftExp(const vmsg::CSWeapon& stWeapon, unsigned int addExp);

	//枪械战备
	void setWeaponInBattleInfo(const vmsg::CSWeaponInBattleRsp& stInfo);
	const vmsg::CSWeaponInBattleRsp& getWeaponInBattleInfo(){
		return m_stWeaponInBattle;
	}

	//切换副武器时的信息
	void setOldWeapon(UNIT64_t uGUID, int iArrayIdx);
	const vmsg::CSInbattleWeaponElmtBrief& getOldWeapon(){
		return m_stOldWeapon;
	}

	int getWeaponIdleNumByCfgID(unsigned int uCfgID, UNIT64_t iIgnoreWeaponGuid = 0);

protected:
    bool static compareByLev(const vmsg::CSWeapon& first, const vmsg::CSWeapon& second);
	bool static compareByLevUp(const vmsg::CSWeapon& first, const vmsg::CSWeapon& second);
    bool static compareWeaponByLv(const int cfgIDA, const int cfgIDB);	//根据枪械评级排序
    //根据枪械评级排序
    bool static compareWeaponInfoByLv(const vmsg::CSWeapon& weaponA, const vmsg::CSWeapon& weaponB);
    bool static compareUpMaterial(vmsg::CSWeaponLvUpConsume first, vmsg::CSWeaponLvUpConsume second);
private:
	//背包
    vmsg::CSWeaponBag weaponBag;	
    vector<vmsg::CSWeapon> m_stAllWeaponBag;		//所有的枪械，包括身上的

	vector<vmsg::CSWeapon> m_stChangeWeaponLst;		//可切换的枪械列表

	//换枪
	UNIT64_t m_uPreWeaponGUID;						//换枪前的guid

	//升级
	vmsg::CSWeapon m_stUpWeapon;					//当前升级枪械
    map<UINT64_t, vmsg::CSWeaponLvUpConsume> m_stUpMaterials;		//索引对应当前升级使用的材料<index, Info>
    vector<vmsg::CSBagCellInfo> m_stLvUpMaterials;	//升级材料
    int m_iLvUpMatLoginSize; //升级材料有几个了（枪械占2个，材料占1个，最多6个）
    
    map<int, set<UINT64_t> > m_stWeaponNumMap; //<WeaponCfgID, <guid list> >, 背包中guid做个map，便于查询(全部的枪械，包括布阵的枪械)

	vmsg::CSWeaponInBattleRsp m_stWeaponInBattle;

	vmsg::CSInbattleWeaponElmtBrief m_stOldWeapon;
};

#define P_ARM_OBJ CVLib::CVSingleton<CFireArmsObj>::Instance()

#endif // FireArmsObj_h__
