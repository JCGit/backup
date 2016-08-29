#include "BasePanel.h"
#include "LoadingLayer.h"
#include "BaseScene.h"
#include "Multiresolution.h"
#include "control/PanelSoundType.h"
#include "control/AudioManager.h"

#include "basic/UIBoxLayer.h"

#include "PanelSoundCfgMaster.h"
#include "PanelSeqMgr.h"

using namespace std;

CLoadingLayer* BasePanel::m_pLoadingLayer = NULL;

#define MONEY_BAR_HEIGHT 20

BasePanel::BasePanel()
:m_bIsOpen(false)
,m_uPopType(POP_CENTER)
,m_bIsInited(false) 
,m_bIsSwallowsTouches(false)
,m_bIsInLoading(false)
,m_bClickOutToClose(false)
,m_bUseBackForClose(false)
,m_bOpenWithBgImg(false)
,m_pLinkedPanel(NULL)
,m_uSoundType(COMMON_PANEL_TYPE)
,m_bShowMoneyTopBar(false)
{
    
}

BasePanel::~BasePanel()
{
    PANEL_SEQ_MGR->delInStack(this);
}

bool BasePanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		setBackGroundColorType(LAYOUT_COLOR_SOLID);
		setBackGroundColorOpacity(0x40);
		setTouchEnabled(true); // 保证接受touch事件
		addTouchEventListener(this, toucheventselector(BasePanel::touchEventHandler));

		setClickOutToClose(true);

        m_bIsInited = true;

		bRet = true;
	} while(0);

	return bRet;
}

std::string BasePanel::getDescription() const {
	return "BasePanel";
}

void BasePanel::onEnterTransitionDidFinish() {
	BaseLayout::onEnterTransitionDidFinish();

	m_bIsOpen = true;
}

void BasePanel::onExit() {
	BaseLayout::onExit();

	m_bIsOpen = false;
}

void BasePanel::initLayoutFromJsonFile(const char *pszFileName) {
	BaseLayout::initLayoutFromJsonFile(pszFileName);

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();
	setSize(screenSize); // 默认满屏
	//setPosition(convertToNodeSpace(screenOrigin)); // 默认添加到屏幕左下角

	setPopType(m_uPopType); // 设置root layout到中心
}

void BasePanel::refresh() {

}

bool BasePanel::isOpen() {
	return m_bIsOpen;
}


void BasePanel::closeOtherPanel() {
    if(isOpen()) {
        return; //否则死循环了。。。
    }
    //有时序问题,removeAllBoxes没有调用close： 打开背包寻路到副本选择处，然后关闭副本选择再开背包就down了
    BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    if(NULL == pScene) {
        CCLOG("[error]BasePanel::closeOtherPanel fail,running scene is null.panel=%s", typeid(*this).name());
        return;
    }
    UIBoxLayer *pUILayer = dynamic_cast<UIBoxLayer*>(pScene->getLayer(LAYER_ID_UI));
    if(NULL == pUILayer) {
        CCLOG("[error]BasePanel::closeOtherPanel fail,LAYER_ID_UI is null.panel=%s", typeid(*this).name());
        return;
    }
    pUILayer->removeAllBoxes();
    
}

bool BasePanel::isPanelOnTop() {
    if(!this->isOpen()){
        return false;
    }
    BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    if(NULL == pScene) {
        CCLOG("[error]BasePanel::isPanelOnTop fail,running scene is null.panel=%s", typeid(*this).name());
        return false;
    }
    UIBoxLayer *pUILayer = dynamic_cast<UIBoxLayer*>(pScene->getLayer(LAYER_ID_UI));
    if(NULL == pUILayer) {
        CCLOG("[error]BasePanel::isPanelOnTop fail,LAYER_ID_UI is null.panel=%s", typeid(*this).name());
        return false;
    }
    return pUILayer->isBoxOnTop(this);
}

void BasePanel::open() {
	if(!m_bIsOpen) {
		BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
        if(NULL == pScene) {
            CCLOG("[error]BasePanel::open fail,running scene is null.panel=%s", typeid(*this).name());
            return;
        }
		UIBoxLayer *pUILayer = dynamic_cast<UIBoxLayer*>(pScene->getLayer(LAYER_ID_UI));
        if(NULL == pUILayer) {
            CCLOG("[error]BasePanel::open fail,LAYER_ID_UI is null.panel=%s", typeid(*this).name());
            return;
        }
		pUILayer->addBox(this);
		//pUILayer->addWidget(this);
		m_bIsOpen = true;

		//面板打开音效
		if(m_uSoundType != NONE_PANEL_TYPE){
			string stOpenSound = PANEL_SOUND_CFG->getPanelOpenSound(m_uSoundType);
			if (stOpenSound.length() && strcmp(stOpenSound.c_str(), "null")){
				/*string fullPath = SOUND_RES_PATH + stOpenSound;
				CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(fullPath.c_str());*/
				AudioManager::playEffect(stOpenSound.c_str());
			}
		}

		/*CCPoint pos = m_pRootLayout->getPosition();
		CCSize size = m_pRootLayout->getSize();
		m_pRootLayout->setOpacity(0x00);
		m_pRootLayout->setPosition(pos + ccp(size.width * 0.2f, size.height * 0.2f) / 2);
		m_pRootLayout->setScale(0.8f, 0.8f);
		CCActionInterval *pMoveAction = CCEaseBackOut::create(CCMoveTo::create(0.4f, pos));
		CCActionInterval *pScaleAction = CCEaseBackOut::create(CCScaleTo::create(0.4f, 1.0f, 1.0f));
		CCActionInterval *pAlphaAction = CCEaseBackOut::create(CCFadeIn::create(0.4f));
		CCActionInterval *pAction = CCSpawn::create(pMoveAction, pScaleAction, pAlphaAction, NULL);

		m_pRootLayout->runAction(pAction);*/

		//强制吞噬事件
		CCTouchDispatcher *pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		m_bIsSwallowsTouches = dynamic_cast<CCTargetedTouchHandler*>(pDispatcher->findHandler(pUILayer))->isSwallowsTouches();
		dynamic_cast<CCTargetedTouchHandler*>(pDispatcher->findHandler(pUILayer))->setSwallowsTouches(true);
	}
}

void BasePanel::close() {
	if(!m_bIsOpen) {
		return;
	}
    BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    if(NULL == pScene) {
        CCLOG("[error]BasePanel::close fail,running scene is null.panel=%s", typeid(*this).name());
        return;
    }
	UIBoxLayer *pUILayer = dynamic_cast<UIBoxLayer*>(pScene->getLayer(LAYER_ID_UI));
    if(NULL == pUILayer) {
        CCLOG("[error]BasePanel::close fail,LAYER_ID_UI is null.panel=%s", typeid(*this).name());
        return;
    }

	/*CCPoint pos = m_pRootLayout->getPosition();
	CCSize size = m_pRootLayout->getSize();
	m_pRootLayout->setOpacity(0xFF);
	m_pRootLayout->setScale(1.0f, 1.0f);
	CCActionInterval *pMoveAction = CCEaseBackIn::create(CCMoveTo::create(0.4f, pos + ccp(size.width * 0.2f, size.height * 0.2f) / 2));
	CCActionInterval *pScaleAction = CCEaseBackIn::create(CCScaleTo::create(0.4f, 0.8f, 0.8f));
	CCActionInterval *pAlphaAction = CCEaseBackIn::create(CCFadeOut::create(0.4f));
	CCActionInstant *pFuncAction = CCCallFunc::create(this, callfunc_selector(BasePanel::onClose));
	CCActionInterval *pAction = CCSpawn::create(pMoveAction, pScaleAction, pAlphaAction, NULL);

	m_pRootLayout->runAction(CCSequence::create(pAction, pFuncAction, NULL));*/

	//面板关闭音效
	if(m_uSoundType != NONE_PANEL_TYPE){
		string stCloseSound = PANEL_SOUND_CFG->getPanelCloseSound(m_uSoundType);
		if (stCloseSound.length() && strcmp(stCloseSound.c_str(), "null")){
			/*string fullPath = SOUND_RES_PATH + stCloseSound;
			CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(fullPath.c_str());*/
			AudioManager::playEffect(stCloseSound.c_str());
		}
	}

	pUILayer->removeBox(this);
	m_bIsOpen = false;
    if(this->getParent()) {
        this->removeFromParent();
    }

	//将UI层的吞噬事件机制改为原来的
	CCTouchDispatcher *pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	dynamic_cast<CCTargetedTouchHandler*>(pDispatcher->findHandler(pUILayer))->setSwallowsTouches(m_bIsSwallowsTouches);

    //检查是否有需要返回的面板
    BasePanel* pStackTopPanel = PANEL_SEQ_MGR->getLast();
    if(this == pStackTopPanel) {
        PANEL_SEQ_MGR->popBack();

        //next
        pStackTopPanel = PANEL_SEQ_MGR->getLast();
        if(pStackTopPanel) {
            pStackTopPanel->open();
        }
    }
    
}

void BasePanel::doGlobalBackClose() {
    close();
}

bool BasePanel::isMeetAutoOpen() {
	return true;
}

void BasePanel::setPopType(unsigned int uPopType) {
	m_uPopType = uPopType;

	float posX = 0.0f;
	float posY = 0.0f;

	const CCSize &containerSize = getSize();
	const CCSize &panelSize = m_pRootLayout->getSize();

	if(panelSize.equals(containerSize)) { // 若大小与UI资源匹配
		m_uPopType = POP_CENTER;
		setBackGroundColorOpacity(0x00);
	}

	switch(m_uPopType) {
		case POP_LEFT:
			posX = 0.0f;
			posY = (containerSize.height - panelSize.height) / 2;
			break;
		case POP_CENTER:
			posX = 0.0f + (containerSize.width - panelSize.width) / 2;
			posY = (containerSize.height - panelSize.height) / 2;
			break;
		case POP_RIGHT:
			posX = containerSize.width - panelSize.width;
			posY = (containerSize.height - panelSize.height) / 2;
			break;
		default:;
	}

	if(m_bShowMoneyTopBar){
		posY -= MONEY_BAR_HEIGHT;
	}

	m_pRootLayout->setPosition(ccp(posX, posY));
}

bool BasePanel::isClickOutToClose() {
	return m_bClickOutToClose;
}

void BasePanel::setClickOutToClose(bool bClickOutToClose) {
	m_bClickOutToClose = bClickOutToClose;
}

bool BasePanel::isUseBackForClose() {
	return m_bUseBackForClose;
}

void BasePanel::setUseBackForClose(bool bUseBackForClose) {
	m_bUseBackForClose = bUseBackForClose;
}

bool BasePanel::isOpenWithBgImg() {
	return m_bOpenWithBgImg;
}

void BasePanel::setOpenWithBgImg(bool bOpenWithBgImg) {
	m_bOpenWithBgImg = bOpenWithBgImg;
}

BasePanel* BasePanel::getLinkedPanel() {
	return m_pLinkedPanel;
}

void BasePanel::setLinkedPanel(BasePanel *pLinkedPanel) {
	m_pLinkedPanel = pLinkedPanel;
}

void BasePanel::showLoading() {
	if(getIsInLoading()){
		return;
	}

    if(NULL == m_pLoadingLayer) {
        m_pLoadingLayer = CLoadingLayer::create();
        m_pLoadingLayer->retain();
        //m_pLoadingLayer->AddEventListener(this, REVIVE_DONE, eventListner_selector(GuardActScene::onReviveDone));
    }

    m_pLoadingLayer->showLoading();
}

void BasePanel::showLoading(CCObject* target, SEL_CallFunc callbackFun){
	if(getIsInLoading()){
		return;
	}

	if(NULL == m_pLoadingLayer) {
		m_pLoadingLayer = CLoadingLayer::create();
		m_pLoadingLayer->retain();
	}

	m_pLoadingLayer->showLoading(target, callbackFun);
}

void BasePanel::closeLoading() {
	if(!getIsInLoading()){
		return;
	}

    if(m_pLoadingLayer) {
        m_pLoadingLayer->doLoadingDone();
    }
}

bool BasePanel::getIsInLoading() const{
	if(NULL == m_pLoadingLayer){
		return false;
	}

	return m_pLoadingLayer->getIsInLoading();
}

void BasePanel::onClose() {
	BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
	if(NULL == pScene) {
		CCLOG("[error]BasePanel::close fail,running scene is null.panel=%s", typeid(*this).name());
		return;
	}
	UIBoxLayer *pUILayer = dynamic_cast<UIBoxLayer*>(pScene->getLayer(LAYER_ID_UI));
	if(NULL == pUILayer) {
		CCLOG("[error]BasePanel::close fail,LAYER_ID_UI is null.panel=%s", typeid(*this).name());
		return;
	}

	if(getParent() != NULL) {
		pUILayer->removeBox(this);
	}
	
}

void BasePanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, getName()) == 0) {
				bool bClickOut = false;
				const CCPoint &startPos = getTouchStartPos();
				const CCPoint &endPos = getTouchEndPos();
				const CCSize &uiSize = m_pRootLayout->getSize();
				CCPoint screenOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
				CCPoint stPos = screenOrigin + ccp(m_pRootLayout->getPositionX(), m_pRootLayout->getPositionY());
				CCRect uiRect = CCRectMake(stPos.x, stPos.y, uiSize.width, uiSize.height);
				bClickOut = !uiRect.containsPoint(startPos) && !uiRect.containsPoint(endPos);

				if(m_bClickOutToClose && bClickOut) {
					close();
				}
			}
			break;
		default:;
	}
}


bool BasePanel::isShowMoneyTopBar()  {
    return m_bShowMoneyTopBar;
}
void BasePanel::setShowMoneyTopBar(bool bIsShow) {
	if(m_bShowMoneyTopBar != bIsShow && m_pRootLayout){
		float posY = m_pRootLayout->getPositionY();
		if(m_bShowMoneyTopBar){
			m_pRootLayout->setPositionY(posY + MONEY_BAR_HEIGHT);
		}else{
			m_pRootLayout->setPositionY(posY - MONEY_BAR_HEIGHT);
		}
	}

    m_bShowMoneyTopBar = bIsShow;
}

void BasePanel::openInSeq(BasePanel* pBackPanel) {
    if(pBackPanel) {
        PANEL_SEQ_MGR->pushBack(pBackPanel);
        PANEL_SEQ_MGR->pushBack(this);
        pBackPanel->close();
    }
    open();
}
