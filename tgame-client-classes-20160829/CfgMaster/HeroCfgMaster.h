#ifndef HeroCfgMaster_h__
#define HeroCfgMaster_h__

#include "CfgMasterBase.h"
#include "HeroCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CHeroCfgMaster : public CCfgMasterBase<HeroCfgSet>
{
public:
	CHeroCfgMaster(){}
	~CHeroCfgMaster(){}

	const HeroCfg* GetHeroCfgByID(const unsigned int uiCfgID);
    const string* GetResName(const unsigned int uiCfgID);
protected:
	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;
};

#define HERO_CFG_MASTER (CVSingleton<CHeroCfgMaster>::Instance())

#endif // HeroCfgMaster_h__