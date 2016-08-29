#ifndef WeaponCfgMaster_h__
#define WeaponCfgMaster_h__

#include "CfgMasterBase.h"
#include "WeaponCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class  CWeaponCfgMaster : public CCfgMasterBase<WeaponCfgSet>
{
public:
	CWeaponCfgMaster(){}
	~CWeaponCfgMaster(){}

    const WeaponCfg* GetWeaponCfgByID(const unsigned int uiCfgID);
    WeaponCfg* GetMutableWeaponCfgByID(const unsigned int uiCfgID);

	int GetWeaponCfgByStarAndStep(const unsigned int uWeaponID, 
								  const unsigned int uStar, 
								  const unsigned int uStep,
								  WeaponCfg& stCfg);

	//枪械攻击秒伤加成
	int GetWeaponAtkAdd(const unsigned int uiCfgID, unsigned int color, unsigned int level);
	//枪械攻击属性加成
	int GetWeaponAtkAttrAdd(const unsigned int uiCfgID, unsigned int level, unsigned int color);

	const string getAtkAttrTypeDesc(const unsigned int atkAttrType);

	const string getWeaponColorDesc(const unsigned int color);

	vector<int> getWeaponsByType(const unsigned int type);

    string getName(const unsigned int uiCfgID);
    string getIcon(const unsigned int uiCfgID);
    string getWeaponDesc(const unsigned int uiCfgID);
    int getWeaponLv(const unsigned int uiCfgID); //枪械评级

	//随意一把星级、阶级相同的枪
	unsigned int getWeaponRandByStarAndStep(unsigned int uStar, unsigned int uStep, unsigned int uWeaponID);
protected:
	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
        m_stKEYTOIDX.clear();
        m_stTypeToIDX.clear();
	}
private:
	unsigned int MakeKey(const unsigned int uWeaponID,
		const unsigned int uStar,
		const unsigned int uStep)const;
private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;

	map<unsigned int , int> m_stKEYTOIDX;

	//类型对应数组
	map<int, vector<int> > m_stTypeToIDX;
};

#define WEAPON_CFG_MASTER (CVSingleton<CWeaponCfgMaster>::Instance())

#endif // WeaponCfgMaster_h__
