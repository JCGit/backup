#include "AwardBoxTipPanel.h"

#include "BasicTypes.h"
#include "defines.h"
#include "Macros.pb.h"

#include "CfgMaster/WarAwardConditionCfgMaster.h"
#include "CfgMaster/PackageCfgMaster.h"

#include "game/IconFrameCell.h"

AwardBoxTipPanel::AwardBoxTipPanel()
:m_pAwardTxt(NULL)
,m_pAwardIconFrameCell(NULL)
,m_pItemCntTxt(NULL)
,m_pCondDescTxt(NULL)
{
}

AwardBoxTipPanel::~AwardBoxTipPanel()
{
}

AwardBoxTipPanel* AwardBoxTipPanel::create() {
	AwardBoxTipPanel *pRet = new AwardBoxTipPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool AwardBoxTipPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		bRet = true;
	} while(0);

	return bRet;
}

void AwardBoxTipPanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	closeLoading();
}

void AwardBoxTipPanel::show(unsigned int uType, unsigned int uCond, unsigned int uAwardID) {
	open();

	const PackageCfg *pPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uAwardID);

	if(pPackageCfg != NULL) {
		const PackageElmt &stElmt = pPackageCfg->elmtlst(0);
		m_pAwardIconFrameCell->setInfo(stElmt.elementtype(), stElmt.elementid());
		m_pItemCntTxt->setText(string("x") + intToString(stElmt.elementcntmin()));
	}

	m_pAwardIconFrameCell->setPosition(m_pAwardTxt->getPosition() + ccp(m_pAwardTxt->getSize().width + m_pAwardIconFrameCell->getSize().width / 2 + 8, 16 - m_pAwardIconFrameCell->getSize().height / 2));
	m_pItemCntTxt->setPosition(m_pAwardIconFrameCell->getPosition() + ccp(m_pAwardIconFrameCell->getSize().width / 2 + 8, 0));

	const WarAwardConditionCfg *pWarAwardConditionCfg = WAR_AWARD_CONDITION_CFG->GetWarAwardConditionCfgByID(uType);

	if(pWarAwardConditionCfg != NULL) {
		char text[100];
		string numStr; 
		if(uType == WAR_HIDE_TIME_TYPE || uType == WAR_HIDE_HP_TYPE){
			if(uType == WAR_HIDE_TIME_TYPE){
				numStr = intToString(uCond);
			}else{
				unsigned int tempInt = 100*uCond/EGAME_DENOM;
				numStr = intToString(tempInt) + "%";
			}
			snprintf(text, 100, pWarAwardConditionCfg->desc().c_str(), numStr.c_str());
			m_pCondDescTxt->setText(text);
		}else{
			m_pCondDescTxt->setText(pWarAwardConditionCfg->desc());
		}
	}
}

void AwardBoxTipPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/AwardBoxTipPanelUI.ExportJson");
	addChild(m_pRootLayout);

	addTouchEventListener(this, toucheventselector(AwardBoxTipPanel::touchEventHandler));

	m_pAwardTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_txt"));

	m_pAwardIconFrameCell = IconFrameCell::create();
	m_pRootLayout->addChild(m_pAwardIconFrameCell);

	m_pItemCntTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("item_cnt_txt"));

	m_pCondDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("cond_desc_txt"));

	m_bUIInited = true;

}

void AwardBoxTipPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == this) {
				close();
			}
			break;
		default:;
	}
}
