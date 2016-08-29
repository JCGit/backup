#ifndef QiangHunCfgMaster_h__
#define QiangHunCfgMaster_h__

#include "CfgMasterBase.h"
#include "QiangHunCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CQiangHunCfgMaster : public CCfgMasterBase<QiangHunCfgSet>
{
public:
	CQiangHunCfgMaster(){}
	~CQiangHunCfgMaster(){}

	const QiangHunCfg* GetCfgByLayerAndLev(const int layer, const unsigned int lev);

	unsigned int GetMaxLayer(){
		return m_stID2IDX.size();
	}

protected:
	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
private:
	//每层的所有等级段对应的下标
	map< unsigned int, set<int> > m_stID2IDX;
};

#define QIANGHUN_CFG_MASTER (CVSingleton<CQiangHunCfgMaster>::Instance())

#endif // QiangHunCfgMaster_h__