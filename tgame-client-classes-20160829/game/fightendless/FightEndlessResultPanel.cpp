#include "FightEndlessResultPanel.h"
#include "UserObj.h"
#include "IconCell.h"
#include "SceneManager.h"
#include "defines.h"
#include "FightEndlessControl.h"
#include "CommonDialog.h"
#include "NounHelper.h"

#include "HeroCfgMaster.h"

using namespace vmsg;

CFightEndlessResultPanel::CFightEndlessResultPanel()
:m_pSureBtn(NULL)
,m_pShareBtn(NULL)
,m_pRoleImg(NULL)
,m_pScoreTxt(NULL)
,m_pCoinTxt(NULL)
,m_pMaxTxt(NULL)
,m_pRole(NULL)
,m_pCellArray(NULL)
,m_pUICellArray(NULL)
{
	
}

CFightEndlessResultPanel::~CFightEndlessResultPanel(){
	if(m_pCellArray) {
		m_pCellArray->removeAllObjects();
		m_pCellArray->release();
	}

	if(m_pUICellArray) {
		m_pUICellArray->removeAllObjects();
		m_pUICellArray->release();
	}
}

bool CFightEndlessResultPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui/endless_result_panel.json");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CFightEndlessResultPanel::initUI(){
	m_pSureBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("sure_btn"));
	m_pShareBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("share_btn"));
	m_pScoreTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("score_txt"));
	m_pCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_txt"));
	m_pMaxTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("max_score_txt"));
	m_pRoleImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("role_img"));

	m_pScoreLabel = CRollNumber::createWithProperty("00", "ui_v2/numbers/number_4.png", 19, 30, "0");
	m_pScoreLabel->setLabelAnchorPoint(ccp(0, 0.5));
	m_pRootLayout->addChild(m_pScoreLabel);
	m_pScoreLabel->setPosition(m_pScoreTxt->getPosition());
	m_pScoreTxt->setVisible(false);

	m_pCellArray = CCArray::create();
	m_pCellArray->retain();
	m_pUICellArray = CCArray::create();
	m_pUICellArray->retain();

	UIImageView *pImg;
	IconCell *stCell;
	for(int i = 0; i < 5; ++i) {
		string strUIName = "cell_" + intToString(i);
		pImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(strUIName.c_str()));
		stCell = IconCell::create();
		stCell->showNumTxt();
		pImg->addChild(stCell);

		m_pCellArray->addObject(stCell);
		m_pUICellArray->addObject(pImg);
	}

	m_pSureBtn->addTouchEventListener(this, toucheventselector(CFightEndlessResultPanel::touchEventHandler));
	m_pShareBtn->addTouchEventListener(this, toucheventselector(CFightEndlessResultPanel::touchEventHandler));
}

void CFightEndlessResultPanel::updateUI(){
	//m_pScoreTxt->setText(to_string(P_ENDLESS_CTRL->getScore()));
	m_pCoinTxt->setText(to_string(P_ENDLESS_CTRL->getCoin()));
	m_pMaxTxt->setText(to_string(P_ENDLESS_CTRL->getMaxScore()));

	//物件奖励
	IconCell *stCell;
	UIImageView *pImg;
	const CSInfiniteEndRsp& stAwardInfo = P_ENDLESS_CTRL->getEndInfo();
	for(int i = 0; i < 5; i++){
		stCell = dynamic_cast<IconCell*>(m_pCellArray->objectAtIndex(i));
		pImg = dynamic_cast<UIImageView*>(m_pUICellArray->objectAtIndex(i));
		pImg->setVisible(true);
		if(i < stAwardInfo.awardlist_size()){
			const CSAwardElement& elmt = stAwardInfo.awardlist(i);
			stCell->setInfo(elmt.elementtype(), elmt.elementcnt(), elmt.elementid());
		}else{
			stCell->clearCell();
			pImg->setVisible(false);
		}
	}

	updateRole();

	m_pScoreLabel->showNumWithAction(P_ENDLESS_CTRL->getScore());
}

void CFightEndlessResultPanel::updateRole(){
	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
	const string* pHeroStr = HERO_CFG_MASTER->GetResName(pUserInfo->showID);
	string stBodyName;
	int iRet = CMainHero::GetResourceNames(pUserInfo->weapon.cfgid(), pUserInfo->showID, stBodyName);
	if (iRet){
		CCLog("no resource name is found for hero %s", pHeroStr->c_str());
		return;
	}

	m_pRole = CMainHero::create();
	m_pRole->setShowID(pUserInfo->showID);
	m_pRole->setResPath(stBodyName);
	m_pRole->LoadActionRes();
	m_pRole->setPosition(m_pRoleImg->getPosition() + ccp(0, -100));
	m_pRole->setHP(1);
	m_pRole->getBlood()->setVisible(false);
	m_pRole->setScale(2.0);
	m_pRole->Idle();
	m_pRootLayout->addNode(m_pRole);
}

void CFightEndlessResultPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget* pWidget = dynamic_cast<UIWidget*>(pSender);
	const char* pName = pWidget->getName();
	string strName = pWidget->getName();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		if(strcmp(pName, "sure_btn") == 0){
			this->close();
			SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
		}else if(strcmp(pName, "share_btn") == 0){
			
		}
		break;
	default:
		break;
	}
}