#ifndef NotificationLayout_h__
#define NotificationLayout_h__

#include "basic/BaseLayout.h"

class NotificationLayout: public BaseLayout
{
public:
	NotificationLayout();
	virtual ~NotificationLayout();

	static NotificationLayout* create();

	// override start

	virtual bool init();

	// override end

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton *m_pNotifyBtn;

};

#endif // NotificationLayout_h__