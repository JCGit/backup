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

	UIButton* getEntryBtn(unsigned int uEntryID); // 获取入口按钮

	void addEntryBtn(unsigned int uEntryID); // 添加一个入口按钮

	void removeEntryBtn(unsigned int uEntryID); // 移除一个入口按钮

	void updateEntry();

	CCPoint calcEntryPos(unsigned int uEntryID);

	void badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum); // BadgeNum大于0才显示

	void updateOnlineAndMorrow();
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void scheduelOnlineTime(float dt);
private:
	map<unsigned int, UIButton*> m_stBtnMap; // 所有入口按钮  <EntryID, EntryBtn*>

	vector<unsigned int> m_stRenderList; // 已开放入口 <EntryID>

	vector<unsigned int> m_removeEnterVec;// 暂时不展示的入口

	map<unsigned int, int> m_leftTimeMap; //剩余时间  <EntryID, leftTime>
};

#endif // TopEntryLayout_h__