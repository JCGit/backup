#include "GuardActPrizeUI.h"
#include "defines.h"
CGuardActPrizeUI::CGuardActPrizeUI()
{

}

CGuardActPrizeUI::~CGuardActPrizeUI(){

}
void CGuardActPrizeUI::addShadow(UILabel *pLabel){
#ifndef WIN32
    CCLabelTTF* pLabelTTF = dynamic_cast<CCLabelTTF*>(pLabel->getVirtualRenderer());
    pLabelTTF->enableShadow(CCSizeMake(2,-2), 1.0f, 1.0f);
#endif // !WIN32
}

bool CGuardActPrizeUI::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/act_guard_prize_ui.ExportJson");
		addChild(m_pRootLayout);


        m_pTotalValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("total_val_txt"));
        m_pKillTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("kill_award_txt"));
		m_pKillTxt->setText("0");
        m_pTotalValTxt->setText("0");

        addShadow(m_pTotalValTxt);
        addShadow(m_pKillTxt);


		bRet = true;
	} while (0);

	return bRet;
}

void CGuardActPrizeUI::setParent(CCNode* parent){
	BaseLayout::setParent(parent);
}



void CGuardActPrizeUI::updateMyContrNum(const int iMyContrNum) {
    if(iMyContrNum >= 0) {
        m_pTotalValTxt->setText(intToString(iMyContrNum));
    }
}

void CGuardActPrizeUI::updateMyCoinNum(const int iCoin){
	if(iCoin >= 0) {
		m_pKillTxt->setText(intToString(iCoin));
	}
}

