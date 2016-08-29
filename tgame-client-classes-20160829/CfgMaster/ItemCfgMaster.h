#ifndef __ITEM_CFG_MASTER_H__
#define __ITEM_CFG_MASTER_H__

#include "ItemCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CItemCfgMaster : public CCfgMasterBase<ItemCfgSet>
{
public:

    CItemCfgMaster(){}
    ~CItemCfgMaster(){}

    const ItemCfg* GetItemCfgByID(const unsigned int uiCfgID);

	const vector<unsigned int>* GetItemsByType(unsigned int uiType);
    const char* GetItemIcon(const unsigned int uiCfgID);
    const char* GetItemName(const unsigned int uiCfgID);
    int GetItemColor(const unsigned int uiCfgID);
protected:

    virtual int ValidateCfg()const;

    virtual int SetCfg();

    virtual void Clear() 
    {
        m_stID2IDX.clear(); 
        m_stType2Items.clear();
    }
private:
    //id到下标的映射
    map<unsigned int, int> m_stID2IDX;

	//类型对应items
	map<int, vector<unsigned int> > m_stType2Items;
};

#define ITEM_CFG_MASTER (CVLib::CVSingleton<CItemCfgMaster>::Instance())

#endif

