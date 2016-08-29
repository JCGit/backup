#ifndef ExpCfgMaster_h__
#define ExpCfgMaster_h__

#include "CfgMasterBase.h"
#include "ExpCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class ExpCfgMaster: public CCfgMasterBase<ExpCfgSet>
{
public:
	ExpCfgMaster() {};
	~ExpCfgMaster() {};

	ExpCfg* getExpCfgByID(const unsigned int uCfgID);

	unsigned int getNextTotalExp(unsigned int uCurLevel);
	
protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
		m_stExpNeedMap.clear();
	}

	// override end

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;
	map<unsigned int, unsigned int> m_stExpNeedMap; // <CurLevel, TotalExp>
};

#define EXP_CFG_MASTER CVLib::CVSingleton<ExpCfgMaster>::Instance()

#endif // ExpCfgMaster_h__
