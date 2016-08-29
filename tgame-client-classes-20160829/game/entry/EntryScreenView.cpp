#include "EntryScreenView.h"

#include "basic/SceneManager.h"

#include "game/UserObj.h"
#include "game/weaponlib/WeaponLibControl.h"
#include "game/evolution/EvolutionControl.h"
#include "game/arena/ArenaControl.h"
#include "game/fightendless/FightEndlessControl.h"

EntryScreenView::EntryScreenView()
:m_pBackBtn(NULL)
,m_pScreenScrollView(NULL)
,m_pBgImg(NULL)
,m_pEntryLayout(NULL)
,m_nScrollType(-1)
,m_pEquipEntryBtn(NULL)
,m_pWeaponEntryBtn(NULL)
,m_pEvolutionEntryBtn(NULL)
,m_pArenaEntryBtn(NULL)
,m_pInfinitEntryBtn(NULL)
{
}

EntryScreenView::~EntryScreenView()
{
}

EntryScreenView* EntryScreenView::create() {
	EntryScreenView *pRet = new EntryScreenView();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EntryScreenView::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void EntryScreenView::initUI() {
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();

	initLayoutFromJsonFile("ui_v2/EntryScreenViewUI.ExportJson");
	m_pRootLayout->setSize(screenSize);
	addChild(m_pRootLayout);

	m_pBackBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("back_btn"));
	m_pBackBtn->addTouchEventListener(this, toucheventselector(EntryScreenView::touchEventHandler));

	m_pScreenScrollView = dynamic_cast<ScrollView*>(m_pRootLayout->getChildByName("screen_scroll"));
	m_pScreenScrollView->setSize(screenSize);
	m_pScreenScrollView->addEventListenerScrollView(this, scrollvieweventselector(EntryScreenView::scrollEventHandler));
	m_pScreenScrollView->addTouchEventListener(this, toucheventselector(EntryScreenView::touchEventHandler));

	m_pBgImg = dynamic_cast<ImageView*>(m_pScreenScrollView->getChildByName("bg_img"));
	m_pBgImg->loadTexture("ui_bg/bg_entry_scene.jpg");
	m_pBgImg->setScale(screenSize.height / m_pBgImg->getSize().height);

	m_pEntryLayout = dynamic_cast<UILayout*>(m_pScreenScrollView->getChildByName("entry_layout"));
	m_pEntryLayout->setScale(screenSize.height / m_pBgImg->getSize().height);

	m_pEquipEntryBtn = dynamic_cast<UIButton*>(m_pEntryLayout->getChildByName("equip_entry_btn"));
	m_pEquipEntryBtn->addTouchEventListener(this, toucheventselector(EntryScreenView::touchEventHandler));

	m_pWeaponEntryBtn = dynamic_cast<UIButton*>(m_pEntryLayout->getChildByName("weapon_entry_btn"));
	m_pWeaponEntryBtn->addTouchEventListener(this, toucheventselector(EntryScreenView::touchEventHandler));

	m_pEvolutionEntryBtn = dynamic_cast<UIButton*>(m_pEntryLayout->getChildByName("evolution_entry_btn"));
	m_pEvolutionEntryBtn->addTouchEventListener(this, toucheventselector(EntryScreenView::touchEventHandler));

	m_pArenaEntryBtn = dynamic_cast<UIButton*>(m_pEntryLayout->getChildByName("shop_entry_btn"));
	m_pArenaEntryBtn->addTouchEventListener(this, toucheventselector(EntryScreenView::touchEventHandler));

	m_pInfinitEntryBtn = dynamic_cast<UIButton*>(m_pEntryLayout->getChildByName("infinit_entry_btn"));
	m_pInfinitEntryBtn->addTouchEventListener(this, toucheventselector(EntryScreenView::touchEventHandler));

	CCArray *pChildren = m_pEntryLayout->getChildren();
	CCObject *pObj = NULL;

	CCARRAY_FOREACH(pChildren, pObj) {
		UILabel *pNameTxt = dynamic_cast<UILabel*>(pObj);

		if(pNameTxt != NULL) {
			CCLabelTTF *pLabel = dynamic_cast<CCLabelTTF*>(pNameTxt->getVirtualRenderer());
			pLabel->enableStroke(ccc3(0x56, 0xdb, 0x33), 1.0f);
			pLabel->enableShadow(CCSizeMake(2,-2), 1.0f, 1.0f);
		}
	}

	m_pScreenScrollView->setInnerContainerSize(m_pBgImg->getSize() * screenSize.height / m_pBgImg->getSize().height);
}

void EntryScreenView::touchEventHandler(CCObject *pSender, TouchEventType type) {
	Widget *pWidget = dynamic_cast<Widget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pBackBtn->getName()) == 0) {
				unsigned int uSceneID = P_USER_OBJ->getUserInfo()->sceneID;
				SceneManager::sharedSceneManager()->enterScene(uSceneID);
			} else if(std::strcmp(pszWidgetName, m_pScreenScrollView->getName()) == 0) {
				if(m_nScrollType == SCROLLVIEW_EVENT_SCROLL_TO_LEFT) {
					
				} else if(m_nScrollType == SCROLLVIEW_EVENT_SCROLL_TO_RIGHT) {

				}

				m_nScrollType = -1;
			}/* else if(std::strcmp(pszWidgetName, m_pEquipEntryBtn->getName()) == 0) { // 装备强化
				P_EQUIP_CTRL->getEquipRefinePanel()->open();
			} */else if(std::strcmp(pszWidgetName, m_pWeaponEntryBtn->getName()) == 0) { // 枪械培养
				P_WEAPON_CTRL->getWeaponLibPanel()->open();
			} else if(std::strcmp(pszWidgetName, m_pEvolutionEntryBtn->getName()) == 0) { // 进化
				P_EVOLUTION_CTRL->openEvolutionPanel();
			} else if(std::strcmp(pszWidgetName, m_pArenaEntryBtn->getName()) == 0) { // 竞技场
				P_ARENA_CTRL->getArenaPanel()->open();
			} else if(std::strcmp(pszWidgetName, m_pInfinitEntryBtn->getName()) == 0) { // 无尽模式
				P_ENDLESS_CTRL->getEndlessRankPanel()->open();
			}
			break;
		default:;
	}
}

void EntryScreenView::scrollEventHandler(CCObject *pSender, ScrollviewEventType type) {
	ScrollView *pScrollView = dynamic_cast<ScrollView*>(pSender);

	switch(type) {
		case SCROLLVIEW_EVENT_SCROLL_TO_LEFT:
			m_nScrollType = SCROLLVIEW_EVENT_SCROLL_TO_LEFT;
			break;
		case SCROLLVIEW_EVENT_SCROLL_TO_RIGHT:
			m_nScrollType = SCROLLVIEW_EVENT_SCROLL_TO_RIGHT;
			break;
		default:;
	}
}
