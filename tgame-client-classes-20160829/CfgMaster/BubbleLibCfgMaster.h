#ifndef BubbleLibCfgMaster_h__
#define BubbleLibCfgMaster_h__

#include "CfgMasterBase.h"
#include "BubbleLibCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CBubbleLibCfgMaster : public CCfgMasterBase<BubbleLibCfgSet>
{
public:
	CBubbleLibCfgMaster(){}
	~CBubbleLibCfgMaster(){}

	const BubbleLibCfg* GetBubbleLibCfgByID(const unsigned int uiCfgID);
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

#define BUBBLE_LIB_CFG (CVSingleton<CBubbleLibCfgMaster>::Instance())

#endif // BubbleLibCfgMaster_h__