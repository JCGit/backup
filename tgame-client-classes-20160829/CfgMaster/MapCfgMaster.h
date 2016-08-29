#ifndef MapCfgMaster_h__
#define MapCfgMaster_h__

#include "CfgMasterBase.h"
#include "MapCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class  CMapCfgMaster : public CCfgMasterBase<MapCfgSet>
{
public:
	CMapCfgMaster(){}
	~CMapCfgMaster(){}

	MapCfg* GetMapCfgByID(const unsigned int uiCfgID);
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

#define MAP_CFG_MASTER (CVSingleton<CMapCfgMaster>::Instance())

#endif // MapCfgMaster_h__
