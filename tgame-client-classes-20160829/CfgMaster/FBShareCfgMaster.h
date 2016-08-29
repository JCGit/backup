#ifndef FBShareCfgMaster_H__
#define FBShareCfgMaster_H__

#include "FBShareCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class FBShareCfgMaster : public CCfgMasterBase<FBShareCfgSet>
{
public:

	FBShareCfgMaster(){}
	~FBShareCfgMaster(){}

	const FBShareCfg* GetFBShareCfg(unsigned int uShareNum);

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

#define FBShare_CFG (CVSingleton<FBShareCfgMaster>::Instance())

#endif

