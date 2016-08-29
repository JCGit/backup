#ifndef OSDCfgMaster_h__
#define OSDCfgMaster_h__

#include "CfgMasterBase.h"
#include "OSDCfg.pb.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class OSDCfgMaster: public CCfgMasterBase<OSDCfgSet>
{
public:
	OSDCfgMaster() {};
	~OSDCfgMaster() {};

	OSDCfg* getOSDCfgByID(const unsigned int uSceneID);

	OSDCfg* getPrevOSDCfg(unsigned int uCurSceneID); // 根据当前场景ID获取上一个场景的配置，若为第一个场景则返回当前场景配置

	OSDCfg* getNextOSDCfg(unsigned int uCurSceneID); // 根据当前场景ID获取下一个场景的配置，若为最后一个场景则返回当前场景配置
	
protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stID2Idx.clear();
	}

	// override end

private:
	// id到下标的映射
	map<unsigned int, int> m_stID2Idx;
};

#define OSD_CFG_MASTER CVLib::CVSingleton<OSDCfgMaster>::Instance()

#endif // OSDCfgMaster_h__
