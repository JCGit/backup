#include "FightEndlessScoreUI.h"
#include "defines.h"
#include "LabelChange.h"
#include "TextHelper.h"
#include "CCColorUtil.h"

CFightEndlessScoreUI::CFightEndlessScoreUI()
:m_pScoreTxt(NULL)
,m_pCoinTxt(NULL)
,m_pPackageTxt(NULL)
,m_uPreCoin(0)
{

}

CFightEndlessScoreUI::~CFightEndlessScoreUI(){

}

bool CFightEndlessScoreUI::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/nihility_info_layout_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pInfoTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("info_txt"));
		m_pScoreTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("score_txt"));
		m_pCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_txt"));
		m_pPackageTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("package_num_txt"));
		m_pCoinTxt->setText("0");

		CCColorUtil::addStroke(m_pInfoTxt, 2.0f);
		CCColorUtil::addStroke(m_pScoreTxt, 2.0f);
		CCColorUtil::addStroke(m_pCoinTxt, 2.0f);
		CCColorUtil::addStroke(m_pPackageTxt, 2.0f);

		CCColorUtil::addShadow(m_pInfoTxt);
		CCColorUtil::addShadow(m_pScoreTxt);
		CCColorUtil::addShadow(m_pCoinTxt);
		CCColorUtil::addShadow(m_pPackageTxt);

		setInfo(1);
		setScore(0);
		setCoin(0);
		setPackage(0);

		bRet = true;
	} while (0);

	return bRet;
}

void CFightEndlessScoreUI::setInfo(unsigned int stage){
	string str = "MISSION " + intToString(stage);
	m_pInfoTxt->setText(str);
}

void CFightEndlessScoreUI::setScore(unsigned int score){
	m_pScoreTxt->setText(intToString(score));
}

void CFightEndlessScoreUI::setCoin(unsigned int coin){
	if(m_uPreCoin == coin){
		return;
	}

	if(m_pCoinTxt->getVirtualRenderer()->numberOfRunningActions() > 0){
		m_pCoinTxt->getVirtualRenderer()->stopAllActions();
		m_pCoinTxt->setText(intToString(m_uPreCoin));
	}

	float duration = 0.5f;
	CLabelChange* pScoreChange  = CLabelChange::create(duration, m_uPreCoin, coin);
	m_pCoinTxt->getVirtualRenderer()->runAction(pScoreChange);
	m_uPreCoin = coin;
}

void CFightEndlessScoreUI::setPackage(unsigned int package){
	string str = "x " + intToString(package);
	m_pPackageTxt->setText(str);
}

