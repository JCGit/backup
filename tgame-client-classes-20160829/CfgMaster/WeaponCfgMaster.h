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

	//ǹе�������˼ӳ�
	int GetWeaponAtkAdd(const unsigned int uiCfgID, unsigned int color, unsigned int level);
	//ǹе�������Լӳ�
	int GetWeaponAtkAttrAdd(const unsigned int uiCfgID, unsigned int level, unsigned int color);

	const string getAtkAttrTypeDesc(const unsigned int atkAttrType);

	const string getWeaponColorDesc(const unsigned int color);

	vector<int> getWeaponsByType(const unsigned int type);

    string getName(const unsigned int uiCfgID);
    string getIcon(const unsigned int uiCfgID);
    string getWeaponDesc(const unsigned int uiCfgID);
    int getWeaponLv(const unsigned int uiCfgID); //ǹе����

	//����һ���Ǽ����׼���ͬ��ǹ
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
	//id���±��ӳ��
	map<unsigned int, int> m_stID2IDX;

	map<unsigned int , int> m_stKEYTOIDX;

	//���Ͷ�Ӧ����
	map<int, vector<int> > m_stTypeToIDX;
};

#define WEAPON_CFG_MASTER (CVSingleton<CWeaponCfgMaster>::Instance())

#endif // WeaponCfgMaster_h__
