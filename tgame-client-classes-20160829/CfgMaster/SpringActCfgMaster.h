#ifndef SpringActCfgMaster_h__
#define SpringActCfgMaster_h__

#include "CfgMasterBase.h"
#include "SpringActCfg.pb.h"

#include <map>

using namespace com::cfg::vo;
using namespace std;

class SpringActCfgMaster: public CCfgMasterBase<SpringActCfgSet>
{
public:
	SpringActCfgMaster() {};
	virtual ~SpringActCfgMaster() {};

	const SpringActCfg* getSpringActCfg(const unsigned int uDay);
	
protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;

};

#define SPRING_ACT_CFG_MASTER CVLib::CVSingleton<SpringActCfgMaster>::Instance()

#endif // SpringActCfgMaster_h__