#include "ItemInfoPanel.h"
#include "NounHelper.h"
#include "BagControl.h"
#include "ClientSvrMsg.pb.h"
#include "ItemUtils.h"

#include "ItemCfgMaster.h"
#include "EquipmentCfgMaster.h"
#include "defines.h"
#include "ToolUtils.h"

using namespace vmsg;

ItemInfoPanel::ItemInfoPanel()
:m_pCloseBtn(NULL)
,m_pItemImg(NULL)
,m_pItemNameTxt(NULL)
,m_pItemTypeNameTxt(NULL)
,m_pSalePriceTxt(NULL)
,m_pSalePriceValTxt(NULL)
,m_pItemDescTxt(NULL)
,m_pUseBtn(NULL)
,m_nItemCfgID(0)
{
}

ItemInfoPanel::~ItemInfoPanel()
{
}

ItemInfoPanel* ItemInfoPanel::create() {
	ItemInfoPanel *pRet = new ItemInfoPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ItemInfoPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());
        
        setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/ItemInfoPanelUI.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(ItemInfoPanel::touchEventHandler));

        m_pBgItemImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_item_img"));
		m_pItemImg = dynamic_cast<UIImageView*>(m_pBgItemImg->getChildByName("item_img"));

		m_pItemNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("item_name_txt"));

		m_pItemTypeNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("item_type_name_txt"));
		
		m_pSalePriceTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("sale_price_txt"));

		m_pSalePriceValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("sale_price_val_txt"));

		m_pItemDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("item_desc_txt"));

		m_pUseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("use_btn"));
		m_pUseBtn->addTouchEventListener(this, toucheventselector(ItemInfoPanel::touchEventHandler));

		bRet = true;
	} while(0);

	return bRet;
}

void ItemInfoPanel::updateInfo(const int iItemCfgID, const int iItemType, const bool bIsShowUseBtn, const unsigned int uItemCnt, int nBagIdx) {
	m_nItemCfgID = iItemCfgID;
	m_nBagIdx = nBagIdx;
	bool btnEnable = true;
	std::string iconURL = ItemUtils::getItemIcon(iItemType, iItemCfgID);
	std::string iconName = ItemUtils::getItemName(iItemType, iItemCfgID);

	m_pSalePriceTxt->setVisible(iItemType == THING_TYPE_ITEM);
	m_pSalePriceValTxt->setVisible(iItemType == THING_TYPE_ITEM);
	//m_pItemDescTxt->setPositionY(m_pSalePriceTxt->isVisible() ? 200.0f : 228.0f);

    std::string typeName = "";
	if(iItemType == THING_TYPE_ITEM){  //item
		const ItemCfg *pItemCfg = ITEM_CFG_MASTER->GetItemCfgByID(iItemCfgID);
        if(NULL == pItemCfg) {
            CCLOG("[error]ItemInfoPanel::updateInfo, itme cfg is null,cfg id %d", iItemCfgID);
            return;
        }
		if(pItemCfg->itemtype() == 0) {
			typeName = NounStr(NOUN_ITEM);
		} else if(pItemCfg->itemtype() == 1) {
			typeName = NounStr(NOUN_MATEARIAL);
		} else if(pItemCfg->itemtype() == 2) {
			typeName = NounStr(NOUN_GIFT);
		}

		m_pItemTypeNameTxt->setText(typeName);
		m_pSalePriceValTxt->setText(intToString(pItemCfg->coinprice() * uItemCnt));

		if(bIsShowUseBtn) {
			if(pItemCfg->usable() == 1) {
				m_pUseBtn->setTitleText(NounStr(NOUN_USE));
			} else if(pItemCfg->cansell() == 1) {
				m_pUseBtn->setTitleText(NounStr(NOUN_BUY));
			}

			btnEnable = (pItemCfg->usable() == 1 || pItemCfg->cansell() == 1);
		} else {
			btnEnable = false;
		}
	}else if(iItemType == THING_TYPE_EQUIP){   //×°±¸
		if(bIsShowUseBtn) {
			btnEnable = true;
		} else {
			btnEnable = false;
		}
	}else if(iItemType == THING_TYPE_ARENA_MONEY){
		btnEnable = false;
	}else if(iItemType == THING_TYPE_COIN){
		btnEnable = false;
	}else if(iItemType == THING_TYPE_GOLD){
		btnEnable = false;
	} else {
        btnEnable = false;
    }
    if(typeName.size() <= 0) {
        m_pItemTypeNameTxt->setText(ItemUtils::getThingTypeName(iItemType));
    }
    
    m_pItemDescTxt->setText(ItemUtils::getItemDesc(iItemType, iItemCfgID));

    CToolUtils::adjustIconBgSize(iItemType, iItemCfgID, m_pBgItemImg);
    CToolUtils::adjustFragIcon(iItemType, m_pBgItemImg);

	m_pItemImg->loadTexture(iconURL.c_str());
	m_pItemNameTxt->setText(iconName.c_str());
	m_pUseBtn->setVisible(btnEnable);
	m_pUseBtn->setTouchEnabled(btnEnable);
}

void ItemInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
			close();
		} else if(std::strcmp(pszWidgetName, m_pUseBtn->getName()) == 0) {
			int nBagIdx = m_nBagIdx;

			if(NounStr(NOUN_BUY) == m_pUseBtn->getTitleText()) {
				P_BAG_CTRL->sendBagSellRqst(nBagIdx);
			} else if(NounStr(NOUN_USE) == m_pUseBtn->getTitleText()) {
                P_BAG_CTRL->sendOpenPackageRqst(nBagIdx, m_nItemCfgID);
            }
			
			close();
		}
		break;
	default:
		break;
	}
}
