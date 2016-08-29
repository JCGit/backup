#ifndef ItemDropTipCfgMaster_h__
#define ItemDropTipCfgMaster_h__

#include "ItemDropTipCfg.pb.h"
#include "CfgMasterBase.h"

using namespace com::cfg::vo;

class ItemDropTipCfgMaster : public CCfgMasterBase<ItemDropTipCfgSet>
{
public:
    enum 
    {
        WAR_TYPE_ELITE  = 0, //0 精英副本
        WAR_TYPE_NORMAL = 1, //1 普通副本
    };

    ItemDropTipCfgMaster(){}
    ~ItemDropTipCfgMaster(){}

    const ItemDropTipCfg* GetOneCfg(const int iThingID, const int iThingType);

    virtual void Clear() 
    {
        m_stCfgMap.clear(); 
    }

    string getWarName(const int iWarType, const int iWarID);
protected:

    virtual int ValidateCfg()const;
    virtual int SetCfg();

private:
    map< pair<int, int>, int> m_stCfgMap; //< <ThingID, ThingType>, cfg index>
};

#define ITEM_DROP_TIP_MASTER (CVSingleton<ItemDropTipCfgMaster>::Instance())


#endif // ItemDropTipCfgMaster_h__
