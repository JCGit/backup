#ifndef EntryCfgMaster_h__
#define EntryCfgMaster_h__

#include "CfgMasterBase.h"
#include "EntryCfg.pb.h"

#include <map>

using namespace com::cfg::vo;
using namespace std;

class EntryCfgMaster: public CCfgMasterBase<EntryCfgSet>
{
public:
	EntryCfgMaster() {};
	virtual ~EntryCfgMaster() {};

	const EntryCfg* getEntryCfg(const unsigned int uEntryID);
	
	unsigned int getParentEntryID(unsigned int uEntryID);

	unsigned int getEntryIDByModuleID(unsigned int uModuleID);

	pair<unsigned int, vector<unsigned int> > getLevelEntrys(unsigned int uLevel);

	unsigned int getChildEntryIDSize(unsigned int uEntryID);

protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
		m_stChild2Parent.clear();
		m_stModule2Entry.clear();
		m_stLevel2Entrys.clear();
	}

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;

	map<unsigned int, unsigned int> m_stChild2Parent; // <ChildEntryID, ParentEntryID>

	map<unsigned int, unsigned int> m_stModule2Entry; // <ModuleID, EntryID>

	map<unsigned int, vector<unsigned int> > m_stLevel2Entrys; // <Level, [EntryID]>

};

#define ENTRY_CFG_MASTER CVLib::CVSingleton<EntryCfgMaster>::Instance()

#endif // EntryCfgMaster_h__