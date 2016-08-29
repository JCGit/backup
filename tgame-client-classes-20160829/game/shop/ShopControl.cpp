#include "ShopControl.h"
#include "PopTipLayer.h"
#include "TextHelper.h"
#include "frag/FragControl.h"
#include "RmbShopPanel.h"
#include "game/bag/BagControl.h"


ShopControl::ShopControl()
{
    m_pBlackShopPanel = NULL;
    m_pBlackShopItemInfoPanel = NULL;
    m_pRmbShopPanel = NULL;
	m_pBuyMonthCardPanel = NULL;
	m_pFirstChargePanel = NULL;
}

ShopControl::~ShopControl()
{
	finalize();
}


void ShopControl::finalize() {
    CC_SAFE_RELEASE_NULL(m_pBlackShopPanel); 
    CC_SAFE_RELEASE_NULL(m_pBlackShopItemInfoPanel); 
    CC_SAFE_RELEASE_NULL(m_pRmbShopPanel);
	CC_SAFE_RELEASE_NULL(m_pBuyMonthCardPanel);
	CC_SAFE_RELEASE_NULL(m_pFirstChargePanel);
}


BlackShopPanel* ShopControl::getBlackShopPanel() {
    if(m_pBlackShopPanel == NULL) {
        m_pBlackShopPanel = BlackShopPanel::create();
        m_pBlackShopPanel->retain(); // release at ShopControl::finalize()
    }

    return m_pBlackShopPanel;
}

BlackShopItemInfoPanel* ShopControl::getBlackShopItemInfoPanel() {
    if(m_pBlackShopItemInfoPanel == NULL) {
        m_pBlackShopItemInfoPanel = BlackShopItemInfoPanel::create();
        m_pBlackShopItemInfoPanel->retain(); // release at ShopControl::finalize()
    }

    return m_pBlackShopItemInfoPanel;
}

RmbShopPanel* ShopControl::getRmbShopPanel() {
    if(m_pRmbShopPanel == NULL) {
        m_pRmbShopPanel = RmbShopPanel::create();
        m_pRmbShopPanel->retain(); // release at ShopControl::finalize()
    }

    return m_pRmbShopPanel;
}

CRestrictShopPanel* ShopControl::getRestrictShopPanel(){
	if(!m_stRestrictShopPanel.isInited()){
		m_stRestrictShopPanel.init();
	}

	return &m_stRestrictShopPanel;
}

CRestrictShopItemInfoPanel* ShopControl::getRestrictItemInfoPanel(){
	if(!m_stRestrictItemInfoPanel.isInited()){
		m_stRestrictItemInfoPanel.init();
	}

	return &m_stRestrictItemInfoPanel;
}

BuyMonthCardPanel* ShopControl::getBuyMonthCardPanel() {
	if(m_pBuyMonthCardPanel == NULL) {
		m_pBuyMonthCardPanel = BuyMonthCardPanel::create();
		CC_SAFE_RETAIN(m_pBuyMonthCardPanel);
	}

	return m_pBuyMonthCardPanel;
}

FirstChargePanel* ShopControl::getFirstChargePanel() {
	if(m_pFirstChargePanel == NULL) {
		m_pFirstChargePanel = FirstChargePanel::create();
		CC_SAFE_RETAIN(m_pFirstChargePanel);
	}

	return m_pFirstChargePanel;
}

void ShopControl::onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg) {
	unsigned int uCmd  = stMsg.msg_head().msg_id();

	switch(uCmd) {
		case vmsg::CS_CMD_ERR:
			
			break;
		case vmsg::CS_CMD_STORE_BUY:
            //购买成功
            POP_TIP_LAYER_MASTER->ShowTipStr(TextStr(TEXT_SHOP_BUY_DONE));
            break;
        case vmsg::CS_CMD_PLAYER_QUERY_BLACKMARKET:
            if(m_pBlackShopPanel != NULL && m_pBlackShopPanel->isOpen()) {
                m_pBlackShopPanel->updateUI(stMsg.playerrespond().queryblackmarket_rsp());
            }
            break;
        case vmsg::CS_CMD_PLAYER_BUY_BLACKMARKET:
            if(m_pBlackShopPanel != NULL && m_pBlackShopPanel->isOpen()) {
                m_pBlackShopPanel->updateUI(stMsg.playerrespond().buyblackmarket_rsp());
            }
            //更新碎片信息
            P_FRAG_CTRL->getProxy()->sendQueryDebrisBagRqst();
            break;
        case vmsg::CS_CMD_PLAYER_RESET_BLACKMARKET:
            if(m_pBlackShopPanel != NULL && m_pBlackShopPanel->isOpen()) {
                m_pBlackShopPanel->updateUI(stMsg.playerrespond().resetblackmarket_rsp() );
            }
            break;
        case vmsg::CS_CMD_GOLD_RMBSTORE_QUERY:

            break;
        case vmsg::CS_CMD_GOLD_RMBSTORE_BUY:

            break;
		case vmsg::CS_CMD_PANIC_BUYING_QUERY:
			if(m_stRestrictShopPanel.isInited() && m_stRestrictShopPanel.isOpen()) {
				m_stRestrictShopPanel.updateUI(stMsg.panicbuyingrespond().status_query_response());
			}
			break;
		case vmsg::CS_CMD_PANIC_BUYING:
			if(m_stRestrictShopPanel.isInited() && m_stRestrictShopPanel.isOpen()) {
				m_stRestrictShopPanel.updateUI(stMsg.panicbuyingrespond().buying_response().curstatus(), false);
			}

			P_BAG_CTRL->sendQueryBagInfoRqst();
			break;
		default:;
	}
}

void ShopControl::sendStoreBuyRqst(unsigned int uType, unsigned int uCfgID, unsigned int uCnt) {
	m_stShopProxy.sendStoreBuyRqst(uType, uCfgID, uCnt);
}


void ShopControl::setRMBBuyLoading(bool bIsShow) {
    if(!m_pRmbShopPanel) {
        return;        
    }
    if(bIsShow && m_pRmbShopPanel->isOpen()) {
        m_pRmbShopPanel->showLoading();
    } else {
        m_pRmbShopPanel->closeLoading();
    }
}

void ShopControl::sendRestrictShopQueryRqst(){
	m_stShopProxy.sendRestrictShopQueryRqst();
}

void ShopControl::sendRestrictShopBuyRqst(const int iItemIndex){
	m_stShopProxy.sendRestrictShopBuyRqst(iItemIndex);
}

