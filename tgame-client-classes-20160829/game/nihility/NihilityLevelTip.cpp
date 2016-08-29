#include "NihilityLevelTip.h"
#include "defines.h"

CNihilityLevelTip::CNihilityLevelTip(){
	m_uLevel = 0;
}

CNihilityLevelTip::~CNihilityLevelTip(){

}

bool CNihilityLevelTip::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/nihility_level_tip_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pMissImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("mission"));

		Label* txt = dynamic_cast<Label*>(m_pMissImg->getChildByName("level_txt"));
		txt->setVisible(false);
		m_pLevelTxt = ImageNumber::create(27);
		m_pLevelTxt->setNum("0");
		m_pLevelTxt->setAnchorPoint(ccp(0.5, 0.5));
		m_pMissImg->addNode(m_pLevelTxt);
		m_pLevelTxt->setPosition(txt->getPosition() + ccp(0, m_pLevelTxt->getContentSize().height/2));

		bRet = true;
	} while(0);

	return bRet;
}


void CNihilityLevelTip::showLevelTip(unsigned int level){
	m_uLevel = level;
	m_pLevelTxt->setNum(intToString(m_uLevel));

	float dur = 0.25f;
	m_pMissImg->setScale(3.0f);
	CCActionInterval *scaleTo = CCScaleTo::create(dur, 1.0f);
	CCFiniteTimeAction* endFun = CCCallFunc::create(this, callfunc_selector(CNihilityLevelTip::showEnd));
	m_pMissImg->runAction(CCSequence::create(scaleTo, CCDelayTime::create(dur*8), endFun, NULL));
}


void CNihilityLevelTip::showEnd(){
	this->removeFromParentAndCleanup(true);
}


