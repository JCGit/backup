#include "HappyNewYearPanel.h"

#include "CfgMaster/PackageCfgMaster.h"

#include "basic/ItemUtils.h"

#include "game/IconFrameCell.h"

#define MAX_CELL_CNT 4

HappyNewYearPanel::HappyNewYearPanel()
:m_pCloseBtn(NULL)
,m_pOkBtn(NULL)
,m_pAwardsLayout(NULL)
,m_pFlyStars(NULL)
{
}

HappyNewYearPanel::~HappyNewYearPanel()
{
	for(unsigned int i = 0; i < m_stAwardCells.size(); i++) {
		IconFrameCell *pCell = m_stAwardCells[i];
		CC_SAFE_RELEASE(pCell);
	}

	m_stAwardCells.clear();

	for(unsigned int i = 0; i < m_stLabelNames.size(); i++) {
		UILabel *pNameTxt = m_stLabelNames[i];
		CC_SAFE_RELEASE(pNameTxt);
	}

	m_stLabelNames.clear();
}

HappyNewYearPanel* HappyNewYearPanel::create() {
	HappyNewYearPanel *pRet = new HappyNewYearPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool HappyNewYearPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void HappyNewYearPanel::close() {
	BasePanel::close();

	m_pFlyStars->removeFromParent();
	CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("effect/ut00088/ut00088.ExportJson");
}

void HappyNewYearPanel::openWithPkgID(unsigned int uPkgID) {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00088/ut00088.ExportJson");

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCSize screenSize = pDirector->getVisibleSize();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCPoint screenCenter = screenOrigin + CCPoint(screenSize / 2.0f);

	m_pFlyStars = CCArmature::create("ut00088");
	m_pFlyStars->getAnimation()->play("fly", -1, -1, 1); //not loop
	m_pFlyStars->setPosition(convertToNodeSpace(screenCenter));
	addNode(m_pFlyStars, m_pRootLayout->getZOrder() - 1);

	m_pAwardsLayout->removeAllChildren();
	const PackageCfg *pPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(uPkgID);
	bool hasBigIcon = false;

	if(pPackageCfg != NULL) {
		for(int i = 0; i < MAX_CELL_CNT; i++) {
			IconFrameCell *pCell = m_stAwardCells.at(i);
			UILabel *pNameTxt = m_stLabelNames.at(i);

			if(i < pPackageCfg->elmtlst_size()) {
				const PackageElmt &stElmt = pPackageCfg->elmtlst(i);
				pCell->setInfo(stElmt.elementtype(), stElmt.elementid(), stElmt.elementcntmin());
				m_pAwardsLayout->addChild(pCell);
				pNameTxt->setText(ItemUtils::getItemName(stElmt.elementtype(), stElmt.elementid()));
				m_pAwardsLayout->addChild(pNameTxt);
			} else {
				pCell->clear();
			}

			if(pCell->getSize().width > 100) {
				hasBigIcon = true;
			}
		}
	}

	float originX = m_pAwardsLayout->getSize().width / 2 - (hasBigIcon ? 424 : 504) / 2 - 50;
	float originY = m_pAwardsLayout->getSize().height / 2 + 20;
	float posX = originX;

	for(int i = 0; i < MAX_CELL_CNT; i++) {
		IconFrameCell *pCell = m_stAwardCells.at(i);
		pCell->setPosition(ccp(posX + pCell->getSize().width / 2, originY));
		UILabel *pNameTxt = m_stLabelNames.at(i);
		pNameTxt->setPosition(ccp(posX + pCell->getSize().width / 2, originY - 80));
		posX += pCell->getSize().width + 8;
	}

	closeLoading();
}

void HappyNewYearPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/HappyNewYearPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(HappyNewYearPanel::touchEventHandler));

	m_pOkBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("ok_btn"));
	m_pOkBtn->addTouchEventListener(this, toucheventselector(HappyNewYearPanel::touchEventHandler));

	m_pAwardsLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("awards_layout"));

	for(int i = 0; i < MAX_CELL_CNT; i++) {
		IconFrameCell *pCell = IconFrameCell::create();
		m_stAwardCells.push_back(pCell);
		CC_SAFE_RETAIN(pCell);

		UILabel *pNameTxt = UILabel::create();
		pNameTxt->setFontName("fonts/tgamefont.ttf");
		pNameTxt->setFontSize(24);
		pNameTxt->setColor(ccc3(0xff, 0xfc, 0x00)); //fff000
		pNameTxt->setTouchEnabled(false);
		pNameTxt->setAnchorPoint(ccp(0.5, 0.5));
		dynamic_cast<CCLabelTTF*>(pNameTxt->getVirtualRenderer())->enableShadow(CCSizeMake(2, 2), 0.4f, 1.0f);
		m_stLabelNames.push_back(pNameTxt);
		CC_SAFE_RETAIN(pNameTxt);
	}

	m_bUIInited = true;
}

void HappyNewYearPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(strcmp(pszWidgetName, m_pOkBtn->getName()) == 0) {
				close();
			}
			break;
		default:;
	}

}
