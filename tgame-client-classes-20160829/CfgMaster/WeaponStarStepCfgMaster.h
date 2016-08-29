#ifndef WeaponStarStepCfgMaster_h__
#define WeaponStarStepCfgMaster_h__

#include "CfgMasterBase.h"
#include "WeaponStarStepCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CWeaponStarStepCfgMaster : public CCfgMasterBase<WeaponStarStepCfgSet>
{
public:
	CWeaponStarStepCfgMaster(){}
	~CWeaponStarStepCfgMaster(){}

	WeaponStarStepCfg* GetWeaponStarStepCfgByStarAndStep(const unsigned int star, const unsigned int step);

	//获取升星对应的配置项
	const WeaponStarStepCfg* GetWeaponStarMaterialsByStar(const unsigned int star);

	//获得枪械升星的次数
	const unsigned int GetWeaponStarCount(){
		return m_stStarSteps.size() - 1;		//最后一项不是进阶项
	}
protected:
	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
        m_stStarSteps.clear();
        m_stStar2IDX.clear();
	}
private:
	//id到下标的映射
	map<pair<unsigned int, unsigned int>, int> m_stID2IDX;

	//每星级对应阶级数
	map<unsigned int, unsigned int> m_stStarSteps;	

	//星级对应的下标
	map<unsigned int, unsigned int> m_stStar2IDX;
};

#define WEAPON_STAR_CFG (CVSingleton<CWeaponStarStepCfgMaster>::Instance())

#endif // WeaponStarStepCfgMaster_h__