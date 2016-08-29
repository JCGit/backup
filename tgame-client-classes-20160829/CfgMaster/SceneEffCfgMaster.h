#ifndef SceneEffCfgMaster_h__
#define SceneEffCfgMaster_h__

#include "CfgMasterBase.h"
#include "SceneEffCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
class CSceneEffCfgMaster : public CCfgMasterBase<SceneEffCfgSet>
{
public:
	CSceneEffCfgMaster(){}
	~CSceneEffCfgMaster(){}

	const SceneEffCfg* GetSceneEffCfgByID(const unsigned int uiCfgID);
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

#define SCENE_EFFECT_CFG (CVSingleton<CSceneEffCfgMaster>::Instance())

#endif // SceneEffCfgMaster_h__
