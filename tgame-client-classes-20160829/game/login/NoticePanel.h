#ifndef NoticePanel_h__
#define NoticePanel_h__

#include "basic/BasePanel.h"

class NoticePanel: public BasePanel
{
public:
	NoticePanel();
	virtual ~NoticePanel();

	static NoticePanel* create();

	// override start

	virtual bool init();

	virtual void open();

	// override end

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void limitWidth(std::string &text, unsigned int width);

private:
	UIScrollView *m_pNoticeScrollView;

	UILabel *m_pNoticeTxt;

	UIButton *m_pOKBtn;

};

#endif // NoticePanel_h__