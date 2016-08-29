#ifndef BagLayout_h__
#define BagLayout_h__

#include "basic/BaseLayout.h"

#include "CSTaskMsg.pb.h"

#include "game/guide/GuideControl.h"

class BagCell;

class BagLayout: public BaseLayout
{
public:
	enum BagTabType {
		BAG_TAB_ALL, // 全部
		BAG_TAB_EQUIP, // 装备
		BAG_TAB_MATERIAL, // 材料
	};
public:
	BagLayout();
	virtual ~BagLayout();

	static BagLayout* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	void updateUI();

	void updateCells();

	void gotoTabPage(unsigned int uTabType);

	CCNode* getGuideBagCell(unsigned int equipId);
	CCNode* getEquipTab();
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	void scrollViewEventHandler(CCObject *pSender, ScrollviewEventType type);

private:
	UIScrollView *m_pBagScrollView;

	UILabel *m_pCapacityTxt;

	std::vector<BagCell*> m_stBagCells; 

	BagCell *m_pCurSelectedCell;

	unsigned int m_uBagCapacity;

	UIListView *m_pTabListView;

	int m_nSelectedTabIdx;

	unsigned int m_uTabType;

	UILayout *m_pBagLayout;

};

#endif // BagLayout_h__