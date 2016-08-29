#include "StageTipLayout.h"
#include "NounHelper.h"
#include "defines.h"
#include "FightObj.h"

#include "WarMapCfgMaster.h"
#include "EliteWarMapCfgMaster.h"
#include "FightGuideMapCfgMaster.h"

using namespace std;

CStageTipLayout::CStageTipLayout(){
	m_uStageIdx = 0;
	m_uWarID = 0;
}

CStageTipLayout::~CStageTipLayout(){

}

bool CStageTipLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/fight_stage_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pBgImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("stage_bg"));
		m_pTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("stage_txt"));

		m_pBgImg->setOpacity(0x00);
		m_pTxt->setVisible(false);

		bRet = true;
	} while(0);

	return bRet;
}

void CStageTipLayout::showStageTip(unsigned int warId, unsigned int stageIdx){
	m_uWarID = warId;
	m_uStageIdx = stageIdx;

	string txtStr = "";
	if(P_FIGHT_OBJ->getFightType() == NORMAL_FIGHT_TYPE || P_FIGHT_OBJ->getFightType() == DAILY_FIGHT_TYPE){
		txtStr = WAR_MAP_CFG->GetWarMapCfgByID(m_uWarID)->name();
	}else if(P_FIGHT_OBJ->getFightType() == GUIDE_NORMAL_FIGHT_TYPE){
		txtStr = FIGHT_GUIDE_MAP_CFG_MASTER->GetFightGuideMapCfgByID(m_uWarID)->name();
	}else{
		txtStr = ELITE_WAR_MAP_CFG_MASTER->getEliteWarMapCfg(m_uWarID)->name();
	}
	
	txtStr += " " + NounStr(NOUN_STAGE_TXT) + intToString(m_uStageIdx);
	m_pTxt->setText(txtStr);

	CCActionInterval *pOptionAction = CCFadeIn::create(0.8f);
	CCFiniteTimeAction* endFun = CCCallFunc::create(this, callfunc_selector(CStageTipLayout::showEnd));
	m_pBgImg->setOpacity(0x00);
	m_pBgImg->getVirtualRenderer()->runAction(CCSequence::create(pOptionAction, CCDelayTime::create(1.0f), endFun, NULL));

	CCPoint pos = m_pTxt->getPosition();
	m_pTxt->setPosition(pos + ccp(-100, 0));
	CCActionInterval *pMoveAction = CCEaseBackOut::create(CCMoveTo::create(0.8f, pos));
	m_pTxt->runAction(CCSequence::create(CCShow::create(), pMoveAction, NULL));
}

void CStageTipLayout::showEnd(){
	this->removeFromParentAndCleanup(true);
}