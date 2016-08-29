#ifndef __NOUN_CFG_MASTER_H__
#define __NOUN_CFG_MASTER_H__

#include "CfgMasterBase.h"
#include "NounCfg.pb.h"

using namespace std;
using namespace com::cfg::vo;

class CNounCfgMaster : public CCfgMasterBase<NounCfgSet>
{
public:

    CNounCfgMaster(){}
    ~CNounCfgMaster(){}

    std::string GetStr(const int iKey);
protected:
    
    virtual int ValidateCfg()const;

    virtual int SetCfg();
    
private:
    map<int, string> m_stMap;
};

#define NOUN_CFG_MASTER (CVSingleton<CNounCfgMaster>::Instance())

#endif

