#ifndef EliteWarMapCfgMaster_h__
#define EliteWarMapCfgMaster_h__

#include "CfgMasterBase.h"
#include "EliteWarMapCfg.pb.h"

#include <map>

using namespace com::cfg::vo;
using namespace std;

class EliteWarMapCfgMaster: public CCfgMasterBase<EliteWarMapCfgSet>
{
public:
	EliteWarMapCfgMaster() {};
	virtual ~EliteWarMapCfgMaster() {};

	const EliteWarMapCfg* getEliteWarMapCfg(const unsigned int uID);
	const vector<EliteWarMapCfg*>& getEliteWarChapter(const unsigned int uChapterID);
	
protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
        m_stEliteWarChapters.clear();
	}

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;
	map<unsigned int, vector<EliteWarMapCfg*> > m_stEliteWarChapters;

};

#define ELITE_WAR_MAP_CFG_MASTER CVLib::CVSingleton<EliteWarMapCfgMaster>::Instance()

#endif // EliteWarMapCfgMaster_h__