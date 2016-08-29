#include "BuyCoinPanel.h"
#include "VIPControl.h"
#include "defines.h"
#include "UserObj.h"
#include "PopTipLayer.h"
#include "TextHelper.h"
#include "tip/TipControl.h"
#include "ToolUtils.h"
#include "CCColorUtil.h"

BuyCoinPanel::BuyCoinPanel()
{
    
}

BuyCoinPanel::~BuyCoinPanel()
{
}

BuyCoinPanel* BuyCoinPanel::create() {
	BuyCoinPanel *pRet = new BuyCoinPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool BuyCoinPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

        setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/buy_coin_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pIconImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("icon_img"));
        m_pBuyNumTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("buy_num_txt"));
        m_pGoldNumTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("gold_num_txt"));
        m_pCoinNumTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_num_txt"));

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(BuyCoinPanel::touchEventHandler));

        m_pBuyBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("buy_btn"));
        m_pBuyBtn->addTouchEventListener(this, toucheventselector(BuyCoinPanel::touchEventHandler));

		m_pIconImg->loadTexture("icon/buycoin_icon.png");

		bRet = true;
	} while(0);

	return bRet;
}

void BuyCoinPanel::open() {
	BasePanel::open();
    P_VIP_CTRL->getProxy()->sendQueryBuyCoinRqst();
	this->showLoading();

	CToolUtils::loadArmatureRGBA4444("effect/ut00086/ut00086.ExportJson");
}

void BuyCoinPanel::close() {
    BasePanel::close();

	CToolUtils::removeArmatureFile("effect/ut00086/ut00086.ExportJson");
}

void BuyCoinPanel::updateUI() {
	if(this->getIsInLoading()) {
		this->closeLoading();
	}
    const vmsg::CSBuyCoinRsp& stBuyCoinRsp = P_VIP_CTRL->getBuyCoinRsp();
    m_pGoldNumTxt->setText(intToString(stBuyCoinRsp.info().nextcostgold()));
    m_pCoinNumTxt->setText(intToString(stBuyCoinRsp.info().nextcoin() ) );

	string numStr = " " + intToString(stBuyCoinRsp.info().remaincnt()) + "/" + intToString(stBuyCoinRsp.info().maxcnt()) + " ";
	char str[100];
	snprintf(str, 100,  TextStr(TEXT_VIP_BUY_COIN_NUM_DESC) , numStr.c_str()); 
	m_pBuyNumTxt->setText(str);

	if(stBuyCoinRsp.info().remaincnt() <= 0) {
		CCColorUtil::setLabelColor(m_pBuyNumTxt, ccRED);
	} else {
		CCColorUtil::setLabelColor(m_pBuyNumTxt, ccWHITE);
	}
}

void BuyCoinPanel::showMultiEff(unsigned int uMulti){
	if(uMulti < 2){
		return;
	}

	CCArmature* pEff = CCArmature::create("ut00086");
	pEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BuyCoinPanel::onEffDone));
	pEff->setPosition(ccp(m_pRootLayout->getSize().width/2, m_pRootLayout->getSize().height/2));
	m_pRootLayout->addNode(pEff, 11);
	string actName = "fly_" + intToString(uMulti);
	pEff->getAnimation()->play(actName.c_str(), -1, -1, 0); //not loop
}


void BuyCoinPanel::onEffDone(CCArmature* armature, MovementEventType type, const char* name) {
	if (COMPLETE == type){
		armature->removeFromParentAndCleanup(true);
	} 
}

void BuyCoinPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
            } else if(std::strcmp(pszWidgetName, m_pBuyBtn->getName()) == 0) {
                doBuy();
            } 
			break;
		default:;
	}
}

void BuyCoinPanel::doBuy() {
    
    const vmsg::CSBuyCoinRsp& stBuyCoinRsp = P_VIP_CTRL->getBuyCoinRsp();
    
    //check remain num
    if(stBuyCoinRsp.info().remaincnt() <= 0) {
        P_TIP_CTRL->getPromptPayPanel()->openWithVIPModule(vmsg::VIP_MODULE_BUYCOIN);
        return;
    }

    //check cost
	P_TIP_CTRL->GoldCostTip(stBuyCoinRsp.info().nextcostgold(), this, dialogbtneventselector(BuyCoinPanel::onBuyCallback));
}

void BuyCoinPanel::onBuyCallback() {
    P_VIP_CTRL->getProxy()->sendBuyCoinRqst();
}
