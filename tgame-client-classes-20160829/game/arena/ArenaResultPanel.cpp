#include "ArenaResultPanel.h"
#include "SceneManager.h"
#include "UserObj.h"
#include "ArenaControl.h"
#include "defines.h"
#include "city/CityControl.h"
#include "city/CityProxy.h"

#include "GlobalCfgMaster.h"
#include "HeroCfgMaster.h"
#include "ArenaLvCfgMaster.h"
#include "ArenaFightAwardCfgMaster.h"

CArenaResultPanel::CArenaResultPanel(){

}

CArenaResultPanel::~CArenaResultPanel(){

}

bool CArenaResultPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/arena_result_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		setClickOutToClose(false);

		m_pSureBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("sure_btn"));
		m_pScoreTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("score_txt"));
		m_pCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_coin_txt"));
		m_pArenaCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_arena_coin_txt"));
		m_pResultImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("result_img"));

		m_pSureBtn->addTouchEventListener(this, toucheventselector(CArenaResultPanel::touchEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}

void CArenaResultPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget* pWidget = dynamic_cast<UIWidget*>(pSender);
	const char* pName = pWidget->getName();
	string strName = pWidget->getName();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		if(strcmp(pName, "sure_btn") == 0){
			End();
		}
		break;
	default:
		break;
	}
}

void CArenaResultPanel::updateUI(){
	const CSArenaQueryRsp& stInfo = P_ARENA_CTRL->getArenaObj()->getArenaInfo();
	const CSArenaEndFightRsp& stEndInfo = P_ARENA_CTRL->getArenaObj()->getFightEndInfo();
	unsigned int isMyWin = P_ARENA_CTRL->getArenaObj()->getIsMyWin();

	m_pCoinTxt->setText("x" + intToString(stEndInfo.coinchange()));
	m_pArenaCoinTxt->setText("x" + intToString(stEndInfo.arenamoneychange()));

	unsigned int winScore = GLOBAL_CFG_MASTER->GetCfg().arenawinscore();
	unsigned int loseScore = GLOBAL_CFG_MASTER->GetCfg().arenalosescore();

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/nihility_ui.plist");
	if(isMyWin){
		m_pResultImg->loadTexture("arena_ui/victory_img.png", UI_TEX_TYPE_PLIST);
		m_pScoreTxt->setText("+" + intToString(winScore));
	}else{
		m_pResultImg->loadTexture("nihility_ui/fail_title.png", UI_TEX_TYPE_PLIST);
		m_pScoreTxt->setText("-" + intToString(loseScore));
	}

	updateRole();
}

void CArenaResultPanel::updateRole(){
    /*
	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
	unsigned int uShowID = pUserInfo->showID;
	unsigned int uWeaponCfgID = pUserInfo->weapon.cfgid();

	// 获取资源路径
	string bodyName;
	int iRet = CMainHero::GetResourceNames(uWeaponCfgID, uShowID, bodyName);

	if(iRet != 0) {
		CCLOG("Fail to get hero res! ShowID=%u WeaponCfgID=%u", uShowID, uWeaponCfgID);
		return;
	}

	role = CMainHero::create();
	role->setShowID(uShowID);
	role->setResPath(bodyName);
	role->LoadActionRes();
	role->setPosition(m_pRoleImg->getPosition() + ccp(0, -100));
	role->setHP(1);
	role->getBlood()->setVisible(false);
	role->setScale(2.0);
	role->ForceIdle();
	role->SwitchWeapon(uWeaponCfgID, false);
	m_pRootLayout->addNode(role);
    */
}


void CArenaResultPanel::End(){
	this->close();

	SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
}