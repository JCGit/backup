#include "FightHitLayout.h"
#include "defines.h"

const unsigned int CFightHitLayout::HIT_LAST_TIME = 2;
const string CFightHitLayout::HIT_OUT_TIME_EVENT = "hit_out_time_event";

CFightHitLayout::CFightHitLayout()
:m_pHitNumImg(NULL)
,m_fObjectTime(0)
,m_bIsOutTime(false)
,m_uPreHitNum(0)
,m_uHitNum(0)
{

}

CFightHitLayout::~CFightHitLayout(){

}

void CFightHitLayout::onEnter(){
	BaseLayout::onEnter();

	scheduleUpdate();
}

void CFightHitLayout::onExit(){
	BaseLayout::onExit();

	unscheduleUpdate();
	reset();
}

bool CFightHitLayout::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/fight_hit_ui.ExportJson");
		addChild(m_pRootLayout);

		Label* pTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("hit_txt"));
		pTxt->setVisible(false);
		m_pHitNumImg = ImageNumber::create(22);
		m_pHitNumImg->setNum("0");
		m_pHitNumImg->setAnchorPoint(ccp(0, 0.5));
		m_pRootLayout->addNode(m_pHitNumImg);
		m_pHitNumImg->setPosition(pTxt->getPosition() + ccp(0, m_pHitNumImg->getContentSize().height - 5));

		bRet = true;
	} while (0);

	return bRet;
}

void CFightHitLayout::update(float delta){
	m_fObjectTime += delta;

	if(m_fObjectTime > HIT_LAST_TIME && !m_bIsOutTime){  //超时
		m_bIsOutTime = true;
		DispatchEvent(HIT_OUT_TIME_EVENT, this);	
	}
}

void CFightHitLayout::setHitNum(unsigned int uNum){
	if(m_bIsOutTime){
		return;
	}

	if(m_pHitNumImg->numberOfRunningActions() > 0){
		m_pHitNumImg->stopAllActions();
		m_pHitNumImg->setNum(intToString(m_uPreHitNum));
	}

	m_fObjectTime = 0;		//时间重置
	m_uHitNum = uNum;
	m_uPreHitNum = uNum;

	m_pHitNumImg->setNum(intToString(m_uHitNum));

	float dur = 0.5f;
	m_pHitNumImg->setScale(1.0f);
	CCActionInterval* scaleBy = CCScaleBy::create(dur, 1.05f);
	m_pHitNumImg->runAction(CCSequence::create(scaleBy, scaleBy->reverse(), NULL));
}

void CFightHitLayout::reset(){
	m_uHitNum = 0;
	m_uPreHitNum = 0;
	m_fObjectTime = 0;
	m_bIsOutTime = false;
}
