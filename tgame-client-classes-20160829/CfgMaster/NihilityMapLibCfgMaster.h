#ifndef __NihilityMapLibCfg_H__
#define __NihilityMapLibCfg_H__

#include "NihilityMapLibCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CNihilityMapLibCfgMaster : public CCfgMasterBase<NihilityMapLibCfgSet>
{
public:

	CNihilityMapLibCfgMaster(){}
	~CNihilityMapLibCfgMaster(){}

	const NihilityMapLibCfg* GetNihilityMapLibCfgByID(const unsigned int uiCfgID);

	const NihilityMapLibCfg* GetNihilityMapRandByType(const unsigned int uType, const unsigned int floor);

	const NihilityMapLibCfg* GetNihilityMapRandByTypeAndRes(const unsigned int uType, const unsigned int floor, const string stRes);

protected:

	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
        m_stTypeToIDX.clear();
	}
private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;

	//类型对应下标集合
	map<unsigned int, vector<int> > m_stTypeToIDX;
};

#define NIHILITY_MAP_LIB_CFG (CVSingleton<CNihilityMapLibCfgMaster>::Instance())

#endif

