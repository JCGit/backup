#include "BossWarWaitLayer.h"
#include "defines.h"
#include "Global.h"
#include "CCColorUtil.h"
#include "BossWarControl.h"

const string CBossWarWaitLayer::WAIT_TIME_DONE_EVENT = "WAIT_TIME_DONE_EVENT";

CBossWarWaitLayer::CBossWarWaitLayer(){
	m_pLayer = NULL;
}

CBossWarWaitLayer::~CBossWarWaitLayer(){

}


bool CBossWarWaitLayer::init(){
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
		m_pLayer->setSize(largeSize);
		addChild(m_pLayer);

		m_pUILayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("ui_v2/boss_war_wait_ui.ExportJson"));
		m_pUILayout->setPosition(ccp(size.width/2 - m_pUILayout->getSize().width/2, size.height/2));
		m_pLayer->addChild(m_pUILayout);

		m_pTimeTxt = dynamic_cast<Label*>(m_pUILayout->getChildByName("time_txt"));
		m_pTimeTxt->setText("0");

		CCColorUtil::addStroke(m_pTimeTxt, 2.0f);
		CCColorUtil::addShadow(m_pTimeTxt);

		bRet = true;
	} while(0);

	return bRet;
}

void CBossWarWaitLayer::startCountdown(){
	unsigned int uStartTime = P_BOSS_WAR_CTRL->getBossChallengeInfo().starttime();
	unsigned int uNowTime = P_GLOBAL->getSvrTimeRelative();
	if(uNowTime >= uStartTime){
		continueGame();
		return;
	}

	timeFlag = uStartTime - uNowTime;
	m_pTimeTxt->setText(intToString(timeFlag));
	this->schedule(schedule_selector(CBossWarWaitLayer::updateTime),1);
}

void CBossWarWaitLayer::updateTime(float dt){
	if (timeFlag > 1){ 
		timeFlag--;
		m_pTimeTxt->setText(intToString(timeFlag));
	}else{
		continueGame();
		this->unschedule(schedule_selector(CBossWarWaitLayer::updateTime));
	}
}

void CBossWarWaitLayer::continueGame(){
	this->removeFromParentAndCleanup(true);
	DispatchEvent(WAIT_TIME_DONE_EVENT, this);	
}

