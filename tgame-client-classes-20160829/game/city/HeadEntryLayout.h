#ifndef HeadEntryLayout_h__
#define HeadEntryLayout_h__

#include "basic/BaseLayout.h"

class HeadEntryLayout: public BaseLayout
{
public:
	HeadEntryLayout();
	virtual ~HeadEntryLayout();

	static HeadEntryLayout* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	UIButton* createEntryBtn(unsigned int uEntryID); // 创建一个入口按钮

	UIButton* getEntryBtn(unsigned int uEntryID); // 获取入口按钮

	void addEntryBtn(unsigned int uEntryID); // 添加一个入口按钮

	void removeEntryBtn(unsigned int uEntryID); // 移除一个入口按钮

	void updateEntry();

	void mailNotify();

	void badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	map<unsigned int, UIButton*> m_stBtnMap; // 所有入口按钮  <EntryID, EntryBtn*>

	vector<unsigned int> m_stOrderList; // 按钮顺序 <EntryID>

	vector<unsigned int> m_stRenderList; // 已开放入口 <EntryID>

};

#endif // HeadEntryLayout_h__