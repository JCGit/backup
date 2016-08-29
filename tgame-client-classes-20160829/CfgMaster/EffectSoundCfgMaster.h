#ifndef __EFFECT_SOUND_CFG_MASTER_H__
#define __EFFECT_SOUND_CFG_MASTER_H__

#include "EffectSoundCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CEffectSoundCfgMaster : public CCfgMasterBase<EffectSoundCfgSet>
{
public:

    CEffectSoundCfgMaster(){}
    ~CEffectSoundCfgMaster(){}

    EffectSoundCfg* GetSoundCfgByID(const unsigned int uiCfgID);
	EffectSoundCfg* GetSoundCfgBySkillUrl(string stSkillUrl);
protected:

    virtual int ValidateCfg()const;

    virtual int SetCfg();

    virtual void Clear() 
    {
        m_stID2IDX.clear(); 
		m_stName2IDX.clear();
    }
private:
    //id到下标的映射
    map<unsigned int, int> m_stID2IDX;
	map<string, int> m_stName2IDX;
};

#define EFFECT_SOUND_CFG_MASTER (CVSingleton<CEffectSoundCfgMaster>::Instance())

#endif

