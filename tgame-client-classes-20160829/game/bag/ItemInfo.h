#ifndef ItemInfo_h__
#define ItemInfo_h__

#include "CSEquipItemMsg.pb.h"
#include "ItemCfg.pb.h"

using namespace com::cfg::vo;

class ItemInfo
{
public:
	ItemInfo();
	~ItemInfo();

	const ItemCfg* getItemCfg() const;

	void update(const vmsg::CSItemInfo stInfo);

private:
	unsigned int m_uItemCfgID;		// ��Ʒ����ID

	unsigned int m_uItemCnt;		// ��Ʒ����
};

#endif // ItemInfo_h__
