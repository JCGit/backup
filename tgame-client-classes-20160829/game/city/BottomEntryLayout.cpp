#include "BottomEntryLayout.h"

#include "defines.h"
#include "QEnvSet.h"

#include "Macros.pb.h"

#include "basic/CCColorUtil.h"
#include "basic/TextHelper.h"
#include "basic/SceneManager.h"
#include "basic/ToolUtils.h"

#include "game/task/TaskControl.h"
#include "game/notification/NotificationControl.h"

#include "EntryControl.h"
#include "CityControl.h"

#define BTN_PREFIX_NAME "btn_" // 入口按钮名称前缀
#define BTN_WIDTH 100 // 按钮宽
#define BTN_MARGIN 8 // 按钮之间的间距
#define BTN_TIP_CHILD_TAG 5 // 按钮上数字的tag

BottomEntryLayout::BottomEntryLayout()
:m_pUnfoldBtn(NULL)
,m_pFoldBtn(NULL)
,m_pHContainerLayout(NULL)
,m_pVContainerLayout(NULL)
,m_bUnfolded(false)
,m_pFuncBtn(NULL)
,m_pFuncWordsTxt(NULL)
,m_pBgMask(NULL)
,m_pBtnEffArmature(NULL)
,m_uNowModuleID(0)
{
}

BottomEntryLayout::~BottomEntryLayout()
{
	std::map<unsigned int, UIButton*>::const_iterator iter;

	for(iter = m_stBtnMap.begin(); iter != m_stBtnMap.end(); iter++) {
		UIButton *pBtn = iter->second;
		pBtn->release();
	}
}

BottomEntryLayout* BottomEntryLayout::create(UILayout *pRefLayout) {
	BottomEntryLayout *pRet = new BottomEntryLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool BottomEntryLayout::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

        initLayoutWithUI(pRefLayout);

		m_pUnfoldBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("unfold_btn"));
		m_pUnfoldBtn->addTouchEventListener(this, toucheventselector(BottomEntryLayout::touchEventHandler));

		m_pFoldBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("fold_btn"));
		m_pFoldBtn->addTouchEventListener(this, toucheventselector(BottomEntryLayout::touchEventHandler));

		m_pHContainerLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("h_container_layout"));

		m_pVContainerLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("v_container_layout"));

		// 横向入口排列顺序， 从左到右
		unsigned int hOrderList[] = {
			ENTRY_ROLE,
			//ENTRY_EQUIP_CULTURE,
			ENTRY_WEAPON_LIB,
			ENTRY_REFIT,
			ENTRY_GROWTH,
			ENTRY_SOCIAL,
			ENTRY_CHALLENGE,
		};

		m_stHOrderList = std::vector<unsigned int>(hOrderList, hOrderList + sizeof(hOrderList) / sizeof(unsigned int));
		
		// 纵向入口排列顺序， 从上到下
		unsigned int vOrderList[] = {
			ENTRY_RANK,
			ENTRY_BUSINESS,
		};

		m_stVOrderList = std::vector<unsigned int>(vOrderList, vOrderList + sizeof(vOrderList) / sizeof(unsigned int));

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/entry.plist", "ui_v2/entry.png");
		CToolUtils::loadArmatureRGBA4444("effect/ut00055/ut00055.ExportJson");

		updateEntry();

		bRet = true;
	} while(0);

	return bRet;
}

UIButton* BottomEntryLayout::getEntryBtn(unsigned int uEntryID) {
	UIButton *pRet = NULL;
	std::map<unsigned int, UIButton*>::const_iterator iter = m_stBtnMap.find(uEntryID);

	if(iter != m_stBtnMap.end()) {
		pRet = iter->second;
	}

	return pRet;
}

void BottomEntryLayout::addEntryBtn(unsigned int uEntryID) {
	UIButton *pBtn = getEntryBtn(uEntryID);

	if(pBtn == NULL) {
		pBtn = P_ENTRY_CTRL->createEntryBtn(uEntryID);
		m_stBtnMap[uEntryID] = pBtn;
		pBtn->retain(); // release at ModuleEntryLayout::removeEntryBtn(unsigned int uEntryID)
	}

	if(pBtn->getParent() == NULL) {
		pBtn->setPosition(m_pUnfoldBtn->getPosition());
		pBtn->setVisible(false); // 展开时才可见
		pBtn->setTouchEnabled(false);
		pBtn->addTouchEventListener(this, toucheventselector(BottomEntryLayout::touchEventHandler));

		for(unsigned int i = 0; i < m_stHOrderList.size(); i++) {
			if(m_stHOrderList[i] == uEntryID) {
				m_pHContainerLayout->addChild(pBtn);
				break;
			}
		}

		for(unsigned int i = 0; i < m_stVOrderList.size(); i++) {
			if(m_stVOrderList[i] == uEntryID) {
				m_pVContainerLayout->addChild(pBtn);
				break;
			}
		}

	}
}

void BottomEntryLayout::removeEntryBtn(unsigned int uEntryID) {
	UIButton *pBtn = getEntryBtn(uEntryID);

	if(pBtn != NULL && pBtn->getParent() != NULL) {
		pBtn->removeFromParent();
		m_stBtnMap.erase(uEntryID);
		pBtn->release(); // retain at ModuleEntryLayout::addEntryBtn(unsigned int uEntryID)
	}
}

unsigned int BottomEntryLayout::getEntryID(unsigned int uModuleID) {
	return P_ENTRY_CTRL->getEntryIDByModuleID(uModuleID);
}

void BottomEntryLayout::playBtnEff(bool bIsFold){
	if(getNodeByTag(101)){
		removeNodeByTag(101);
	}

	CCArmature *pArm = CCArmature::create("ut00055");
	pArm->setPosition(m_pUnfoldBtn->getPosition());
	addNode(pArm, 0, 101);
	string actStr = "";
	if(bIsFold){
		actStr = "fly_shou";
	}else{
		actStr = "fly_fang";
	}
	pArm->getAnimation()->play(actStr.c_str(), -1, -1, 0);
	pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BottomEntryLayout::handleBtnEffComplete));
}

void BottomEntryLayout::handleBtnEffComplete(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);
	}
}

bool BottomEntryLayout::isEntryUnfold() {
	return m_bUnfolded;
}

void BottomEntryLayout::unfoldEntry() {
	playBtnEff(false);
	// switch btn
	m_pUnfoldBtn->setEnabled(false);
	m_pFoldBtn->setEnabled(true);

	CCPoint originPos = ccp(BTN_WIDTH / 2, BTN_WIDTH / 2);

	// unfold horizontal entry
	unsigned int uHRenderListSize = m_stHRenderList.size();

	for(unsigned int i = 0; i < uHRenderListSize; i++) {
		UIButton *pBtn = getEntryBtn(m_stHRenderList[i]);
		
		if(pBtn != NULL) {
			float posX = originPos.x - (getSize().width - BTN_WIDTH) / 2 - (i + 1) * (BTN_WIDTH + BTN_MARGIN);
			float posY = originPos.y;
			CCActionInterval *pMove = CCMoveTo::create(0.2f * (i + 1), ccp(posX, posY));
			CCActionInterval *pEaseMove = CCEaseBackOut::create(dynamic_cast<CCActionInterval*>(pMove->copy()));

			if(pBtn->numberOfRunningActions() > 0) {
				pBtn->stopAllActions();
			}

			pBtn->setVisible(true);
			pBtn->setTouchEnabled(true);
			pBtn->runAction(pEaseMove);
		}
	}
	
	// unfold vertical entry
	unsigned int uVRenderListSize = m_stVRenderList.size();

	for(unsigned int i = 0; i < uVRenderListSize; i++) {
		UIButton *pBtn = getEntryBtn(m_stVRenderList[i]);

		if(pBtn != NULL) {
			float posX = originPos.x;
			float posY = originPos.y + (getSize().height - BTN_WIDTH) / 2 + (i + 1) * (BTN_WIDTH + BTN_MARGIN);
			CCActionInterval *pMove = CCMoveTo::create(0.2f * (i + 1), ccp(posX, posY));
			CCActionInterval *pEaseMove = CCEaseBackOut::create(dynamic_cast<CCActionInterval*>(pMove->copy()));

			if(pBtn->numberOfRunningActions() > 0) {
				pBtn->stopAllActions();
			}

			pBtn->setVisible(true);
			pBtn->setTouchEnabled(true);
			pBtn->runAction(pEaseMove);
		}
	}

	m_bUnfolded = true;
	 
	float dt = (float)(uHRenderListSize > uVRenderListSize ? uHRenderListSize : uVRenderListSize) * 0.2f;
	scheduleOnce(schedule_selector(BottomEntryLayout::onUnfoldEntry), dt);
	
}

void BottomEntryLayout::foldEntry() {
	playBtnEff(true);
	// switch btn
	m_pUnfoldBtn->setEnabled(true);
	m_pFoldBtn->setEnabled(false);

	CCPoint originPos = ccp(BTN_WIDTH / 2, BTN_WIDTH / 2);

	// fold horizontal entry
	unsigned int uHRenderListSize = m_stHRenderList.size();

	for(unsigned int i = 0; i < uHRenderListSize; i++) {
		UIButton *pBtn = getEntryBtn(m_stHRenderList[i]);

		if(pBtn != NULL) {
			CCActionInterval *pMove = CCMoveTo::create(0.2f * (i + 1), originPos);
			CCActionInterval *pEaseMove = CCEaseBackIn::create(dynamic_cast<CCActionInterval*>(pMove->copy()));
			CCActionInstant *pHide = CCHide::create();
			CCCallFuncN *pFunc = CCCallFuncN::create(this, callfuncN_selector(BottomEntryLayout::onBtnHide));

			if(pBtn->numberOfRunningActions() > 0) {
				pBtn->stopAllActions();
			}

			pBtn->runAction(CCSequence::create(pEaseMove, pHide, pFunc, NULL));
		}
	}

	// fold vertical entry
	unsigned int uVRenderListSize = m_stVRenderList.size();

	for(unsigned int i = 0; i < uVRenderListSize; i++) {
		UIButton *pBtn = getEntryBtn(m_stVRenderList[i]);

		if(pBtn != NULL) {
			CCActionInterval *pMove = CCMoveTo::create(0.2f * (i + 1), originPos);
			CCActionInterval *pEaseMove = CCEaseBackIn::create(dynamic_cast<CCActionInterval*>(pMove->copy()));
			CCActionInstant *pHide = CCHide::create();
			CCCallFuncN *pFunc = CCCallFuncN::create(this, callfuncN_selector(BottomEntryLayout::onBtnHide));

			if(pBtn->numberOfRunningActions() > 0) {
				pBtn->stopAllActions();
			}

			pBtn->runAction(CCSequence::create(pEaseMove, pHide, pFunc, NULL));
		}
	}

	m_bUnfolded = false;

	float dt = (uHRenderListSize > uVRenderListSize ? uHRenderListSize : uVRenderListSize) * 0.2f;
	scheduleOnce(schedule_selector(BottomEntryLayout::onFoldEntry), dt);

	//GUIDE_HIDE(GUIDE_UI_MODULE_ENTRY);
	
}

void BottomEntryLayout::updateEntry() {
	CCPoint originPos = ccp(BTN_WIDTH / 2, BTN_WIDTH / 2);
	// update horizontal entry
	m_stHRenderList.clear();
	unsigned int uHOrderListSize = m_stHOrderList.size();

	for(unsigned int i = 0; i < uHOrderListSize; i++) {
		unsigned int uEntryID = m_stHOrderList[uHOrderListSize - 1 - i];

		if(P_ENTRY_CTRL->isEntryOpen(uEntryID)) {
			m_stHRenderList.push_back(uEntryID);
			UIButton *pBtn = getEntryBtn(uEntryID);

			if(pBtn == NULL) {
				pBtn = P_ENTRY_CTRL->createEntryBtn(uEntryID);
			}

			addEntryBtn(uEntryID);
			pBtn = getEntryBtn(uEntryID);

			if(m_bUnfolded) {
				int idx = m_stHRenderList.size() - 1;
				float posX = originPos.x - (getSize().width - BTN_WIDTH) / 2 - (idx + 1) * (BTN_WIDTH + BTN_MARGIN);
				float posY = originPos.y;
				CCActionInterval *pMove = CCMoveTo::create(0.2f * (idx + 1), ccp(posX, posY));
				CCActionInterval *pEaseMove = CCEaseBackOut::create(dynamic_cast<CCActionInterval*>(pMove->copy()));

				if(pBtn->numberOfRunningActions() > 0) {
					pBtn->stopAllActions();
				}

				pBtn->setVisible(true);
				pBtn->setTouchEnabled(true);
				pBtn->runAction(pEaseMove);
			}
		} else {
			removeEntryBtn(uEntryID);
		}
	}

	// update vertical entry
	m_stVRenderList.clear();
	unsigned int uVOrderListSize = m_stVOrderList.size();

	for(unsigned int i = 0; i < uVOrderListSize; i++) {
		unsigned  int uEntryID = m_stVOrderList[uVOrderListSize - 1 - i];

		if(P_ENTRY_CTRL->isEntryOpen(uEntryID)) {
			m_stVRenderList.push_back(uEntryID);
			UIButton *pBtn = getEntryBtn(uEntryID);

			if(pBtn == NULL) {
				pBtn = P_ENTRY_CTRL->createEntryBtn(uEntryID);
			}

			addEntryBtn(uEntryID);
			pBtn = getEntryBtn(uEntryID);

			if(m_bUnfolded) {
				int idx = m_stVRenderList.size() - 1;
				float posX = originPos.x;
				float posY = originPos.y + (getSize().height - BTN_WIDTH) / 2 + (idx + 1) * (BTN_WIDTH + BTN_MARGIN);
				CCActionInterval *pMove = CCMoveTo::create(0.2f * (idx + 1), ccp(posX, posY));
				CCActionInterval *pEaseMove = CCEaseBackOut::create(dynamic_cast<CCActionInterval*>(pMove->copy()));

				if(pBtn->numberOfRunningActions() > 0) {
					pBtn->stopAllActions();
				}

				pBtn->setVisible(true);
				pBtn->setTouchEnabled(true);
				pBtn->runAction(pEaseMove);
			}
		} else {
			removeEntryBtn(uEntryID);
		}
	}

	if(m_bUnfolded) {
		//unfoldEntry();
	}

	CCLOG("update entry");


}

void BottomEntryLayout::moduleOpen(unsigned int uModuleID) {
	unsigned int uEntryID = getEntryID(uModuleID);

	if(uEntryID != 0) {
		m_uNowModuleID = uModuleID;
		CCDirector *pDirector = CCDirector::sharedDirector();
		CCSize visibleSize = pDirector->getVisibleSize();
		CCPoint visibleOrigin = pDirector->getVisibleOrigin();
		CCPoint centerPos = this->convertToNodeSpace(ccp(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height / 2));

		if(m_pBgMask == NULL) {
			m_pBgMask = UILayout::create();
			m_pBgMask->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
			m_pBgMask->setBackGroundColorType(LAYOUT_COLOR_SOLID);
			m_pBgMask->setBackGroundColorOpacity(0x40);
			m_pBgMask->setTouchEnabled(true);
			m_pBgMask->setName("bg_mask");
			m_pBgMask->setSize(visibleSize);
			m_pBgMask->setPosition(this->convertToNodeSpace(visibleOrigin));
			addChild(m_pBgMask);
		}

		if(m_pFuncBtn == NULL) {
			m_pFuncBtn = P_ENTRY_CTRL->createEntryBtn(uEntryID);
			std::string btnName = std::string("func") + intToString(uEntryID);
			m_pFuncBtn->setName(btnName.c_str());
			m_pFuncBtn->setPosition(centerPos);
			addChild(m_pFuncBtn);
			m_pFuncBtn->setVisible(false);
		}

		if(m_pFuncWordsTxt == NULL) {
			m_pFuncWordsTxt = UILabel::create();
			m_pFuncWordsTxt->setFontName("fonts/tgamefont.ttf");
			m_pFuncWordsTxt->setFontSize(28);
			m_pFuncWordsTxt->setColor(ccc3(0x00, 0xff, 0x00));
			CCColorUtil::addStroke(m_pFuncWordsTxt, 0.8f, ccc3(0xFF, 0xff, 0x00));
			CCColorUtil::addShadow(m_pFuncWordsTxt);
			m_pFuncWordsTxt->setPosition(ccp(centerPos.x, centerPos.y - m_pFuncBtn->getSize().height));
			addChild(m_pFuncWordsTxt);
			m_pFuncWordsTxt->setVisible(false);
			m_pFuncWordsTxt->setText(getModuleText(uModuleID));
		}

		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00011/ut00011.ExportJson");
		m_pBtnEffArmature = CCArmature::create("ut00011");
		addNode(m_pBtnEffArmature, 0, 101);
		m_pBtnEffArmature->setPosition(centerPos);
		m_pBtnEffArmature->getAnimation()->play("fly01", -1, -1, 0);
		m_pBtnEffArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BottomEntryLayout::onBtnEffPlayEvent));
		m_pBtnEffArmature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(BottomEntryLayout::onBtnEffFrameEvent));

	}

	//P_CITY_CTRL->getHeadLayout()->checkModuleOpen();
}

CCPoint BottomEntryLayout::calcEntryPos(unsigned int uEntryID) {
	CCPoint pos = m_pUnfoldBtn->getPosition();
	CCPoint originPos = ccp(BTN_WIDTH / 2, BTN_WIDTH / 2);

	if(!P_ENTRY_CTRL->isEntryOpen(uEntryID)) {
		return pos;
	}

	bool bPosCalculated = false;

	for(unsigned int i = 0; i < m_stHOrderList.size(); i++) {
		if(uEntryID == m_stHOrderList[i]) {
			unsigned int idx = 0;

			while(idx < m_stHRenderList.size()) {
				if(uEntryID == m_stHRenderList[idx]) {
					break;
				}

				idx++;
			}

			pos.x = originPos.x - (getSize().width - BTN_WIDTH) / 2 - (idx + 1) * (BTN_WIDTH + BTN_MARGIN);
			pos.y = originPos.y;
			UIButton *pBtn = getEntryBtn(uEntryID);
			pos = convertToNodeSpace(pBtn->getParent()->convertToWorldSpace(pos));
			bPosCalculated = true;
			break;
		}
	}

	if(!bPosCalculated) {
		for(unsigned int i = 0; i < m_stVOrderList.size(); i++) {
			if(uEntryID == m_stVOrderList[i]) {
				unsigned int idx = 0;

				while(idx < m_stVRenderList.size()) {
					if(uEntryID == m_stVRenderList[idx]) {
						break;
					}

					idx++;
				}

				pos.x = originPos.x;
				pos.y = originPos.y + (getSize().height - BTN_WIDTH) / 2 + (idx + 1) * (BTN_WIDTH + BTN_MARGIN);
				UIButton *pBtn = getEntryBtn(uEntryID);
				pos = convertToNodeSpace(pBtn->getParent()->convertToWorldSpace(pos));
				bPosCalculated = true;
				break;
			}
		}
	}
	
	return pos;
}

CCPoint BottomEntryLayout::getEntryOriginPos() {
	return m_pUnfoldBtn->getPosition();
}

void BottomEntryLayout::badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum) {
	UIButton *pBtn = getEntryBtn(uEntryID);

	if(pBtn != NULL) {
		float offsetX = pBtn->getSize().width * 0.32f;
		float offsetY = pBtn->getSize().height * 0.32f;
		//CToolUtils::badgeWidgetWithPoint(pBtn, uBadgeNum, offsetX, offsetY);
		CToolUtils::effectEntryIcon(pBtn, uBadgeNum > 0);
	}
}

void BottomEntryLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pUnfoldBtn->getName()) == 0) {
				unfoldEntry();
			} else if(std::strcmp(pszWidgetName, m_pFoldBtn->getName()) == 0) {
				foldEntry();
			} else if(string(pszWidgetName).substr(0, string(ENTRY_PREFIX_NAME).size()) == ENTRY_PREFIX_NAME) {
				string btnName = pszWidgetName;
				unsigned int uPrefixSize = string(ENTRY_PREFIX_NAME).size();
				string strEntryID = btnName.substr(uPrefixSize, btnName.size());
				unsigned int uEntryID = atol(strEntryID.c_str());
				GUIDE_TOUCH(pWidget);
				CCArmature* pArmature = NULL;

				switch(uEntryID) {
					case ENTRY_ROLE:
					case ENTRY_EVOLUTION:
					case ENTRY_FRIEND:
					case ENTRY_WEAPON_LIB:
					case ENTRY_ARENA:
					case ENTRY_DAILY_TASK:
					case ENTRY_SOCIAL:
					case ENTRY_CHALLENGE:
					case ENTRY_EQUIP_CULTURE:
						
						break;
					default:;
				}

				P_ENTRY_CTRL->enter(uEntryID);
			} else if(m_pFuncBtn != NULL && std::strcmp(pszWidgetName, m_pFuncBtn->getName()) == 0) {
				m_pFuncBtn->setTouchEnabled(false);
				m_pBtnEffArmature->getAnimation()->play("fly03", -1, -1, 0);
			} else if(m_pBgMask != NULL && std::strcmp(pszWidgetName, m_pBgMask->getName()) == 0) {
				m_pBgMask->setTouchEnabled(false);
				m_pBtnEffArmature->getAnimation()->play("fly03", -1, -1, 0);
			}
			break;
		default:;
	}
}

void BottomEntryLayout::onUnfoldEntry(float dt) {
	GUIDE_SHOW(GUIDE_UI_CITY_SCREEN);
}

void BottomEntryLayout::onFoldEntry(float dt) {
	
}

void BottomEntryLayout::onBtnEffPlayEvent(CCArmature* pArmature, MovementEventType type, const char *pszActionName) {
	switch(type) {
		case COMPLETE:
			if(std::strcmp(pszActionName, "fly01") == 0) {
				pArmature->getAnimation()->play("fly02", -1, -1, 1);
				m_pBgMask->addTouchEventListener(this, toucheventselector(BottomEntryLayout::touchEventHandler));
				m_pFuncBtn->setVisible(true);
				m_pFuncBtn->addTouchEventListener(this, toucheventselector(BottomEntryLayout::touchEventHandler));
				m_pFuncWordsTxt->setVisible(true);
				/*std::string url = SOUND_RES_PATH + "module_open.ogg";
				AudioManager::playEffect(url.c_str());*/
			}else if(std::strcmp(pszActionName, "fly03") == 0) {
				std::string strWidgetName = m_pFuncBtn->getName();
				unsigned int uEntryID = std::atol(strWidgetName.substr(std::string("func").size(), strWidgetName.size()).c_str());
				m_pBgMask->removeFromParent();
				m_pBgMask = NULL;
				m_pFuncBtn->removeFromParent();
				m_pFuncBtn = NULL;
				m_pFuncWordsTxt->removeFromParent();
				m_pFuncWordsTxt = NULL;
				updateEntry(); // 更新入口

				CCPoint startPos = m_pBtnEffArmature->getPosition();
				CCPoint endPos = CCPointZero;

				if(uEntryID == ENTRY_LOTTERY || uEntryID == ENTRY_WEAPON_BOOK) {
					/*HeadLayout *pHeadLayout = P_CITY_CTRL->getHeadLayout();
					CCPoint pos = CCPointZero;
					if(uEntryID == ENTRY_LOTTERY) {
						pos = pHeadLayout->convertToWorldSpace(pHeadLayout->getLotteryBtn()->getPosition());
					} else if(uEntryID == ENTRY_WEAPON_BOOK) {
						pos = pHeadLayout->convertToWorldSpace(pHeadLayout->getQianghunBtn()->getPosition());
					}
					
					endPos = convertToNodeSpace(pos);*/
				} else {
					if(m_bUnfolded) {
						endPos = calcEntryPos(uEntryID);
						unfoldEntry();
					} else {
						endPos = m_pUnfoldBtn->getPosition();
					}
				}

				float angle = (endPos - startPos).getAngle();
				angle = atan2f(endPos.y - startPos.y, endPos.x - startPos.x);
				m_pBtnEffArmature->setRotation(-angle * 180 / M_PI - 90);

				CCActionInterval *pEaseMove = CCEaseIn::create(CCMoveTo::create(0.2f, endPos), 1.6f);
				CCActionInstant *pFunc = CCCallFunc::create(this, callfunc_selector(BottomEntryLayout::flyArrived));

				m_pBtnEffArmature->runAction(CCSequence::create(pEaseMove, pFunc, NULL));

			} else if(std::strcmp(pszActionName, "fly04") == 0) {
				if(pArmature->getParent() != NULL) {
					pArmature->removeFromParent();
                }
                CToolUtils::removeArmatureFile("effect/ut00011/ut00011.ExportJson");
			}
			break;
		default:;
	}
}

void BottomEntryLayout::onBtnEffFrameEvent(CCBone *pBone, const char *pszFrameName, int a, int b) {
	if(std::strcmp(pszFrameName, "icon_appear") == 0) {

	} else if(std::strcmp(pszFrameName, "icon_disappear") == 0) {

	}
}

void BottomEntryLayout::flyArrived() {
	m_pBtnEffArmature->getAnimation()->play("fly04", -1, -1, 0);

	updateEntry();
	unfoldEntry();

	GUIDE_TRIGGER(GUIDE_TRIGGER_MODULE, m_uNowModuleID);
	
}

std::string BottomEntryLayout::getModuleText(unsigned int uModuleID) {
	std::string text = "";

	switch(uModuleID) {
		case vmsg::MODULE_ID_BAG: // 背包
			text = TextStr(TEXT_MODULE_OPEN_BAG);
			break;
		case vmsg::MODULE_ID_WEAPON_LVUP: // 枪械强化
			text = TextStr(TEXT_MODULE_OPEN_WEAPON_LEVELUP);
			break;
		case vmsg::MODULE_ID_WEAPON_STARSTEP: // 枪械升星阶
			text = TextStr(TEXT_MODULE_OPEN_WEAPON_STARSTEP);
			break;
		case vmsg::MODULE_ID_EQUIP_REFINE: // 装备强化
			text = TextStr(TEXT_MODULE_OPEN_EQUIP_LEVELUP);
			break;
		case vmsg::MODULE_ID_GUILD: // 部落
			text = TextStr(TEXT_MODULE_OPEN_GUILD);
			break;
		case vmsg::MODULE_ID_ATTRSTRENGTHEN: // 属性强化
			text = TextStr(TEXT_MODULE_OPEN_ATTR_STRENGTHEN);
			break;
		case vmsg::MODULE_ID_EVOLUTION: // 进化
			text = TextStr(TEXT_MODULE_OPEN_EVOLUTION);
			break;
		case vmsg::MODULE_ID_FRIEND: // 好友
			text = TextStr(TEXT_MODULE_OPEN_FRIENDS);
			break;
		case vmsg::MODULE_ID_INFINIT: // 无尽模式
			text = TextStr(TEXT_MODULE_OPEN_INFINITE);
			break;
		case vmsg::MODULE_ID_LOTTERY: // 探秘
			text = TextStr(TEXT_MODULE_OPEN_LOTTERY);
			break;
		case vmsg::MODULE_ID_EVOLUTIONSKILL: // 设置进化技
			text = TextStr(TEXT_MODULE_OPEN_EVOLUTION_SKILL);
			break;
		case vmsg::MODULE_ID_MAIL: // 邮件
			text = TextStr(TEXT_MODULE_OPEN_MAIL);
			break;
		default:
			break;
	}

	return text;
}

void BottomEntryLayout::onBtnHide(CCNode *pNode) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pNode);

	if(pWidget != NULL) {
		pWidget->setTouchEnabled(false);
	}
}
