#include "FightEndlessWaveUI.h"
#include "defines.h"

CFightEndlessWaveUI::CFightEndlessWaveUI()
:m_pLeftLabel(NULL)
,m_pRightLabel(NULL)
,m_pTipImg(NULL)
,m_pWaveTxt(NULL)
,m_bIsTiping(false)
{

}

CFightEndlessWaveUI::~CFightEndlessWaveUI(){

}

bool CFightEndlessWaveUI::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/fight_endless_wave_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pLeftLabel = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("label_left"));
		m_pRightLabel = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("label_right"));
		m_pTipImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("next_tip"));
		m_pTipImg->setVisible(false);

		Label* pNumTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("num_txt"));
		pNumTxt->setVisible(false);
		m_pWaveTxt = ImageNumber::create(12);
		m_pWaveTxt->setPosition(pNumTxt->getPosition() + ccp(0, pNumTxt->getContentSize().height/2));
		m_pRootLayout->addNode(m_pWaveTxt);

		Label* pTimeTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("time_txt"));
		pTimeTxt->setVisible(false);
		m_pTimeTxt = ImageNumber::create(11);
		m_pTimeTxt->setPosition(pTimeTxt->getPosition() + ccp(0, pTimeTxt->getContentSize().height/2));
		m_pTimeTxt->setNum("0000", 0, "colon", 2);
		m_pRootLayout->addNode(m_pTimeTxt);

		bRet = true;
	} while (0);

	return bRet;
}

void CFightEndlessWaveUI::setWave(unsigned int wave){
	m_pWaveTxt->setNum(intToString(wave));
}

void CFightEndlessWaveUI::startTip(){
	if(m_bIsTiping){
		return;
	}

	if(m_pTipImg->numberOfRunningActions() > 0){
		m_pTipImg->setVisible(false);
		m_pTipImg->stopAllActions();
	}

	m_bIsTiping = true;
	CCBlink* blink = CCBlink::create(3, 3);
	CCFiniteTimeAction* completeFun = CCCallFunc::create(this, callfunc_selector(CFightEndlessWaveUI::binkComplete));
	m_pTipImg->runAction(CCSequence::create(CCShow::create(), blink, CCDelayTime::create(0.5f), CCHide::create(), completeFun, NULL));
}

void CFightEndlessWaveUI::binkComplete(){
	m_bIsTiping = false;
}