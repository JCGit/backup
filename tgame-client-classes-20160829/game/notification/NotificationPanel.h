#ifndef NotificationPanel_h__
#define NotificationPanel_h__

#include "basic/BasePanel.h"

class NotificationPanel: public BasePanel
{
public:
	NotificationPanel();
	virtual ~NotificationPanel();

	static NotificationPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	// override end

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pCloseBtn;

	UIImageView *m_pEntryImg;

	UILabel *m_pNotificationTxt;

	UIButton *m_pGotoBtn;

};

#endif // NotificationPanel_h__