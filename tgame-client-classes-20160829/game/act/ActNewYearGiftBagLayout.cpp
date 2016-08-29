#include "ActNewYearGiftBagLayout.h"

#include "defines.h"

#include "CfgMaster/PackageCfgMaster.h"
#include "CfgMaster/ActCfgMaster.h"
#include "CfgMaster/SpringActCfgMaster.h"

#include "basic/ToolUtils.h"
#include "basic/TimeEx.h"

#include "game/tip/TipControl.h"
#include "game/IconCell.h"

#include "ActControl.h"

#define MAX_CELL_CNT 4

ActNewYearGiftBagLayout::ActNewYearGiftBagLayout()
:m_pActTimeValTxt(NULL)
,m_pGreetingBtn(NULL)
,m_pGreetingEff(NULL)
{
}

ActNewYearGiftBagLayout::~ActNewYearGiftBagLayout()
{
}

ActNewYearGiftBagLayout* ActNewYearGiftBagLayout::create() {
	ActNewYearGiftBagLayout *pRet = new ActNewYearGiftBagLayout();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ActNewYearGiftBagLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void ActNewYearGiftBagLayout::onEnter() {
	ActLayout::onEnter();

	CToolUtils::loadArmatureRGBA4444("effect/ut00089/ut00089.ExportJson");
	m_pGreetingEff = CCArmature::create("ut00089");
	m_pGreetingEff->setPosition(m_pGreetingBtn->getPosition());
	m_pRootLayout->addNode(m_pGreetingEff);
	m_pGreetingEff->getAnimation()->play("fly", -1, -1, 1);
}

void ActNewYearGiftBagLayout::onExit() {
	ActLayout::onExit();

	m_pGreetingEff->removeFromParent();
	m_pGreetingEff = NULL;
	CToolUtils::removeArmatureFile("effect/ut00089/ut00089.ExportJson");
}

void ActNewYearGiftBagLayout::updateInfo(const vmsg::CSCouponActStatusItem& info) {
	if(info.ByteSize() > 0) {
		const ActCfg* pActCfg = ACT_CFG_MASTER->GetActCfgByType(info.type());

		if(pActCfg != NULL) {
			m_pActTimeValTxt->setText(pActCfg->time());
		}

		bool isActTime = false;

		if(info.spring7daystatus_size() > 0) {
			const vmsg::CSSpring7DayPkgStatus &stSpringPkgStatus = info.spring7daystatus(0);
			time_t tmVal = TimeEx::getTimeVal();
			isActTime = tmVal >= stSpringPkgStatus.starttime() && tmVal <= stSpringPkgStatus.stoptime();
		}

		m_pGreetingBtn->setVisible(info.status() == vmsg::ACT_NOT_PARTED && isActTime);
		m_pGreetingBtn->setTouchEnabled(m_pGreetingBtn->isVisible());

		if(m_pGreetingEff != NULL) {
			m_pGreetingEff->setVisible(m_pGreetingBtn->isVisible());
		}
	}
}

void ActNewYearGiftBagLayout::initUI() {
	initLayoutFromJsonFile("ui_v2/ActNewYearGiftBagLayoutUI.ExportJson");
	addChild(m_pRootLayout);

	m_pActTimeValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_time_val_txt"));

	m_pGreetingBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("greeting_btn"));
	m_pGreetingBtn->addTouchEventListener(this, toucheventselector(ActNewYearGiftBagLayout::touchEventHandler));

	unsigned int uPkgID = SPRING_ACT_CFG_MASTER->getSpringActCfg(0)->pkgid(); // 0为此活动
	const PackageCfg* pPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uPkgID);

	for(int i = 0; i < MAX_CELL_CNT; i++) {
		string cellName = "cell_" + intToString(i);
		UIImageView *pImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(cellName.c_str()));
		IconCell *pCell = IconCell::create();
		pCell->setName(cellName.c_str());
		pImg->addChild(pCell);
		pImg->getVirtualRenderer()->setZOrder(pCell->getZOrder() + 1);
		pImg->setVisible(true);
		pImg->addTouchEventListener(this, toucheventselector(ActNewYearGiftBagLayout::touchEventHandler));

		if(pPackageCfg != NULL && i < pPackageCfg->elmtlst_size()) {
			const PackageElmt &stElmt = pPackageCfg->elmtlst(i);
			pCell->setInfo(stElmt.elementtype(), stElmt.elementcntmin(), stElmt.elementid());
			pImg->setTouchEnabled(true);
		} else {
			pCell->clearCell();
			pImg->setTouchEnabled(false);
		}
	}

	m_bUIInited = true;

}

void ActNewYearGiftBagLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pGreetingBtn->getName()) == 0) {
				P_ACT_CTRL->sendClaimSpringPkgRqst(0);
				unsigned int uPkgID = SPRING_ACT_CFG_MASTER->getSpringActCfg(0)->pkgid(); // 0为此活动
				P_ACT_CTRL->getHappyNewYearPanel()->openWithPkgID(uPkgID);
			} else if(string(pszWidgetName).substr(0, string("cell_").size()) == "cell_"){
				string cellName = pszWidgetName;
				string sub = cellName.substr(cellName.find_first_not_of("cell_"));
				unsigned int idx = atoi(sub.c_str());

				if(idx >= 0 && idx < MAX_CELL_CNT){
					UIImageView *pImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName(cellName.c_str()));
					IconCell *pCell = dynamic_cast<IconCell*>(pImg->getChildByName(pszWidgetName));
					P_TIP_CTRL->showItemTipPanel(pCell->getItemType(), pCell->getItemCfgid(), pCell->getItemNum(), false);
				}
			}
			break;
		default:;
	}
}
