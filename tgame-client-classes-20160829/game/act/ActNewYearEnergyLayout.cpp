#include "ActNewYearEnergyLayout.h"

#include "defines.h"

#include "CfgMaster/ActCfgMaster.h"

#include "ActControl.h"

ActNewYearEnergyLayout::ActNewYearEnergyLayout()
:m_pActTimeValTxt(NULL)
,m_pActIntroValTxt(NULL)
,m_pProgressBar(NULL)
,m_pProgressSliderImg(NULL)
,m_pManualRecvValTxt(NULL)
,m_pGetManualBtn(NULL)
{
}

ActNewYearEnergyLayout::~ActNewYearEnergyLayout()
{
}

ActNewYearEnergyLayout* ActNewYearEnergyLayout::create() {
	ActNewYearEnergyLayout *pRet = new ActNewYearEnergyLayout();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ActNewYearEnergyLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void ActNewYearEnergyLayout::updateInfo(const vmsg::CSCouponActStatusItem& info) {
	if(info.ByteSize() > 0) {
		const ActCfg* pActCfg = ACT_CFG_MASTER->GetActCfgByType(info.type());

		if(pActCfg != NULL) {
			m_pActTimeValTxt->setText(pActCfg->time());
			m_pActIntroValTxt->setText(pActCfg->content());

			int percent = info.storedmanual() * 100 / 500;
			m_pProgressBar->setPercent(percent);
			float barLen = m_pProgressBar->getSize().width;
			m_pProgressSliderImg->setPositionX(m_pProgressBar->getPositionX() - 0.5f * barLen + barLen * percent / 100);
			m_pProgressSliderImg->setVisible(percent > 1);

			m_pManualRecvValTxt->setText(intToString(info.storedmanual()));

			m_pGetManualBtn->setBright(info.storedmanual() > 0);
			m_pGetManualBtn->setTouchEnabled(m_pGetManualBtn->isBright());
		}
	}
}

void ActNewYearEnergyLayout::initUI() {
	initLayoutFromJsonFile("ui_v2/ActNewYearEnergyLayoutUI.ExportJson");
	addChild(m_pRootLayout);

	m_pActTimeValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_time_val_txt"));

	m_pActIntroValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_intro_val_txt"));

	m_pProgressBar = dynamic_cast<UILoadingBar*>(m_pRootLayout->getChildByName("progress_bar"));

	m_pProgressSliderImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("progress_slider_img"));
	m_pProgressSliderImg->setVisible(false);

	m_pManualRecvValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("manual_recv_val_txt"));

	m_pGetManualBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("get_manual_btn"));
	m_pGetManualBtn->addTouchEventListener(this, toucheventselector(ActNewYearEnergyLayout::touchEventHandler));

	m_bUIInited = true;

}

void ActNewYearEnergyLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pGetManualBtn->getName()) == 0) {
				P_ACT_CTRL->sendClaimStoredManualRqst();
			}
			break;
		default:;
	}
}
