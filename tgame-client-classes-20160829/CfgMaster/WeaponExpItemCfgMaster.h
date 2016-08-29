#ifndef WeaponExpItemCfgMaster_h__
#define WeaponExpItemCfgMaster_h__

#include "CfgMasterBase.h"
#include "WeaponExpItemCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CWeaponExpItemCfgMaster : public CCfgMasterBase<WeaponExpItemCfgSet>
{
public:
	CWeaponExpItemCfgMaster(){}
	~CWeaponExpItemCfgMaster(){}

	WeaponExpItemCfg* GetWeaponExpItemCfgByID(const unsigned int uiCfgID);

	bool CheckItemIsExpItem(unsigned int uiCfgID);		//���item�Ƿ�Ϊ���������������
protected:
	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
        m_stAllExpItemID.clear();
	}
private:
	//id���±��ӳ��
	map<unsigned int, int> m_stID2IDX;

	//�������������id
	vector<unsigned int> m_stAllExpItemID;
};

#define WEAPON_EXP_CFG (CVSingleton<CWeaponExpItemCfgMaster>::Instance())

#endif // WeaponExpItemCfgMaster_h__