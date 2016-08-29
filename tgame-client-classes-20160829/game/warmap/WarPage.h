#ifndef WarPage_h__
#define WarPage_h__

#include "basic/BaseLayout.h"

class WarPage: public BaseLayout
{
public:
	WarPage();
	virtual ~WarPage();

	static WarPage* create(unsigned int uChapterID);

	bool initWithChapterID(unsigned int uChapterID);

	unsigned int getChapterID();

	void setChapterID(unsigned int uChapterID);

	void updateUI();

	void setTaskTraceStat(const bool bStat) {
		m_bIsTaskTrace = bStat;
	}

	Widget* getCurLevelCell();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	unsigned int m_uChapterID;

	UIImageView *m_pChapterMapImg;

	CCArray *m_pWarCells;

	bool m_bIsTaskTrace; //是否是任务寻路打开的

	Widget *m_pCurLevelWidget;

};

#endif // WarPage_h__