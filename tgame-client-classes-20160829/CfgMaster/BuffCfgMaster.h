#ifndef __BUFF_CFG_MASTER_H__
#define __BUFF_CFG_MASTER_H__

#include "BuffCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CBuffCfgMaster : public CCfgMasterBase<BuffCfgSet>
{
public:

    CBuffCfgMaster(){}
    ~CBuffCfgMaster(){}

    BuffCfg* GetBuffCfgByID(const unsigned int uiCfgID);

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

#define BUFF_CFG_MASTER (CVSingleton<CBuffCfgMaster>::Instance())

#endif

