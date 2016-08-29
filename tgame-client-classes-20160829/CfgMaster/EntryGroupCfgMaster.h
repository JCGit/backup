#ifndef EntryGroupCfgMaster_h__
#define EntryGroupCfgMaster_h__

#include "CfgMasterBase.h"
#include "EntryGroupCfg.pb.h"

#include <map>

using namespace com::cfg::vo;
using namespace std;

class EntryGroupCfgMaster: public CCfgMasterBase<EntryGroupCfgSet>
{
public:
	EntryGroupCfgMaster() {};
	virtual ~EntryGroupCfgMaster() {};

	const EntryGroupCfg* getEntryGroupCfg(const unsigned int uEntryGroupID);
	
	unsigned int getEntryGroupID(unsigned int uEntryID);

protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
		m_stEntry2Group.clear();
	}

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;

	map<unsigned int, unsigned int> m_stEntry2Group; // <EntryID, EntryGroupID>

};

#define ENTRY_GROUP_CFG_MASTER CVLib::CVSingleton<EntryGroupCfgMaster>::Instance()

#endif // EntryGroupCfgMaster_h__