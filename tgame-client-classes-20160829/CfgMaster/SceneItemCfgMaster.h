#ifndef SceneItemCfgMaster_h__
#define SceneItemCfgMaster_h__

#include "CfgMasterBase.h"
#include "SceneItemCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CSceneItemCfgMaster : public CCfgMasterBase<SceneItemCfgSet>
{
public:
	CSceneItemCfgMaster(){}
	~CSceneItemCfgMaster(){}

	SceneItemCfg* GetSceneItemCfgByID(const unsigned int uiCfgID);
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

#define SCENE_ITEM_CFG (CVSingleton<CSceneItemCfgMaster>::Instance())

#endif // SceneItemCfgMaster_h__
