#include "HeadEntryLayout.h"

#include "defines.h"

#include "basic/ToolUtils.h"

#include "game/control/AudioManager.h"
#include "game/mail/MailControl.h"

#include "EntryControl.h"

#define BTN_PREFIX_NAME "btn_" // 按钮前缀
#define BTN_MARGIN 8 // 按钮间距
#define BTN_CHILD_EFF_TAG 101
#define BTN_TIP_CHILD_TAG 102

HeadEntryLayout::HeadEntryLayout()
{
}

HeadEntryLayout::~HeadEntryLayout()
{
	CToolUtils::removeArmatureFile("effect/ut00030/ut00030.ExportJson");
}

HeadEntryLayout* HeadEntryLayout::create(UILayout *pRefLayout) {
	HeadEntryLayout *pRet = new HeadEntryLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool HeadEntryLayout::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);

		// 入口排列顺序， 从左到右
		unsigned int stOrderLst[] = {
			ENTRY_CHAT,
			ENTRY_MAIL,
			ENTRY_SETTINGS,
			ENTRY_SECRET_CHAT,
		};

		m_stOrderList = std::vector<unsigned int>(stOrderLst, stOrderLst + sizeof(stOrderLst) / sizeof(unsigned int));

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/city.plist", "ui_v2/city.png");
		
		CToolUtils::loadArmatureRGBA4444("effect/ut00030/ut00030.ExportJson");
		
		bRet = true;
	} while(0);

	return bRet;
}

UIButton* HeadEntryLayout::createEntryBtn(unsigned int uEntryID) {
	UIButton *pBtn = UIButton::create();
	std::string btnName = std::string(BTN_PREFIX_NAME) + intToString(uEntryID);
	pBtn->setName(btnName.c_str());

	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/entry.plist", "ui_v2/entry.png");

	if(uEntryID == ENTRY_CHAT) {
		pBtn->loadTextureNormal("city/btn_chat_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_MAIL) {
		pBtn->loadTextureNormal("city/btn_mail_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_SETTINGS) {
		pBtn->loadTextureNormal("city/btn_settings_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_SECRET_CHAT) {
		pBtn->loadTextureNormal("city/btn_secret_chat_up.png", UI_TEX_TYPE_PLIST);
	}

	return pBtn;
}

UIButton* HeadEntryLayout::getEntryBtn(unsigned int uEntryID) {
	UIButton *pRet = NULL;
	std::map<unsigned int, UIButton*>::const_iterator iter = m_stBtnMap.find(uEntryID);

	if(iter != m_stBtnMap.end()) {
		pRet = iter->second;
	}

	return pRet;
}

void HeadEntryLayout::addEntryBtn(unsigned int uEntryID) {
	UIButton *pBtn = getEntryBtn(uEntryID);

	if(pBtn == NULL) {
		pBtn = createEntryBtn(uEntryID);
		m_stBtnMap[uEntryID] = pBtn;
		pBtn->retain(); 
	}

	if(pBtn->getParent() == NULL) {
		pBtn->setPosition(ccp(m_pRootLayout->getSize().width / 2, m_pRootLayout->getSize().height / 2));
		pBtn->setVisible(false); // 展开时才可见
		pBtn->setTouchEnabled(false);
		pBtn->addTouchEventListener(this, toucheventselector(HeadEntryLayout::touchEventHandler));

		m_pRootLayout->addChild(pBtn);
	}
}

void HeadEntryLayout::removeEntryBtn(unsigned int uEntryID) {
	UIButton *pBtn = getEntryBtn(uEntryID);

	if(pBtn != NULL && pBtn->getParent() != NULL) {
		pBtn->removeFromParent();
		m_stBtnMap.erase(uEntryID);
		pBtn->release(); 
	}
}

void HeadEntryLayout::updateEntry() {
	// update entry
	m_stRenderList.clear();
	unsigned int uOrderListSize = m_stOrderList.size();

	for(unsigned int i = 0; i < uOrderListSize; i++) {
		unsigned int uEntryID = m_stOrderList[i];

		if(P_ENTRY_CTRL->isEntryOpen(uEntryID)) {
			m_stRenderList.push_back(uEntryID);
			UIButton *pBtn = getEntryBtn(uEntryID);

			if(pBtn == NULL) {
				pBtn = createEntryBtn(uEntryID);
			}

			addEntryBtn(uEntryID);
		} else {
			removeEntryBtn(uEntryID);
		}
	}

	CCSize size = m_pRootLayout->getSize();
	CCPoint pos = ccp(0, 0);

	for(unsigned int i = 0; i < m_stRenderList.size(); i++) {
		UIButton *pBtn = getEntryBtn(m_stRenderList[i]);

		if(pBtn != NULL) {
			float posX = pos.x + BTN_MARGIN + pBtn->getSize().width / 2;
			float posY = pos.y + size.height - pBtn->getSize().height / 2;
			pos.x = posX + pBtn->getSize().width / 2;

			pBtn->setPosition(ccp(posX, posY));
			pBtn->setVisible(true);
			pBtn->setTouchEnabled(true);
		}
	}
}

void HeadEntryLayout::mailNotify() {
	badgeEntryBtn(ENTRY_MAIL, P_MAIL_CTRL->getMailObj()->getUnReadMailNum());
	/*UIButton *pBtn = getEntryBtn(ENTRY_MAIL);

	if(pBtn != NULL) {
		if(P_MAIL_CTRL->getMailObj()->getUnReadMailNum() > 0) {
			AudioManager::playEffect("ut00030.ogg");
			CCArmature *pMailNotifyArmature = dynamic_cast<CCArmature*>(pBtn->getNodeByTag(BTN_CHILD_EFF_TAG));
			
			if(pMailNotifyArmature == NULL) {
				pMailNotifyArmature = CCArmature::create("ut00030");
				pBtn->addNode(pMailNotifyArmature, 3, BTN_CHILD_EFF_TAG);
				pMailNotifyArmature->getAnimation()->play("fly", -1, -1, 1);
			} else {
				pMailNotifyArmature->getAnimation()->play("fly", -1, -1, 1);
			}
		} else {
			CCArmature *pMailNotifyArmature = dynamic_cast<CCArmature*>(pBtn->getNodeByTag(BTN_CHILD_EFF_TAG));

			if(pMailNotifyArmature != NULL) {
				pMailNotifyArmature->getAnimation()->stop();
				pMailNotifyArmature->removeFromParent();
			}
		}
	}*/
}

void HeadEntryLayout::badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum) {
	UIButton *pBtn = getEntryBtn(uEntryID);
	
	if(pBtn != NULL) {
		float offsetX = pBtn->getSize().width * 0.32f;
		float offsetY = pBtn->getSize().height * 0.32f;
		CToolUtils::badgeWidgetWithPoint(pBtn, uBadgeNum, offsetX, offsetY);
	}
}

void HeadEntryLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(string(pszWidgetName).substr(0, string(BTN_PREFIX_NAME).size()).c_str(), BTN_PREFIX_NAME) == 0) {
				unsigned int uEntryID = atol(string(pszWidgetName).substr(string(BTN_PREFIX_NAME).size(), string(pszWidgetName).size()).c_str());

				P_ENTRY_CTRL->enter(uEntryID);

				if(uEntryID == ENTRY_SECRET_CHAT) {
					removeEntryBtn(uEntryID);
				}
			}
			break;
		default:;
	}
}
