#ifndef MonsterShowCfgMaster_h__
#define MonsterShowCfgMaster_h__

#include "CfgMasterBase.h"
#include "MonsterShowCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CMonsterShowCfgMaster : public CCfgMasterBase<MonsterShowCfgSet>
{
public:
	CMonsterShowCfgMaster(){}
	~CMonsterShowCfgMaster(){}

	MonsterShowCfg* GetMonsterShowCfgByShowID(const std::string showID);

	//������������õ�����
	MonsterShowCfg* GetMonsterShowCfgRand(const unsigned int type);
protected:
	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
        m_stTypeToIDX.clear();
	}
private:
	//id���±��ӳ��
	map<std::string, int> m_stID2IDX;

	//���Ͷ�Ӧ����
	map<int, vector<int> > m_stTypeToIDX;
};

#define MONSTER_SHOW_CFG (CVSingleton<CMonsterShowCfgMaster>::Instance())

#endif // MonsterShowCfgMaster_h__