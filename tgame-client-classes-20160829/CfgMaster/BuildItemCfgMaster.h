#ifndef BuildItemCfgMaster_h__
#define BuildItemCfgMaster_h__

#include "CfgMasterBase.h"
#include "BuildItemCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CBuildItemCfgMaster : public CCfgMasterBase<BuildItemCfgSet>
{
public:
	CBuildItemCfgMaster(){}
	~CBuildItemCfgMaster(){}

	BuildItemCfg* GetBuildItemCfgByID(const unsigned int uiCfgID);
protected:
	virtual int ValidateCfg()const;

	virtual int SetCfg();

	virtual void Clear() 
	{
		m_stID2IDX.clear(); 
	}
private:
	//id���±��ӳ��
	map<unsigned int, int> m_stID2IDX;
};

#define BUILD_ITEM_CFG (CVSingleton<CBuildItemCfgMaster>::Instance())

#endif // BuildItemCfgMaster_h__
