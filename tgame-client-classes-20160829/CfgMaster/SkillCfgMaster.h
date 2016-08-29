#ifndef __SKILL_CFG_MASTER_H__
#define __SKILL_CFG_MASTER_H__

#include "SkillCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CSkillCfgMaster : public CCfgMasterBase<SkillCfgSet>
{
public:

    CSkillCfgMaster(){}
    ~CSkillCfgMaster(){}

    const SkillCfg* GetSkillCfgByID(const unsigned int uiCfgID);

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

#define SKILL_CFG_MASTER (CVSingleton<CSkillCfgMaster>::Instance())

#endif

