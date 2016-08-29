#include "FightMedicalLayout.h"
#include "bag/BagControl.h"
#include "ConstHelper.h"
#include "defines.h"

CFightMedicalLayout::CFightMedicalLayout(){
	m_pMedicalBtn = NULL;
}

CFightMedicalLayout::~CFightMedicalLayout(){

}

bool CFightMedicalLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CFightMedicalLayout::initUI(){
	initLayoutFromJsonFile("ui_v2/fight_medical_ui.ExportJson");
	addChild(m_pRootLayout);

	m_pMedicalBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("medical_btn"));

	m_pNumTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("num_txt"));
}

void CFightMedicalLayout::updateUI(){
	unsigned int uCfgID = ConstHelper(FIGHT_MEDICAL_CFG_DI);
	unsigned int uNum = P_BAG_CTRL->getItemCnt(uCfgID);

	m_pNumTxt->setText(intToString(uNum));
}
