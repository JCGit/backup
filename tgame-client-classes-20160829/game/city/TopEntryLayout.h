#ifndef TopEntryLayout_h__
#define TopEntryLayout_h__

#include "basic/BaseLayout.h"

class TopEntryLayout: public BaseLayout
{
public:
	TopEntryLayout();
	virtual ~TopEntryLayout();

	static TopEntryLayout* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	UIButton* getEntryBtn(unsigned int uEntryID); // ��ȡ��ڰ�ť

	void addEntryBtn(unsigned int uEntryID); // ���һ����ڰ�ť

	void removeEntryBtn(unsigned int uEntryID); // �Ƴ�һ����ڰ�ť

	void updateEntry();

	CCPoint calcEntryPos(unsigned int uEntryID);

	void badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum); // BadgeNum����0����ʾ

	void updateOnlineAndMorrow();
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void scheduelOnlineTime(float dt);
private:
	map<unsigned int, UIButton*> m_stBtnMap; // ������ڰ�ť  <EntryID, EntryBtn*>

	vector<unsigned int> m_stRenderList; // �ѿ������ <EntryID>

	vector<unsigned int> m_removeEnterVec;// ��ʱ��չʾ�����

	map<unsigned int, int> m_leftTimeMap; //ʣ��ʱ��  <EntryID, leftTime>
};

#endif // TopEntryLayout_h__