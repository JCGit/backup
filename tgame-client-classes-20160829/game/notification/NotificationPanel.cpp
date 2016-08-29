#include "NotificationPanel.h"

#include "CfgMaster/NotificationCfgMaster.h"
#include "CfgMaster/EntryCfgMaster.h"

#include "basic/TextHelper.h"

#include "game/city/CityControl.h"

#include "NotificationControl.h"

NotificationPanel::NotificationPanel()
:m_pCloseBtn(NULL)
,m_pEntryImg(NULL)
,m_pNotificationTxt(NULL)
,m_pGotoBtn(NULL)
{
}

NotificationPanel::~NotificationPanel()
{
}

NotificationPanel* NotificationPanel::create() {
	NotificationPanel *pRet = new NotificationPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool NotificationPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/NotificationPanelUI.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(NotificationPanel::touchEventHandler));

		m_pEntryImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("entry_img"));

		m_pNotificationTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("notification_txt"));

		m_pGotoBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("goto_btn"));
		m_pGotoBtn->addTouchEventListener(this, toucheventselector(NotificationPanel::touchEventHandler));

		bRet = true;
	} while(0);

	return bRet;
}

void NotificationPanel::open() {
	BasePanel::open();
	const Notification *pNotification = P_NOTIFICATION_CTRL->getFrontNotification(REMIND_BY_ALERT);

	if(pNotification != NULL) {
		string url = "";
		const EntryCfg *pEntryCfg = ENTRY_CFG_MASTER->getEntryCfg(pNotification->entryid());
		url = string("icon/entry/") + pEntryCfg->entryicon() + ".png";
		m_pEntryImg->loadTexture(url.c_str());
		m_pNotificationTxt->setText(pNotification->words());
	}
}

void NotificationPanel::close() {
	BasePanel::close();

	const Notification *pNotification = P_NOTIFICATION_CTRL->getFrontNotification(REMIND_BY_ALERT);

	if(pNotification != NULL) {
		P_NOTIFICATION_CTRL->lockNotification(pNotification->entryid(), pNotification->tag());
	}
}

void NotificationPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(strcmp(pszWidgetName, m_pGotoBtn->getName()) == 0) {
				const Notification *pNotification = P_NOTIFICATION_CTRL->getFrontNotification(REMIND_BY_ALERT);

				if(pNotification != NULL) {
					unsigned int uEntryID = pNotification->entryid();

					if(uEntryID > 0) {
						P_ENTRY_CTRL->enter(uEntryID);
					} else {
						P_CITY_CTRL->moveToBattlePortal();
					}

					P_NOTIFICATION_CTRL->removeNotification(pNotification->entryid(), pNotification->tag());
					BasePanel::close();
				}
			}
			break;
		default:;
	}
}
