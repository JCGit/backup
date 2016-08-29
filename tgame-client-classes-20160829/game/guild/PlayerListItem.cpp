#include "PlayerListItem.h"

#include "defines.h"
#include "GuildControl.h"
#include "game/bag/BagControl.h"
#include "game/login/LoginControl.h"

PlayerListItem::PlayerListItem()
:m_pBgSelectedImg(NULL)
,m_pNameTxt(NULL)
,m_pLevelTxt(NULL)
,m_pLookupBtn(NULL)
,m_bSelected(false)
,m_uin(0)
{
}

PlayerListItem::~PlayerListItem()
{
}

PlayerListItem* PlayerListItem::create() {
	PlayerListItem *pRet = new PlayerListItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool PlayerListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui/PlayerListItemUI.json");
		addChild(m_pRootLayout);

		m_pBgSelectedImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_selected_img"));

		m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));

		m_pLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_txt"));

		m_pLookupBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("lookup_btn"));
		m_pLookupBtn->addTouchEventListener(this, toucheventselector(PlayerListItem::touchEventHandler));

		selected(true);
		selected(false);

		bRet = true;
	} while(0);

	return bRet;
}

void PlayerListItem::updateUI(const vmsg::CSGuildReq &stGuildRqst) {
	m_uin = stGuildRqst.uin();
	m_pNameTxt->setText(stGuildRqst.name());
	m_pLevelTxt->setText(intToString(stGuildRqst.level()));
}

bool PlayerListItem::isSelected() {
	return m_bSelected;
}

void PlayerListItem::selected(bool bSelected) {
	if(m_bSelected != bSelected) {
		m_bSelected = bSelected;
		m_pBgSelectedImg->setVisible(m_bSelected);
	}
}

void PlayerListItem::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pLookupBtn->getName()) == 0) {
				P_LOGIN_CTRL->getProxy()->sendQueryOtherPlayerInfoRqst(m_uin);
			}
			break;
		default:
			break;
	}
}
