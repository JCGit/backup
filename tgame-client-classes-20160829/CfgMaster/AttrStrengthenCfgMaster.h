#ifndef AttrStrengthenCfgMaster_h__
#define AttrStrengthenCfgMaster_h__

#include "CfgMasterBase.h"
#include "AttrStrengthenCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class AttrStrengthenCfgMaster: public CCfgMasterBase<AttrStrengthenCfgSet>
{
public:
	AttrStrengthenCfgMaster() {};
	~AttrStrengthenCfgMaster() {};

	AttrStrengthenCfg* getAttrStrengthenCfgByLevel(const unsigned int uAttrType, const unsigned int uLevel);
	
protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

	// override end

private:
	// id到下标的映射
	//map<unsigned int, int> m_stID2Idx;
	map<string, int> m_stID2Idx;
	
};

#define ATTR_STRENGTHEN_CFG_MASTER CVLib::CVSingleton<AttrStrengthenCfgMaster>::Instance()

#endif // AttrStrengthenCfgMaster_h__
