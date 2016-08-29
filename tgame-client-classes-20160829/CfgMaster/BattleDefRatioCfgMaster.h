#ifndef __BATTLE_DEF_RATIO_CFG_MASTER_H__
#define __BATTLE_DEF_RATIO_CFG_MASTER_H__

#include "BattleDefRatioCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CBattleDefRatioCfgMaster : public CCfgMasterBase<BattleDefRatioCfgSet>
{
public:

    CBattleDefRatioCfgMaster(){}
    ~CBattleDefRatioCfgMaster(){}

    BattleDefRatioCfg* GetBattleDefRatioCfgByID(const unsigned int uiCfgID);

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

#define BATTLE_DEF_RATIO_CFG_MASTER (CVSingleton<CBattleDefRatioCfgMaster>::Instance())

#endif

