#ifndef StoryCfgMaster_h__
#define StoryCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"

#include "StoryCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class StoryCfgMaster: public CCfgMasterBase<StoryCfgSet>
{
public:
	StoryCfgMaster(){}
	~StoryCfgMaster(){}

	const StoryCfg* getStoryCfg(unsigned int uStoryID);
	
protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2IDX.clear(); 
	}

private:
	//id到下标的映射
	map<unsigned int, int> m_stID2IDX;

};

#define STORY_CFG_MASTER (CVSingleton<StoryCfgMaster>::Instance())

#endif // StoryCfgMaster_h__