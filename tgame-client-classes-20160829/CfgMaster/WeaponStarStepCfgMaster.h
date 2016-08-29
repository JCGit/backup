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

	//��ȡ���Ƕ�Ӧ��������
	const WeaponStarStepCfg* GetWeaponStarMaterialsByStar(const unsigned int star);

	//���ǹе���ǵĴ���
	const unsigned int GetWeaponStarCount(){
		return m_stStarSteps.size() - 1;		//���һ��ǽ�����
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
	//id���±��ӳ��
	map<pair<unsigned int, unsigned int>, int> m_stID2IDX;

	//ÿ�Ǽ���Ӧ�׼���
	map<unsigned int, unsigned int> m_stStarSteps;	

	//�Ǽ���Ӧ���±�
	map<unsigned int, unsigned int> m_stStar2IDX;
};

#define WEAPON_STAR_CFG (CVSingleton<CWeaponStarStepCfgMaster>::Instance())

#endif // WeaponStarStepCfgMaster_h__