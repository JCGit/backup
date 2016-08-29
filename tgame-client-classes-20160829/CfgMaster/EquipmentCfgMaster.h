#ifndef __EQUIPMENT_CFG_MASTER_H__
#define __EQUIPMENT_CFG_MASTER_H__

#include "CfgMasterBase.h"
#include "EquipmentCfg.pb.h"
#include <map>

using namespace std;
using namespace com::cfg::vo;

class CEquipmentCfgMaster : public CCfgMasterBase<EquipmentCfgSet>
{
public:

    CEquipmentCfgMaster(){}
    ~CEquipmentCfgMaster(){}

    const EquipmentCfg* GetEquipCfgByID(const unsigned int uiCfgID);

    unsigned int GetHoleCnt(const unsigned int uiCfgID)const;
    
    bool CheckJob( unsigned int uiEquipCfgID, unsigned int uiJobID );
    const std::string  GetEquipName(const unsigned int uiCfgID);

    const std::string GetIconPath(const unsigned int uiCfgID);
    const int GetEquipColor(const unsigned int uiCfgID);

    const EquipmentCfg* GetEquipCfgByStarStep(unsigned int uiEquipID, unsigned int uiStar, unsigned int uiStep)const;
protected:
    
    virtual int ValidateCfg()const;

    virtual int SetCfg();
    
    virtual void Clear() 
    {
        m_stID2IDX.clear(); 
        m_stStarStepID2Idx.clear();
    }
private:
    static unsigned int MakeKey(unsigned int uiEquipID, unsigned int uiStar, unsigned int uiStep); 
private:
    //id到下标的映射
    map<unsigned int, int> m_stID2IDX;
    map<unsigned int, int> m_stStarStepID2Idx;
};

#define EQUIPMENT_CFG_MASTER (CVSingleton<CEquipmentCfgMaster>::Instance())

#endif

