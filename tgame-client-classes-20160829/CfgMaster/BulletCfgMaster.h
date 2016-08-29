#ifndef __BULLET_CFG_MASTER_H__
#define __BULLET_CFG_MASTER_H__

#include "BulletCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CBulletCfgMaster : public CCfgMasterBase<BulletCfgSet>
{
public:

    CBulletCfgMaster(){}
    ~CBulletCfgMaster(){}

    const BulletCfg* GetBulletCfgByID(const unsigned int uiCfgID);

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

#define BULLET_CFG_MASTER (CVSingleton<CBulletCfgMaster>::Instance())

#endif

