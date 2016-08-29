#include "ItemInfo.h"

#include "ItemCfgMaster.h"

ItemInfo::ItemInfo()
:m_uItemCfgID(0)
,m_uItemCnt(0)
{
}

ItemInfo::~ItemInfo()
{
}

const ItemCfg* ItemInfo::getItemCfg() const {
    return ITEM_CFG_MASTER->GetItemCfgByID(m_uItemCfgID);
}

void ItemInfo::update(const vmsg::CSItemInfo stInfo) {
	 m_uItemCfgID = stInfo.itemcfgid();
	 m_uItemCnt = stInfo.itemcnt();

}
