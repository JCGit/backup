#ifndef __FLOOR_BUFF_CFG_MASTER_H__
#define __FLOOR_BUFF_CFG_MASTER_H__

#include "FloorBuffCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CFloorBuffCfgMaster : public CCfgMasterBase<FloorBuffCfgSet>
{
public:

	CFloorBuffCfgMaster(){}
	~CFloorBuffCfgMaster(){}

	const FloorBuffCfg* GetFloorBuffCfgByID(const unsigned int uiCfgID);

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

#define FLOOR_BUFF_CFG (CVSingleton<CFloorBuffCfgMaster>::Instance())

#endif

