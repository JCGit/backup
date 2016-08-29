#ifndef PanelSoundCfgMaster_h__
#define PanelSoundCfgMaster_h__

#include "CfgMasterBase.h"
#include "PanelSoundCfg.pb.h"
#include "vlib/base/CVSingleton.h"

#include <map>

using namespace ::com::cfg::vo;
using namespace std;
class CPanelSoundCfgMaster : public CCfgMasterBase<PanelSoundCfgSet>
{
public:
	CPanelSoundCfgMaster(){}
	~CPanelSoundCfgMaster(){}

	const PanelSoundCfg* GetPanelSoundCfgByID(const unsigned int uiCfgID);

	string getPanelOpenSound(const unsigned int uiSoundType);
	string getPanelCloseSound(const unsigned int uiSoundType);
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

#define PANEL_SOUND_CFG (CVSingleton<CPanelSoundCfgMaster>::Instance())

#endif // PanelSoundCfgMaster_h__