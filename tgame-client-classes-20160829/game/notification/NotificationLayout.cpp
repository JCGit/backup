#include "NotificationLayout.h"

#include "NotificationControl.h"

NotificationLayout::NotificationLayout()
:m_pNotifyBtn(NULL)
{
}

NotificationLayout::~NotificationLayout()
{
}

NotificationLayout* NotificationLayout::create() {
	NotificationLayout *pRet = new NotificationLayout();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool NotificationLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		m_pNotifyBtn = UIButton::create();
		m_pNotifyBtn->loadTextureNormal("city/btn_notify_up.png", UI_TEX_TYPE_PLIST);
		m_pNotifyBtn->loadTexturePressed("city/btn_notify_down.png", UI_TEX_TYPE_PLIST);
		CCSize btnSize = m_pNotifyBtn->getSize();
		m_pNotifyBtn->setPosition(ccp(btnSize.width / 2, btnSize.height / 2));
		addChild(m_pNotifyBtn);
		m_pNotifyBtn->addTouchEventListener(this, toucheventselector(NotificationLayout::touchEventHandler));
		setSize(btnSize);
		setTouchEnabled(false);

		bRet = true;
	} while(0);

	return bRet;
}

void NotificationLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pNotifyBtn->getName()) == 0) {
				P_NOTIFICATION_CTRL->showNotification();
			}
			break;
		default:;
	}
}
