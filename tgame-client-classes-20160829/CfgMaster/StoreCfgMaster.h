#ifndef StoreCfgMaster_h__
#define StoreCfgMaster_h__

#include "CfgMasterBase.h"
#include "StoreCfg.pb.h"
#include <map>
#include <vector>

using namespace com::cfg::vo;
using namespace std;

class StoreCfgMaster: public CCfgMasterBase<StoreCfgSet>
{
public:
	StoreCfgMaster() {};
	~StoreCfgMaster() {};

	StoreCfg* getStoreCfg(unsigned int uType, unsigned int uCfgID);
	
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

#define STORE_CFG_MASTER CVLib::CVSingleton<StoreCfgMaster>::Instance()

#endif // StoreCfgMaster_h__
