#include "SweepInfoItem.h"
#include "ItemUtils.h"
#include "defines.h"
#include "TextHelper.h"

CSweepInfoItem::CSweepInfoItem(){
	m_IDX = 0;
}

CSweepInfoItem::~CSweepInfoItem(){

}

bool CSweepInfoItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());


		initLayoutFromJsonFile("ui_v2/SweepInfoItemUI.ExportJson");
		addChild(m_pRootLayout);

		m_pNumTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("num_txt"));
		m_pExpTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("exp_txt"));
		m_pCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_txt"));

		m_pAwardDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_desc_txt"));
		
		bRet = true;
	} while(0);

	return bRet;
}

void CSweepInfoItem::setInfo(const CSSweepAward& stAwardInfo, unsigned int idx){
	m_stAwardInfo = stAwardInfo;
	m_IDX = idx;

	updateUI();
}

const CSSweepAward& CSweepInfoItem::getInfo(){
	return m_stAwardInfo;
}

void CSweepInfoItem::updateUI(){
	char str[100];
	snprintf(str, 100,  TextStr(SWEEP_ROUND_DESC) , intToString(m_IDX + 1).c_str()); 

	m_pNumTxt->setText(str);
	m_pExpTxt->setText(intToString(m_stAwardInfo.exp()));
	m_pCoinTxt->setText(intToString(m_stAwardInfo.coin()));

	m_pAwardDescTxt->setVisible(true);

	string desc = "";

	for(int i = 0; i < m_stAwardInfo.award_size(); i++) {
		const vmsg::CSAwardElement &stAwardElmt = m_stAwardInfo.award(i);
		desc += ItemUtils::getItemName(stAwardElmt.elementtype(), stAwardElmt.elementid()) + " x" + intToString(stAwardElmt.elementcnt()) + "    ";
	}

	m_pAwardDescTxt->setText(desc);

}