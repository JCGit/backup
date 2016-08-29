#ifndef ChooseSvrPanel_h__
#define ChooseSvrPanel_h__

#include "basic/BasePanel.h"

class SvrCell;

class ChooseSvrPanel: public BasePanel
{
public:
	ChooseSvrPanel();
	virtual ~ChooseSvrPanel();

	static ChooseSvrPanel* create();

	// override start

    virtual void open();
	virtual bool init();

	virtual std::string getDescription() const;

	// override end

	void updateSvrList();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIImageView *m_pBgImg;

	UIButton *m_pBackBtn;

	UIListView *m_pRecentLoginListView;

	UIScrollView *m_pSvrListScrollView;

	CCSize m_stScrollViewSize;

	SvrCell *m_pCurSelectedSvrCell;
};

#define ROWS 2
#define COLS 5

#endif // ChooseSvrPanel_h__
