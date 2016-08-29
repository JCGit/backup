#include "WeaponLotteryCfgMaster.h"
#include "CSPlayerMsg.pb.h"
#include <set>
using namespace std;

int CWeaponLotteryCfgMaster::ValidateCfg()const
{
    int iRet = ERET_OK;

    set<unsigned int> stWeaponLotteryIDSet;
    
    for(int iIdx = 0; iIdx < m_stCfg.weaponlotterycfg_size(); iIdx++)
    {
        unsigned int uiCfgID = m_stCfg.weaponlotterycfg(iIdx).id();
        if(stWeaponLotteryIDSet.insert(uiCfgID).second == false)
        {
            CV_ERROR(("%s, cfgid = %u is dup cfg id !",
                __PRETTY_FUNCTION__, uiCfgID));
            return -1;//idÖØ¸´
        }

    }
    return iRet;
}

int CWeaponLotteryCfgMaster::SetCfg()
{
    return 0;
}

const WeaponLotteryCfg* CWeaponLotteryCfgMaster::GetWeaponLotteryCfg(const int iLotType)const
{
    if(!vmsg::CS_LOTTERY_TYPE_IsValid(iLotType))
    {
        CV_WARNING(("%s,invalid lot type %d", __PRETTY_FUNCTION__, iLotType));
        return NULL;
    }
    int iIdx = iLotType - 1;


    return &(m_stCfg.weaponlotterycfg(iIdx));

}

