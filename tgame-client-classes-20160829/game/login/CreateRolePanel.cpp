#include "CreateRolePanel.h"

#include "SimpleAudioEngine.h"

#include "Resources.h"

#include "CfgMaster/HeroCfgMaster.h"
#include "CfgMaster/PlayerInitCfgMaster.h"
#include "CfgMaster/NameCfgMaster.h"

#include "basic/NounHelper.h"
#include "basic/CommonDialog.h"
#include "basic/EllipseMove.h"

#include "game/Constant.h"
#include "control/AudioManager.h"

#include "LoginControl.h"
#include "UFOCell.h"
#include "LoginObj.h"
#include "evtlog/EvtLog.h"

#define MIN_SCROLL_DISTANCE 16.0f
#define MAX_SHOW_ROLES 3 // 椭圆轨道上最多同时显示多少个角色
#define ELLIPSE_RADIUS_X 300 // x轴长
#define ELLIPSE_RADIUS_Y 20 // y轴长
#define LOOP_ROTATION_OPEN 1 // 循环旋转

CreateRolePanel::CreateRolePanel()
:m_pBgArmture(NULL)
,m_pFrontBgArmture(NULL)
,m_pBgImg(NULL)
,m_pLightingMaskImg(NULL)
,m_pRoleLightingImg(NULL)
,m_pRolesScrollView(NULL)
,m_pRoleNameTxt(NULL)
,m_pRoleIntroTxt(NULL)
,m_pRoleNameInputTxt(NULL)
,m_pDiceBtn(NULL)
,m_pCreateBtn(NULL)
,m_pLeftFlipBtn(NULL)
,m_pRightFlipBtn(NULL)
,m_pTipTxt(NULL)
,m_nRoleSelectedIdx(-1)
,m_nScrollDirection(0)
,m_pScreenParticleEff(NULL)
{
}

CreateRolePanel::~CreateRolePanel()
{
	for(unsigned int i = 0; i < m_stAircrafts.size(); i++) {
		CC_SAFE_RELEASE(m_stAircrafts[i]);
	}

	m_stAircrafts.clear();

}

CreateRolePanel* CreateRolePanel::create() {
	CreateRolePanel *pRet = new CreateRolePanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CreateRolePanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		m_stAircrafts.clear();
		setUseBackForClose(true);

		bRet = true;
	} while(0);

	return bRet;
}

void CreateRolePanel::open() {
	BasePanel::open();

	if(!isUIInited()) {
		initUI();
	}
    /*
    if(!P_LOGIN_OBJ->getIsStartMovieDone()) {
        ADD_EVT_LOG(EVT_LOG_PLAY_START_ANI); //游戏事件日志
        P_MOVIE_CTRL->playMovie(this, onMovieDone_selector(CreateRolePanel::doAfterMovieDone));
    } else {
        doAfterMovieDone();
    }*/
    doAfterMovieDone();
}

void CreateRolePanel::doAfterMovieDone()  {
    ADD_EVT_LOG(EVT_LOG_END_START_ANI); //游戏事件日志
    if(!P_LOGIN_OBJ->getIsStartMovieDone()) {
        P_LOGIN_OBJ->setStartMovieDone();
    }

    if(m_pScreenParticleEff == NULL) {
        CCDirector *pDirector = CCDirector::sharedDirector();
        CCPoint screenOrigin = pDirector->getVisibleOrigin();
        CCSize screenSize = pDirector->getVisibleSize();

        m_pScreenParticleEff = CCParticleSystemQuad::create("effect/ut00026/ut00026_lizi_quan.plist");
        m_pScreenParticleEff->setPosition(ccp(screenOrigin.x + 40, screenOrigin.y + screenSize.height / 2));
        m_pRootLayout->addNode(m_pScreenParticleEff);
    }
}

void CreateRolePanel::close() {
	BasePanel::close();
    if(m_pScreenParticleEff) {
        m_pScreenParticleEff->removeFromParent();
    }
	
	m_pScreenParticleEff = NULL;

}

void CreateRolePanel::initUI() {
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();
	CCPoint centerPos = ccp(screenOrigin.x + screenSize.width / 2, screenOrigin.y + screenSize.height / 2);

	float posX = 0.0f;
	float posY = 0.0f;
	float scaleX = 0.0f;
	float scaleY = 0.0f;
	float scale = 0.0f;

	m_pBgArmture = CCArmature::create("denglujiemian");
	m_pBgArmture->setPosition(convertToNodeSpace(centerPos));
	scaleX = screenSize.width / 1280;
	scaleY = screenSize.height / 720;
	scale = scaleX > scaleY ? scaleX : scaleY;
	m_pBgArmture->setScale(scale);
	addNode(m_pBgArmture);
	m_pBgArmture->getAnimation()->play("beijing", -1, -1, 1);

	m_pFrontBgArmture = CCArmature::create("denglujiemianqianjin");
	m_pFrontBgArmture->setPosition(convertToNodeSpace(centerPos));
	addNode(m_pFrontBgArmture);
	m_pFrontBgArmture->getAnimation()->play("qian", -1, -1, 1);

	initLayoutFromJsonFile("ui_v2/CreateRolePanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pLightingMaskImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("lighting_mask_img"));
	scaleX = screenSize.width / m_pLightingMaskImg->getSize().width;
	scaleY = screenSize.height / m_pLightingMaskImg->getSize().height;
	scale = scaleX > scaleY ? scaleX : scaleY;
	m_pLightingMaskImg->setScale(scale);
	m_pLightingMaskImg->setPosition(m_pRootLayout->convertToNodeSpace(centerPos));

	m_pRoleLightingImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("role_lighting_img"));
	posX = screenOrigin.x + screenSize.width / 2;
	posY = screenOrigin.y + screenSize.height - m_pRoleLightingImg->getSize().height / 2;
	m_pRoleLightingImg->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));
	m_pRoleLightingImg->setVisible(false);

	m_pRolesScrollView = dynamic_cast<UIScrollView*>(m_pRootLayout->getChildByName("roles_scroll"));
	m_pRolesScrollView->addEventListenerScrollView(this, scrollvieweventselector(CreateRolePanel::scrollViewEventHandler));
	m_pRolesScrollView->addTouchEventListener(this, toucheventselector(CreateRolePanel::touchEventHandler));

	UILayout *pRoleIntroLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("role_intro_layout"));
	
	m_pRoleNameTxt = dynamic_cast<UILabel*>(pRoleIntroLayout->getChildByName("role_name_txt"));

	m_pRoleIntroTxt = dynamic_cast<UILabel*>(pRoleIntroLayout->getChildByName("role_intro_txt"));

	UILayout *pEnterLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("enter_layout"));
	
	m_pRoleNameInputTxt = dynamic_cast<UITextField*>(pEnterLayout->getChildByName("role_name_input_txt"));

	m_pDiceBtn = dynamic_cast<UIButton*>(pEnterLayout->getChildByName("dice_btn"));
	m_pDiceBtn->addTouchEventListener(this, toucheventselector(CreateRolePanel::touchEventHandler));

	m_pCreateBtn = dynamic_cast<UIButton*>(pEnterLayout->getChildByName("create_btn"));
	m_pCreateBtn->addTouchEventListener(this, toucheventselector(CreateRolePanel::touchEventHandler));

	m_pLeftFlipBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("left_flip_btn"));
	m_pLeftFlipBtn->addTouchEventListener(this, toucheventselector(CreateRolePanel::touchEventHandler));
	
	m_pRightFlipBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("right_flip_btn"));
	m_pRightFlipBtn->addTouchEventListener(this, toucheventselector(CreateRolePanel::touchEventHandler));
	
	m_pTipTxt = dynamic_cast<UILabel*>(pEnterLayout->getChildByName("tip_txt"));
	m_pTipTxt->setVisible(false);

	const PlayerInitCfgSet &stPlayerInitCfgSet = PLAYER_INIT_CFG->GetCfg();
	int nTotalRoles = stPlayerInitCfgSet.playerinitcfg_size();

	int nAircraftNum = nTotalRoles <= MAX_SHOW_ROLES ? nTotalRoles : MAX_SHOW_ROLES + 1;

	for(int i = 0; i < nAircraftNum; i++) {
		UFOCell *pAircraft = UFOCell::create();
		pushAircraft(pAircraft);
	}

	int nShowNum = nTotalRoles < MAX_SHOW_ROLES ? nTotalRoles : MAX_SHOW_ROLES;
	m_nRoleSelectedIdx = 0;

	if(nTotalRoles < MAX_SHOW_ROLES || LOOP_ROTATION_OPEN == 0) {
		m_pLeftFlipBtn->setTouchEnabled(m_nRoleSelectedIdx > 0);
		m_pRightFlipBtn->setTouchEnabled(m_nRoleSelectedIdx < nShowNum - 1);
	} else {
		m_pLeftFlipBtn->setTouchEnabled(true);
		m_pRightFlipBtn->setTouchEnabled(true);
	}

	m_pLeftFlipBtn->setBright(m_pLeftFlipBtn->isTouchEnabled());
	m_pRightFlipBtn->setBright(m_pRightFlipBtn->isTouchEnabled());
	
	CCSize viewSize = m_pRolesScrollView->getSize();
	float startRadian = (float)M_PI * 270 / 180;
	float deltaRadian = 2 * (float)M_PI / MAX_SHOW_ROLES;

	for(int i = 0; i < nShowNum; i++) {
		int idx = 0;
		
		if(nTotalRoles > MAX_SHOW_ROLES) {
			if(i == nShowNum - 1) {
				idx = m_nRoleSelectedIdx - 1 >= 0 ? m_nRoleSelectedIdx - 1 : m_nRoleSelectedIdx - 1 + nTotalRoles;
			} else {
				idx = i;
			}
		} else {
			idx = i;
		}

		UFOCell *pAircraft = popAircraft();
		pAircraft->updateRole(stPlayerInitCfgSet.playerinitcfg(idx).playerid());

		float x = ELLIPSE_RADIUS_X * cos(startRadian);
		float y = ELLIPSE_RADIUS_Y * sin(startRadian);
		pAircraft->setPosition(ccpAdd(ccp(viewSize.width / 2, viewSize.height / 2), ccp(x, y)));
		float scale = 0.4f - 0.6f * (y - ELLIPSE_RADIUS_Y) / (2 * ELLIPSE_RADIUS_Y);
		pAircraft->setScale(scale);
		pAircraft->setZOrder(ELLIPSE_RADIUS_Y - y);
		m_pRolesScrollView->addChild(pAircraft);
		m_stAircraftShowLst.push_back(pAircraft);

		if(i == 0) {
			onSelected(pAircraft);
		}

		startRadian += deltaRadian;
	}

	m_pRoleNameInputTxt->setText(NAME_CFG_MASTER->GetRandName());

	m_bUIInited = true;
}

void CreateRolePanel::prevRole() {
	const PlayerInitCfgSet &stPlayerInitCfgSet = PLAYER_INIT_CFG->GetCfg();
	int nTotalRoles = stPlayerInitCfgSet.playerinitcfg_size();

	if(nTotalRoles < MAX_SHOW_ROLES || LOOP_ROTATION_OPEN == 0) {
		if(m_nRoleSelectedIdx > 0) {
			rotateEllipseTrack(false);
			m_nRoleSelectedIdx--;
		}

		m_pLeftFlipBtn->setTouchEnabled(m_nRoleSelectedIdx > 0);
		m_pRightFlipBtn->setTouchEnabled(m_nRoleSelectedIdx < nTotalRoles - 1);
	} else {
		if(m_nRoleSelectedIdx > 0) {
			rotateEllipseTrack(false);
			m_nRoleSelectedIdx--;
		} else {
			rotateEllipseTrack(false);
			m_nRoleSelectedIdx = nTotalRoles - 1;
		}

		m_pLeftFlipBtn->setTouchEnabled(true);
		m_pRightFlipBtn->setTouchEnabled(true);
	}

	m_pLeftFlipBtn->setBright(m_pLeftFlipBtn->isTouchEnabled());
	m_pRightFlipBtn->setBright(m_pRightFlipBtn->isTouchEnabled());
}

void CreateRolePanel::nextRole() {
	const PlayerInitCfgSet &stPlayerInitCfgSet = PLAYER_INIT_CFG->GetCfg();
	int nTotalRoles = stPlayerInitCfgSet.playerinitcfg_size();

	if(nTotalRoles < MAX_SHOW_ROLES || LOOP_ROTATION_OPEN == 0) {
		if(m_nRoleSelectedIdx < nTotalRoles - 1) {
			rotateEllipseTrack(true);
			m_nRoleSelectedIdx++;
		}

		m_pLeftFlipBtn->setTouchEnabled(m_nRoleSelectedIdx > 0);
		m_pRightFlipBtn->setTouchEnabled(m_nRoleSelectedIdx < nTotalRoles - 1);
	} else {
		if(m_nRoleSelectedIdx < nTotalRoles - 1) {
			rotateEllipseTrack(true);
			m_nRoleSelectedIdx++;
		} else {
			rotateEllipseTrack(true);
			m_nRoleSelectedIdx = 0;
		}

		m_pLeftFlipBtn->setTouchEnabled(true);
		m_pRightFlipBtn->setTouchEnabled(true);
	}

	m_pLeftFlipBtn->setBright(m_pLeftFlipBtn->isTouchEnabled());
	m_pRightFlipBtn->setBright(m_pRightFlipBtn->isTouchEnabled());
}

void CreateRolePanel::showTip() {
	closeLoading();
	m_pTipTxt->setVisible(true);
	CCBlink* blink = CCBlink::create(1, 3);

	if(m_pTipTxt->numberOfRunningActions() > 0){
		m_pTipTxt->stopAllActions();
	}

	m_pTipTxt->runAction(CCSequence::create(blink, NULL));
}

void CreateRolePanel::rotateEllipseTrack(bool clockwise) {
	const PlayerInitCfgSet &stPlayerInitCfgSet = PLAYER_INIT_CFG->GetCfg();
	int nTotalRoles = stPlayerInitCfgSet.playerinitcfg_size();
	const CCSize &viewSize = m_pRolesScrollView->getSize();
	CCPoint centerPos = ccp(viewSize.width / 2, viewSize.height / 2);
	float selectRadian = (float)M_PI * 270 / 180;
	float startRadian = selectRadian;
	float deltaRadian = 2 * (float)M_PI / MAX_SHOW_ROLES;

	list<UFOCell*>::iterator iter = m_stAircraftShowLst.begin();
	unsigned int uShowNum = m_stAircraftShowLst.size();
	unsigned int i = 0;

	while(iter != m_stAircraftShowLst.end()) {
		UFOCell *pAircraft = *iter;

		if(nTotalRoles > MAX_SHOW_ROLES) {
			if(clockwise && i == uShowNum - 1) {
				int idx = (m_nRoleSelectedIdx + 2) % nTotalRoles;
				unsigned int uPlayerID = stPlayerInitCfgSet.playerinitcfg(idx).playerid();
				pAircraft->updateRole(uPlayerID);
			} else if(!clockwise && i == uShowNum - 2) {
				int idx = m_nRoleSelectedIdx - 2 >= 0 ? m_nRoleSelectedIdx - 2 : m_nRoleSelectedIdx - 2 + nTotalRoles;
				unsigned int uPlayerID = stPlayerInitCfgSet.playerinitcfg(idx).playerid();
				pAircraft->updateRole(uPlayerID);
			}
		}
		
		EllipseInfo info;
		info.center = centerPos;
		info.a = ELLIPSE_RADIUS_X;
		info.b = ELLIPSE_RADIUS_Y;
		info.startRadian = startRadian;
		info.deltaRadian = deltaRadian;
		info.clockwise = clockwise;
		EllipseMove *pMove = EllipseMove::create(1.6f, info);
		CCCallFunc *pFunc = CCCallFunc::create(this, callfunc_selector(CreateRolePanel::onRotated));
		CCCallFuncN *pFuncSelected = CCCallFuncN::create(this, callfuncN_selector(CreateRolePanel::onSelected));
		CCCallFuncN *pFuncUnSelected = CCCallFuncN::create(this, callfuncN_selector(CreateRolePanel::onUnSelected));
		
		if(sin(startRadian) == sin(selectRadian)) {
			pAircraft->runAction(CCSequence::create(pFuncUnSelected, CCEaseBackOut::create(pMove), pFunc, NULL));
		} else if(clockwise && sin(startRadian - deltaRadian) == sin(selectRadian)) {
			pAircraft->runAction(CCSequence::create(pFuncSelected, CCEaseBackOut::create(pMove), pFunc, NULL));
		} else if(!clockwise && sin(startRadian + deltaRadian) == sin(selectRadian)) {
			pAircraft->runAction(CCSequence::create(pFuncSelected, CCEaseBackOut::create(pMove), pFunc, NULL));
		} else {
			pAircraft->runAction(CCSequence::create(CCEaseBackOut::create(pMove), pFunc, NULL));
		}

		if(nTotalRoles < MAX_SHOW_ROLES) {
			if(clockwise) {
				startRadian += deltaRadian;
			} else {
				startRadian -= deltaRadian;
			}
		} else {
			startRadian += deltaRadian;
		}
		
		i++;
		iter++;
	}

	// m_stAircraftShowLst.front() always selected
	if(clockwise) {
		UFOCell *pAircraft = m_stAircraftShowLst.front();
		m_stAircraftShowLst.push_back(pAircraft);
		m_stAircraftShowLst.pop_front();
	} else {
		UFOCell *pAircraft = m_stAircraftShowLst.back();
		m_stAircraftShowLst.push_front(pAircraft);
		m_stAircraftShowLst.pop_back();
	}
}

void CreateRolePanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, m_pDiceBtn->getName()) == 0) {
			m_pTipTxt->setVisible(false);
			m_pRoleNameInputTxt->setText(NAME_CFG_MASTER->GetRandName());
		} else if(std::strcmp(pszWidgetName, m_pCreateBtn->getName()) == 0) {
			checkName(m_pRoleNameInputTxt->getStringValue());
		} else if(std::strcmp(pszWidgetName, m_pLeftFlipBtn->getName()) == 0) {
			prevRole();
		} else if(std::strcmp(pszWidgetName, m_pRightFlipBtn->getName()) == 0) {
			nextRole();
		} else if(std::strcmp(pszWidgetName, m_pRolesScrollView->getName()) == 0) {
			const CCPoint &startPos = m_pRolesScrollView->getTouchStartPos();
			const CCPoint &endPos = m_pRolesScrollView->getTouchEndPos();

			if(startPos.getDistance(endPos) > MIN_SCROLL_DISTANCE) {
				if(m_nScrollDirection == -1) {
					prevRole();
					m_nScrollDirection = 0;
				} else if(m_nScrollDirection == 1) {
					nextRole();
					m_nScrollDirection = 0;
				} else {
					CCLOG("not scroll");
				}
			}
		}
		break;
	default:;
	}
}

void CreateRolePanel::scrollViewEventHandler(CCObject *pSender, ScrollviewEventType type) {
	switch(type) {
	case SCROLLVIEW_EVENT_SCROLL_TO_LEFT:
		m_nScrollDirection = -1;
		break;
	case SCROLLVIEW_EVENT_SCROLL_TO_RIGHT:
		m_nScrollDirection = 1;
		break;
	default:
		m_nScrollDirection = 0;
	}
}

void CreateRolePanel::textFieldEventHandler(CCObject *pSender, TextFiledEventType type) {
	UITextField* textField = dynamic_cast<UITextField*>(pSender);

	switch (type)
	{
	case TEXTFIELD_EVENT_ATTACH_WITH_IME:
		CCLog("CreateRolePanel::click the name txt");
		break;
	case TEXTFIELD_EVENT_DETACH_WITH_IME:
		CCLog("CreateRolePanel::move out the name txt");
		break;
	case TEXTFIELD_EVENT_INSERT_TEXT:
		CCLog("CreateRolePanel::insert the word");
		break;
	case TEXTFIELD_EVENT_DELETE_BACKWARD:
		CCLog("CreateRolePanel::delete the word");
		break;
	default:;
	}
}

void CreateRolePanel::checkName(std::string nameStr) {
	if(strcmp(nameStr.c_str(), "") == 0){	//名字不能为空
		COMMON_DIALOG->show(NounStr(NOUN_NAME_NOT_EMPTY), NULL, NounStr(OK));
		return;
	}

	if(nameStr.find_first_of(" ") != string::npos){			//名字不能有空格
		COMMON_DIALOG->show(NounStr(NOUN_NAME_NO_SPACE), NULL, NounStr(OK));
		return;
	}

	if(nameStr.size() > MAX_NAME_LEN){				//名字长度超出限制
		COMMON_DIALOG->show(NounStr(NOUN_NAME_TOO_LONG), NULL, NounStr(OK));
		return;
	}

	const PlayerInitCfgSet &stPlayerInitCfgSet = PLAYER_INIT_CFG->GetCfg();

	if(m_nRoleSelectedIdx >= 0 && m_nRoleSelectedIdx < stPlayerInitCfgSet.playerinitcfg_size()) {
		showLoading();
        unsigned int uPlayerID = stPlayerInitCfgSet.playerinitcfg(m_nRoleSelectedIdx).playerid();
		P_LOGIN_CTRL->getProxy()->sendRegisterRqst(uPlayerID, nameStr.c_str());
		P_LOGIN_OBJ->setRecentLoginRoleName(nameStr);
	}
}

void CreateRolePanel::pushAircraft(UFOCell *pAircraft) {
	CC_SAFE_RETAIN(pAircraft);
	m_stAircrafts.push_back(pAircraft);
}

UFOCell* CreateRolePanel::popAircraft() {
	UFOCell *pRet = NULL;

	if(m_stAircrafts.size() > 0) {
		pRet = m_stAircrafts.back();
		m_stAircrafts.pop_back();
		CC_SAFE_RELEASE(pRet);
	}

	return pRet;
}

void CreateRolePanel::removeCellFromScroll(CCNode *pUFOCell) {
	UFOCell *pCell = dynamic_cast<UFOCell*>(pUFOCell);
	pushAircraft(pCell);
	pCell->removeFromParent();
}

void CreateRolePanel::onSelected(CCNode *pNode) {
	UFOCell *pAircraft = dynamic_cast<UFOCell*>(pNode);
	pAircraft->selected(true);

	unsigned int uPlayerID = pAircraft->getPlayerID();
	const HeroCfg *pHeroCfg = HERO_CFG_MASTER->GetHeroCfgByID(PLAYER_INIT_CFG->GetPlayerCfgByID(uPlayerID)->showid());
	m_pRoleNameTxt->setText(pHeroCfg->rolename());
	m_pRoleIntroTxt->setText(pHeroCfg->roleintro());

    if(!P_LOGIN_OBJ->getIsStartMovieDone()) {
        return;
    }
	std::string url = SOUND_RES_PATH + pHeroCfg->wordsvoice();
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(url.c_str());
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(url.c_str());
	AudioManager::stopAllEffects();
	AudioManager::preloadEffect(url.c_str());
	AudioManager::playEffect(url.c_str());
}

void CreateRolePanel::onUnSelected(CCNode *pNode) {
	UFOCell *pAircraft = dynamic_cast<UFOCell*>(pNode);
	pAircraft->selected(false);
}

void CreateRolePanel::onRotated() {
	// 每一次旋转会调用多次
}
