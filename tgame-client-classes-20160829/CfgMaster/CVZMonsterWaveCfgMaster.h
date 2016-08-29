#ifndef CVZMonsterWaveCfgMaster_h__
#define CVZMonsterWaveCfgMaster_h__

#include "CfgMasterBase.h"
#include "CVZMonsterWaveCfg.pb.h"
#include <vector>
#include <string>
#include <map>
using namespace std;
using namespace com::cfg::vo;


class CVZMonsterWaveCfgMaster: public CCfgMasterBase<CVZMonsterWaveCfgSet>
{
public:
	CVZMonsterWaveCfgMaster() {}
	~CVZMonsterWaveCfgMaster() {}

    const string* GetAvatar(const int iWave);
    int getRandSpeed(const int iWave);

	const CVZMonsterWaveCfg* GetCfgByWave(const int iWave);
protected:
	// override start

	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
		m_stWaveToIdx.clear();
	}
	// override end

private:
    map<int, int> m_stWaveToIdx; //<wave, index>
};

#define CVZ_MONSTER_WAVE_CFG_MASTER CVLib::CVSingleton<CVZMonsterWaveCfgMaster>::Instance()

#endif // CVZMonsterWaveCfgMaster_h__
