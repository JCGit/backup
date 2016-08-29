#include "CVZMonsterWaveCfgMaster.h"
#include "vlib/CVRand.h"
#include <set>
using namespace CVLib;
using std::set;



int CVZMonsterWaveCfgMaster::ValidateCfg() const {
	int iRet = 0;

	set<unsigned int> stCfgIDSet;

	for(int iIdx = 0; iIdx < m_stCfg.monsterwavecfg_size(); iIdx++)
	{
		unsigned int uCfgID = m_stCfg.monsterwavecfg(iIdx).wave();

		if(stCfgIDSet.insert(uCfgID).second == false)
		{
			CV_ERROR(("%s, wave = %u is dup cfg id !", __PRETTY_FUNCTION__, uCfgID));

			return -1; // id重复
		}

		//to add other validate check, such as skill etc.
	}
	return iRet;
}

int CVZMonsterWaveCfgMaster::SetCfg() {
    for(int iIdx = 0; iIdx < m_stCfg.monsterwavecfg_size(); iIdx++)
    {
        unsigned int wave = m_stCfg.monsterwavecfg(iIdx).wave();
        if(m_stWaveToIdx.count(wave) <= 0) {
            m_stWaveToIdx[wave] = iIdx;
        }

    }


	return 0;
}


const string* CVZMonsterWaveCfgMaster::GetAvatar(const int iWave) {
    //策划配置从1开始，后台从0开始
    const int wave = iWave + 1;
    if(m_stWaveToIdx.count(wave) <= 0) {
        return NULL;
    }
    const int iIdx = m_stWaveToIdx[wave];

    return &(m_stCfg.monsterwavecfg(iIdx).monsteravatar());
}


int CVZMonsterWaveCfgMaster::getRandSpeed(const int iWave) {
    const int wave = iWave + 1;
    if(m_stWaveToIdx.count(wave) <= 0) {
        return 80;
    }
    const int iIdx = m_stWaveToIdx[wave];
    const int iMinSpeed = m_stCfg.monsterwavecfg(iIdx).minspeed();
    const int iMaxSpeed = m_stCfg.monsterwavecfg(iIdx).maxspeed();
    if(iMaxSpeed <= iMinSpeed) {
        return iMinSpeed;
    }
    int iSpeed = CRandom::GetRandHaveMax(iMaxSpeed - iMinSpeed);


    return iMinSpeed + iSpeed;
}

const CVZMonsterWaveCfg* CVZMonsterWaveCfgMaster::GetCfgByWave(const int iWave){
    const int wave = iWave + 1;
	map<int, int>::const_iterator it = m_stWaveToIdx.find(wave);

	if(it == m_stWaveToIdx.end())
	{
		CV_ERROR(("%s, can not find wave = %u", __PRETTY_FUNCTION__, wave));
		return NULL; //找不到该物品配置
	}

	return &(m_stCfg.monsterwavecfg(it->second));
}