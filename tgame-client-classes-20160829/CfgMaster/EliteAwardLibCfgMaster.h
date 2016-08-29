#ifndef EliteAwardLibCfgMaster_h__
#define EliteAwardLibCfgMaster_h__

#include "CfgMasterBase.h"
#include "EliteAwardLibCfg.pb.h"

#include <map>

using namespace com::cfg::vo;
using namespace std;

class EliteAwardLibCfgMaster: public CCfgMasterBase<EliteAwardLibCfgSet>
{
public:
	EliteAwardLibCfgMaster() {};
	virtual ~EliteAwardLibCfgMaster() {};

	const EliteAwardLibCfg* getEliteAwardLibCfg(const unsigned int uID);
	const vector<EliteAwardLibCfg*>& getChapterAwards(const unsigned int uEliteWarChapterID);
	
protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
        m_stChapterAwards.clear();
	}

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;
	map<unsigned int, vector<EliteAwardLibCfg*> > m_stChapterAwards;

};

#define ELITE_AWARD_LIB_CFG_MASTER CVLib::CVSingleton<EliteAwardLibCfgMaster>::Instance()

#endif // EliteAwardLibCfgMaster_h__