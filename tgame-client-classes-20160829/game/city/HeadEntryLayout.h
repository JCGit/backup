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

	UIButton* createEntryBtn(unsigned int uEntryID); // ����һ����ڰ�ť

	UIButton* getEntryBtn(unsigned int uEntryID); // ��ȡ��ڰ�ť

	void addEntryBtn(unsigned int uEntryID); // ���һ����ڰ�ť

	void removeEntryBtn(unsigned int uEntryID); // �Ƴ�һ����ڰ�ť

	void updateEntry();

	void mailNotify();

	void badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum);

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	map<unsigned int, UIButton*> m_stBtnMap; // ������ڰ�ť  <EntryID, EntryBtn*>

	vector<unsigned int> m_stOrderList; // ��ť˳�� <EntryID>

	vector<unsigned int> m_stRenderList; // �ѿ������ <EntryID>

};

#endif // HeadEntryLayout_h__