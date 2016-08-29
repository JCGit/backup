#include "ChooseRolePanel.h"
#include "basic/BasicTypes.h"
#include "RoleCard.h"
#include "UFOCell.h"
#include "LoginObj.h"
#include "LoginControl.h"

#include "game/role/MainHero.h"
#include "Resources.h"
#include "basic/EllipseMove.h"

#define MAX_ROLE_NUM 3
#define ROLE_CARD_TAG_BASE 56 //tag id base

ChooseRolePanel::ChooseRolePanel()
:m_pBgArmture(NULL)
,m_pFrontBgArmture(NULL)
,m_pBgImg(NULL)
,m_pLightingMaskImg(NULL)
,m_pRoleLightingImg(NULL)
,m_pSvrNameValTxt(NULL)
,m_pRoleNameTxt(NULL)
,m_pEnterBtn(NULL)
,m_pRolesLayout(NULL)
,m_pRolesListView(NULL)
,m_nRoleSelectedTag(ROLE_CARD_TAG_BASE)
,m_pAircraftBody(NULL)
,m_pAircraftArmture(NULL)
,m_pAircraftImg(NULL)
,m_pRole(NULL)
,m_nRoleSelectedIdx(0)
{
}

ChooseRolePanel::~ChooseRolePanel()
{
	
}

ChooseRolePanel* ChooseRolePanel::create() {
	ChooseRolePanel *pRet = new ChooseRolePanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ChooseRolePanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setUseBackForClose(true);

		bRet = true;
	} while(0);

	return bRet;
}

void ChooseRolePanel::open() {
	BasePanel::open();
    
	if(!isUIInited()) {
		initUI();
	}

	CCPoint pos = m_pAircraftBody->getPosition();
	CCActionInterval *pMove = CCMoveTo::create(1.0f, ccp(pos.x, pos.y + 8));
	CCActionInterval *pMoveBack = CCMoveTo::create(1.0f, ccp(pos.x, pos.y));
	CCAction *pAction = CCRepeatForever::create(CCSequence::create(pMove, pMoveBack, NULL));

	if(m_pAircraftBody->numberOfRunningActions() > 0) {
		m_pAircraftBody->stopAllActions();
	}

	m_pAircraftBody->runAction(pAction);

	if(m_pRolesLayout != NULL) {
		CCActionInterval *pCameraAction = CCOrbitCamera::create(0.6f, 1, 0, 90, -72, 0, 0);
		m_pRolesLayout->runAction(CCEaseBackOut::create(pCameraAction));
	}
}

void ChooseRolePanel::close() {
	BasePanel::close();

}

void ChooseRolePanel::onExit() {
	BasePanel::onExit();

	if(m_pRole && m_pRole->getParent()){
		m_pRole->removeFromParent();
		m_pRole = NULL;
	}
}

void ChooseRolePanel::initUI() {
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

	initLayoutFromJsonFile("ui_v2/ChooseRolePanelUI.ExportJson");
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

	UILayout *pSvrLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("svr_layout"));
	posX = screenOrigin.x + screenSize.width - pSvrLayout->getSize().width - 10;
	posY = screenOrigin.y + screenSize.height - pSvrLayout->getSize().height - 10;
	pSvrLayout->setPosition(m_pRootLayout->convertToNodeSpace(ccp(posX, posY)));

	m_pSvrNameValTxt = dynamic_cast<UILabel*>(pSvrLayout->getChildByName("svr_name_val_txt"));

	UILayout *pEnterLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("enter_layout"));
	
	m_pRoleNameTxt = dynamic_cast<UILabel*>(pEnterLayout->getChildByName("role_name_txt"));

	m_pEnterBtn = dynamic_cast<UIButton*>(pEnterLayout->getChildByName("enter_btn"));
	m_pEnterBtn->addTouchEventListener(this, toucheventselector(ChooseRolePanel::touchEventHandler));

	m_pRolesLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("roles_layout"));
	
	m_pRolesListView = dynamic_cast<UIListView*>(m_pRolesLayout->getChildByName("roles_list"));
	m_pRolesListView->addEventListenerListView(this, listvieweventselector(ChooseRolePanel::listViewEventHandler));
	m_pRolesListView->setPositionX(m_pRolesListView->getPositionX() + 408);

	m_pAircraftBody = CCNode::create();
	m_pAircraftBody->setPosition(m_pRootLayout->convertToNodeSpace(centerPos));
	m_pRootLayout->addNode(m_pAircraftBody);

	/*m_pAircraftArmture = CCArmature::create("ut00026");
	m_pAircraftBody->addChild(m_pAircraftArmture);
	m_pAircraftArmture->getAnimation()->play("fly", -1, -1, 1);*/

	m_pAircraftImg = UIImageView::create();
	m_pAircraftBody->addChild(m_pAircraftImg);
	m_pAircraftImg->loadTexture("start/ufo.png", UI_TEX_TYPE_PLIST);

	m_bUIInited = true;
}

void ChooseRolePanel::updateUI() {
	m_pSvrNameValTxt->setText(P_LOGIN_OBJ->getCurSvrInfo().name);

    
	updateRoles();
}

void ChooseRolePanel::updateRoles() {
	std::vector<vmsg::CSPlayerRoleInfo> stRoleInfoList = P_LOGIN_OBJ->getRecentRoleInfoList();
	m_pRolesListView->removeAllItems();
	
	for(unsigned int i = 0; i < stRoleInfoList.size(); i++) {
		RoleCard *pRoleCard = RoleCard::create();
		pRoleCard->setState(RoleCard::CELL_NORMAL);
		pRoleCard->setRoleInfo(stRoleInfoList[i]);
		m_pRolesListView->pushBackCustomItem(pRoleCard);
	}

	if(m_pRolesListView->getItems()->count() < MAX_ROLE_NUM) {
		RoleCard *pRoleCard = RoleCard::create();
		pRoleCard->setState(RoleCard::CELL_NONE);
		m_pRolesListView->pushBackCustomItem(pRoleCard);
	}

	/*for(; i < stRoleInfoList.size(); i++) {
		RoleCard *pRoleCard = RoleCard::create();
		pRoleCard->setTag(getTagByIndex(i) );
		float posX = (m_pRolesLayout->getSize().width - pRoleCard->getSize().width) / 2;
		float posY = m_pRolesLayout->getSize().height - (pRoleCard->getSize().height) * (i + 1);
		pRoleCard->setPosition(ccp(posX, posY));
		pRoleCard->addTouchEventListener(this, toucheventselector(ChooseRolePanel::touchEventHandler));
		m_pRolesLayout->addChild(pRoleCard);
		pRoleCard->setState(RoleCard::CELL_NORMAL);
		pRoleCard->setRoleInfo(stRoleInfoList[i]);
	}

	if(m_pRolesLayout->getChildrenCount() < MAX_ROLE_NUM) {
		RoleCard *pRoleCard = RoleCard::create();
		pRoleCard->setTag(getTagByIndex(i));
		float posX = (m_pRolesLayout->getSize().width - pRoleCard->getSize().width) / 2;
		float posY = m_pRolesLayout->getSize().height - (pRoleCard->getSize().height) * (i + 1);
		pRoleCard->setPosition(ccp(posX, posY));
		pRoleCard->addTouchEventListener(this, toucheventselector(ChooseRolePanel::touchEventHandler));
		m_pRolesLayout->addChild(pRoleCard);
		pRoleCard->setState(RoleCard::CELL_NONE);
	}*/

	if(m_pRolesListView->getItems()->count() > 1) {
		//selectRoleByTag(getTagByIndex(0));
		selectRole(0);
	}
}

void ChooseRolePanel::updateRole(unsigned int uShowID, unsigned int uWeaponCfgID) {
	// 获取资源路径
	string bodyName;
	int iRet = CMainHero::GetResourceNames(uWeaponCfgID, uShowID, bodyName);

	if(iRet != 0) {
		CCLOG("Fail to get hero res! ShowID=%u WeaponCfgID=%u", uShowID, uWeaponCfgID);
		return;
	}

	string bodyPath = HERO_RES_PATH + bodyName + "/" + bodyName + JSON_TYPE_NAME;

	// 加载资源
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("guns/guns.plist");
	CCArmatureDataManager *pArmatureDataManager = CCArmatureDataManager::sharedArmatureDataManager();
	pArmatureDataManager->addArmatureFileInfo(bodyPath.c_str());

	if(m_pRole != NULL) {
		m_pRole->removeFromParent();
		m_pRole = NULL;
	} 

	m_pRole = CMainHero::create();
	m_pRole->setShowID(uShowID);
	m_pRole->setResPath(bodyName);
	m_pRole->LoadActionRes();
	m_pRole->setHP(1);
	m_pRole->getBlood()->setVisible(false);
	m_pRole->setScale(1.6f);
	m_pRole->Idle();
	m_pRole->setShadowVisiable(false);
	m_pAircraftBody->addChild(m_pRole);
	m_pRole->SwitchWeapon(uWeaponCfgID, false);
}

void ChooseRolePanel::selectRole(int nSelectedIdx) {
	int nTotalRoleCnt = P_LOGIN_OBJ->getRecentRoleInfoList().size();

	if(nSelectedIdx < nTotalRoleCnt) {
		RoleCard *pRoleCard = NULL;
		
		if(m_nRoleSelectedIdx >= 0) {
			pRoleCard = dynamic_cast<RoleCard*>(m_pRolesListView->getItem(m_nRoleSelectedIdx));
			pRoleCard->setState(RoleCard::CELL_NORMAL);
		}

		pRoleCard = dynamic_cast<RoleCard*>(m_pRolesListView->getItem(nSelectedIdx));
		pRoleCard->setState(RoleCard::CELL_SELECTED);
		m_nRoleSelectedIdx = nSelectedIdx;

		const vmsg::CSPlayerRoleInfo &stRoleInfo = pRoleCard->getRoleInfo();
		updateRole(stRoleInfo.showid(), stRoleInfo.weaponcfgid());
		m_pRoleNameTxt->setText(stRoleInfo.nick());
	}
}

void ChooseRolePanel::selectRoleByTag(const unsigned int iSelChildTag) {
    RoleCard *pRoleCard = NULL;
    pRoleCard = dynamic_cast<RoleCard*>(m_pRolesLayout->getChildByTag( m_nRoleSelectedTag ) );
    if(pRoleCard) {
        pRoleCard->setState(RoleCard::CELL_NORMAL);
    }
        
	pRoleCard = dynamic_cast<RoleCard*>(m_pRolesLayout->getChildByTag(iSelChildTag));
    if(NULL == pRoleCard) {
        return;
    }
	pRoleCard->setState(RoleCard::CELL_SELECTED);
	m_nRoleSelectedTag = iSelChildTag;

	const vmsg::CSPlayerRoleInfo &stRoleInfo = pRoleCard->getRoleInfo();
	//m_pUFOCell->updateRole(stRoleInfo.showid());
	updateRole(stRoleInfo.showid(), 58);
	m_pRoleNameTxt->setText(stRoleInfo.nick());
}

void ChooseRolePanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pEnterBtn->getName()) == 0) {
				if(m_nRoleSelectedIdx >= 0) {
					RoleCard *pRoleCard = dynamic_cast<RoleCard*>(m_pRolesListView->getItem(m_nRoleSelectedIdx));
					UIN_t uin = pRoleCard->getRoleInfo().uin();
					P_LOGIN_CTRL->getProxy()->sendRoleLoginRqst(uin);
					P_LOGIN_OBJ->setRecentLoginRoleName(pRoleCard->getRoleInfo().nick());
				}
			} else if(pWidget->getDescription() == "RoleCard") {
				if(dynamic_cast<RoleCard*>(pWidget)->getState() == RoleCard::CELL_NONE) {
					P_LOGIN_CTRL->getCreateRolePanel()->open();
				} else {
					//int nSelectedIdx = getIndexByTag(pWidget->getTag());
					//selectRole(nSelectedIdx);
                    //selectRoleByTag(pWidget->getTag());
				}
			}
			break;
		default:;
	}
}

void ChooseRolePanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(strcmp(pszListViewName, m_pRolesListView->getName()) == 0) {
				int nSelectedIdx = m_pRolesListView->getCurSelectedIndex();
				RoleCard *pRoleCard = dynamic_cast<RoleCard*>(m_pRolesListView->getItem(nSelectedIdx));

				if(pRoleCard->getState() == RoleCard::CELL_NONE) {
					P_LOGIN_CTRL->getCreateRolePanel()->open();
				} else {
					selectRole(nSelectedIdx);
				}
			}
			break;
		default:;
	}
}

int ChooseRolePanel::getIndexByTag(const int iTag) {
	return (iTag - ROLE_CARD_TAG_BASE);
}

int ChooseRolePanel::getTagByIndex(const int iIdx) {
	return ROLE_CARD_TAG_BASE + iIdx;
}
