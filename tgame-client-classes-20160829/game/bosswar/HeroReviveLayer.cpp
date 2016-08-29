#include "HeroReviveLayer.h"
#include "defines.h"
#include "BossWarControl.h"
#include "tip/TipControl.h"

#include "BossChallengeInstanceCfgMaster.h"

const string CHeroReviveLayer::HERO_REVIVE_EVENT = "HERO_REVIVE_EVENT";

CHeroReviveLayer::CHeroReviveLayer(){
	m_pUILayout = NULL;
	m_pLayer = NULL;
	m_uType = BOSS_CHALLENGE_WORLD;		//暂时为世界boss
	m_uReliveTime = 0;
	m_uReliveCost = 0;
}

CHeroReviveLayer::~CHeroReviveLayer(){

}

bool CHeroReviveLayer::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

		float largeOffset = 100.0f;
		CCSize largeSize = CCSizeMake(size.width + largeOffset * 2, size.height + largeOffset * 2);

		setSize(size);
		setPosition(point);

		m_pLayer = UILayout::create();
		m_pLayer->setTouchEnabled(true);
		m_pLayer->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		m_pLayer->setBackGroundColorType(LAYOUT_COLOR_SOLID);
		m_pLayer->setBackGroundColorOpacity(0x40);
		m_pLayer->setSize(largeSize);
		addChild(m_pLayer);

		m_pUILayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("ui_v2/boss_war_revive_ui.ExportJson"));
		m_pUILayout->setPosition(ccp(size.width/2 - m_pUILayout->getSize().width/2, size.height/2));
		m_pLayer->addChild(m_pUILayout);

		m_pLabelImg = dynamic_cast<ImageView*>(m_pUILayout->getChildByName("revive_label_img"));
		m_pIconImg = dynamic_cast<ImageView*>(m_pUILayout->getChildByName("icon_img"));
		m_pTimeTxt = dynamic_cast<Label*>(m_pUILayout->getChildByName("time_txt"));
		m_pNumTxt = dynamic_cast<Label*>(m_pIconImg->getChildByName("num_txt"));

		m_pReviveBtn = dynamic_cast<Button*>(m_pUILayout->getChildByName("revive_btn"));
		m_pReviveBtn->addTouchEventListener(this, toucheventselector(CHeroReviveLayer::touchEventHandler));

		const BossChallengeCfg* stCfg = BOSS_CHALLENGE_CFG->GetBossChallengeCfgByType(m_uType);
		m_uReliveCost = stCfg->relivecost();
		m_uReliveTime = stCfg->relivetime();
		m_pNumTxt->setText(intToString(m_uReliveCost));
		m_pTimeTxt->setText(intToString(m_uReliveTime));

		bRet = true;
	} while(0);

	return bRet;
}

void CHeroReviveLayer::startCountdown(){
	timeFlag = m_uReliveTime;
	m_pTimeTxt->setText(intToString(timeFlag));
	this->schedule(schedule_selector(CHeroReviveLayer::updateTime),1);
}

void CHeroReviveLayer::updateTime(float dt){
	if (timeFlag > 1){ 
		timeFlag--;
		m_pTimeTxt->setText(intToString(timeFlag));
	}else{
		continueGame();
		this->unschedule(schedule_selector(CHeroReviveLayer::updateTime));
	}
}

void CHeroReviveLayer::continueGame(){
	this->removeFromParentAndCleanup(true);
	DispatchEvent(HERO_REVIVE_EVENT, this);	
}

void CHeroReviveLayer::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, "revive_btn") == 0) { 
			P_TIP_CTRL->GoldCostTip(m_uReliveCost, this, dialogbtneventselector(CHeroReviveLayer::onBuyReliveBack), TextStr(TEXT_WORLD_BOSS_RELIVE_TIP));
		}
		break;
	}
}

void CHeroReviveLayer::onBuyReliveBack(){
	P_BOSS_WAR_CTRL->sendReliveRqst();
}

