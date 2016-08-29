#ifndef VipGiftCfgMaster_h__
#define VipGiftCfgMaster_h__

#include "vlib/base/CVSingleton.h"

#include "CfgMaster/CfgMasterBase.h"
#include "VipGiftCfg.pb.h"

#include <map>

using namespace ::com::cfg::vo;
class VipGiftCfgMaster : public CCfgMasterBase<VipGiftCfgSet>
{
public:
	VipGiftCfgMaster(){}
	~VipGiftCfgMaster(){}

	const VipGiftCfg* getVipGiftActCfg(unsigned int uLevel);
	unsigned int	getVipGiftTypeNum();


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

#define VIPGIFT_CFG_MASTER (CVSingleton<VipGiftCfgMaster>::Instance())

#endif // VipGiftCfgMaster_h__