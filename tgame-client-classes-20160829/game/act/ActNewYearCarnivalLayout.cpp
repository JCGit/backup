#include "ActNewYearCarnivalLayout.h"

#include "defines.h"

#include "ClientSvrMsg.pb.h"

#include "CfgMaster/ActCfgMaster.h"

#include "basic/ToolUtils.h"
#include "basic/TimeEx.h"

#include "RedEnvelopeCell.h"
#include "ActControl.h"

#define CELL_PREFIX_NAME "cell_"
#define COL_CELL_CNT 6

ActNewYearCarnivalLayout::ActNewYearCarnivalLayout()
:m_pActTimeValTxt(NULL)
,m_pActIntroValTxt(NULL)
,m_pRedEnvelopeLayout(NULL)
{
}

ActNewYearCarnivalLayout::~ActNewYearCarnivalLayout()
{
	CToolUtils::removeArmatureFile("effect/ut00087/ut00087.ExportJson");
}

ActNewYearCarnivalLayout* ActNewYearCarnivalLayout::create() {
	ActNewYearCarnivalLayout *pRet = new ActNewYearCarnivalLayout();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ActNewYearCarnivalLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void ActNewYearCarnivalLayout::updateInfo(const vmsg::CSCouponActStatusItem& info) {
	if(info.ByteSize() > 0) {
		const ActCfg* pActCfg = ACT_CFG_MASTER->GetActCfgByType(info.type());

		if(pActCfg != NULL) {
			m_pActTimeValTxt->setText(pActCfg->time());
			m_pActIntroValTxt->setText(pActCfg->content());
		}

		for(int i = 0; i < info.spring7daystatus_size(); i++) {
			const vmsg::CSSpring7DayPkgStatus &stSpringPkgStatus = info.spring7daystatus(i);
			string cellName = string(CELL_PREFIX_NAME) + intToString(i);
			RedEnvelopeCell *pCell = dynamic_cast<RedEnvelopeCell*>(m_pRedEnvelopeLayout->getChildByName(cellName.c_str()));

			if(pCell != NULL) {
				if(stSpringPkgStatus.claimstatus() == vmsg::ACT_PARTED) {
					pCell->setStat(RedEnvelopeCell::ENVELOPE_OPENED);
				} else {
					time_t tmVal = TimeEx::getTimeVal();

					if(tmVal >= stSpringPkgStatus.starttime() && tmVal <= stSpringPkgStatus.stoptime()) {
						pCell->setStat(RedEnvelopeCell::ENVELOPE_RECEIVED);
					} else {
						pCell->setStat(RedEnvelopeCell::ENVELOPE_CLOSED);
					}
				}
			}
		}
	}
}

void ActNewYearCarnivalLayout::initUI() {
	initLayoutFromJsonFile("ui_v2/ActNewYearCarnivalLayoutUI.ExportJson");
	addChild(m_pRootLayout);

	m_pActTimeValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_time_val_txt"));

	m_pActIntroValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_intro_val_txt"));

	m_pRedEnvelopeLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("red_envelope_layout"));

	CToolUtils::loadArmatureRGBA4444("effect/ut00087/ut00087.ExportJson");

	for(int i = 0; i < 12; i++) {
		RedEnvelopeCell *pCell = RedEnvelopeCell::create();
		string cellName = string(CELL_PREFIX_NAME) + intToString(i);
		pCell->setName(cellName.c_str());
		pCell->setDay(i+1);
		float cellWidth = pCell->getSize().width;
		float cellHeight = pCell->getSize().height;
		float dx = (m_pRedEnvelopeLayout->getSize().width - cellWidth * COL_CELL_CNT) / (COL_CELL_CNT + 1);
		float dy = (m_pRedEnvelopeLayout->getSize().height - cellHeight * 12 / COL_CELL_CNT) / (12 / COL_CELL_CNT);
		float posX = dx + cellWidth / 2 + i % COL_CELL_CNT * (cellWidth + dx);
		float posY = m_pRedEnvelopeLayout->getSize().height - 4 - i / COL_CELL_CNT * cellHeight / 2 - (i / COL_CELL_CNT + 1) * (cellHeight / 2 + dy);
		pCell->setPosition(ccp(posX, posY));
		m_pRedEnvelopeLayout->addChild(pCell);
		pCell->addTouchEventListener(this, toucheventselector(ActNewYearCarnivalLayout::touchEventHandler));
	}

	m_bUIInited = true;
}

void ActNewYearCarnivalLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(string(pszWidgetName).substr(0, string(CELL_PREFIX_NAME).size()) == CELL_PREFIX_NAME) {
				RedEnvelopeCell *pCell = dynamic_cast<RedEnvelopeCell*>(m_pRedEnvelopeLayout->getChildByName(pszWidgetName));

				if(pCell != NULL) {
					P_ACT_CTRL->sendClaimSpringPkgRqst(pCell->getDay());
					pCell->openEnvelope();
				}
			}
			break;
		default:;
	}
}
