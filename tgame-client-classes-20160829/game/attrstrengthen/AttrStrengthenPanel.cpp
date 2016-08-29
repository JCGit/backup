#include "AttrStrengthenPanel.h"

#include "defines.h"

#include "Macros.pb.h"

#include "CfgMaster/AttrStrengthenCfgMaster.h"

#include "basic/CommonDialog.h"
#include "basic/NounHelper.h"
#include "basic/TextHelper.h"

#include "game/UserObj.h"
#include "game/IconCell.h"
#include "game/bag/BagControl.h"

#include "AttrStrengthenControl.h"
#include "AttrStrengthenObj.h"
#include "ItemCfgMaster.h"
#include "tip/TipControl.h"

AttrStrengthenPanel::AttrStrengthenPanel()
:m_pAttrIconImg(NULL)
,m_pAttrNameTxt(NULL)
,m_pAttrLevelValTxt(NULL)
,m_pAttrAddValTxt(NULL)
,m_pBgCostCoinImg(NULL)
,m_pBgCostItemImg(NULL)
,m_pStrengthenBtn(NULL)
,m_uAttrType(0)
,m_uMagicType(0)
{
}

AttrStrengthenPanel::~AttrStrengthenPanel()
{
}

AttrStrengthenPanel* AttrStrengthenPanel::create() {
	AttrStrengthenPanel *pRet = new AttrStrengthenPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool AttrStrengthenPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setUseBackForClose(true);

		initLayoutFromJsonFile("ui_v2/AttrStrengthenPanelUI.ExportJson");
		addChild(m_pRootLayout);

		m_pAttrIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("attr_icon_img"));

		m_pAttrNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("attr_name_txt"));

		m_pAttrLevelValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("attr_level_val_txt"));

		m_pAttrAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("attr_add_val_txt"));

		m_pBgCostCoinImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_cost_coin_img"));

		m_pBgCostItemImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_cost_item_img"));

		IconCell *pCostCoinCell = IconCell::create();
		pCostCoinCell->setName("cost_coin_cell");
		pCostCoinCell->showNumTxt();
		pCostCoinCell->setPosition(m_pBgCostCoinImg->getPosition());
		m_pRootLayout->addChild(pCostCoinCell);

		IconCell *pCostItemCell = IconCell::create();
		pCostItemCell->setName("cost_item_cell");
		pCostItemCell->showNumTxt();
		pCostItemCell->setPosition(m_pBgCostItemImg->getPosition());
		m_pRootLayout->addChild(pCostItemCell);

		m_pStrengthenBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("strengthen_btn"));
		m_pStrengthenBtn->addTouchEventListener(this, toucheventselector(AttrStrengthenPanel::touchEventHandler));

		bRet = true;
	} while(0);

	return bRet;
}

void AttrStrengthenPanel::setType(unsigned int uAttrType, unsigned int uMagicType) {
	m_uAttrType = uAttrType;
	m_uMagicType = uMagicType;
	
	const char *pszIconURL = "";

	switch(m_uMagicType) {
		case ICE_MAGIC:
			pszIconURL = m_uAttrType == vmsg::MAGIC_ATK ? "attr_strengthen/icon_ice_atk.png" : "attr_strengthen/icon_ice_def.png";
			break;
		case FLAME_MAGIC:
			pszIconURL = m_uAttrType == vmsg::MAGIC_ATK ? "attr_strengthen/icon_fire_atk.png" : "attr_strengthen/icon_fire_def.png";
			break;
		case POISON_MAGIC:
			pszIconURL = m_uAttrType == vmsg::MAGIC_ATK ? "attr_strengthen/icon_poison_atk.png" : "attr_strengthen/icon_poison_def.png";
			break;
		case BIOTECH_MAGIC:
			pszIconURL = m_uAttrType == vmsg::MAGIC_ATK ? "attr_strengthen/icon_biochemistry_atk.png" : "attr_strengthen/icon_biochemistry_def.png";
			break;
		case LIGHTNING_MAGIC:
			pszIconURL = m_uAttrType == vmsg::MAGIC_ATK ? "attr_strengthen/icon_thunder_atk.png" : "attr_strengthen/icon_thunder_def.png";
			break;
		default:;
	}

	m_pAttrIconImg->loadTexture(pszIconURL, UI_TEX_TYPE_PLIST);

}

void AttrStrengthenPanel::updateUI() {
    const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();

	std::string name = "";

	switch(m_uMagicType) {
		case ICE_MAGIC:
			name = m_uAttrType == vmsg::MAGIC_ATK ? NounStr(NOUN_ATK_ICE) : NounStr(NOUN_DEF_ICE);
			break;
		case FLAME_MAGIC:
			name = m_uAttrType == vmsg::MAGIC_ATK ? NounStr(NOUN_ATK_FIRE) : NounStr(NOUN_DEF_FIRE);
			break;
		case POISON_MAGIC:
			name = m_uAttrType == vmsg::MAGIC_ATK ? NounStr(NOUN_ATK_POISON) : NounStr(NOUN_DEF_POISON);
			break;
		case BIOTECH_MAGIC:
			name = m_uAttrType == vmsg::MAGIC_ATK ? NounStr(NOUN_ATK_BIOCHEMISTRY) : NounStr(NOUN_DEF_BIOCHEMISTRY);
			break;
		case LIGHTNING_MAGIC:
			name = m_uAttrType == vmsg::MAGIC_ATK ? NounStr(NOUN_ATK_THUNDER) : NounStr(NOUN_DEF_THUNDER);
			break;
		default:;
	}

	m_pAttrNameTxt->setText(name);

	const vmsg::CSAttrStrengthenInfo &stAttrStrengthenInfo = P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenInfo();

	unsigned int uLevel = m_uAttrType == vmsg::MAGIC_ATK ? stAttrStrengthenInfo.atkmagics(m_uMagicType) : stAttrStrengthenInfo.defmagics(m_uMagicType);

	char text[100];
	int step = uLevel % 10 == 0 ? uLevel / 10 : uLevel / 10 + 1;
	int stepLv = uLevel % 10 == 0 ? (uLevel > 0 ? 10 : 0) : uLevel % 10;
	snprintf(text, 100, TextStr(TEXT_ATTR_STEP_1 + step - 1), step, uLevel);
	m_pAttrLevelValTxt->setText(intToString(uLevel));
	//m_pAttrLevelValTxt->setText(text);
	//m_pAttrLevelValTxt->setVisible(uLevel > 0);

	AttrStrengthenCfg *pCurAttrStrengthenCfg = ATTR_STRENGTHEN_CFG_MASTER->getAttrStrengthenCfgByLevel(m_uAttrType, uLevel);

	if(pCurAttrStrengthenCfg != NULL) {
		m_pAttrAddValTxt->setText(intToString(pCurAttrStrengthenCfg->attrval()));
	} else {
		m_pAttrAddValTxt->setText(intToString(0));
	}

	AttrStrengthenCfg *pNextAttrStrengthenCfg = ATTR_STRENGTHEN_CFG_MASTER->getAttrStrengthenCfgByLevel(m_uAttrType, uLevel + 1);

	if(pNextAttrStrengthenCfg != NULL) {
		/*m_pNextAttrAddValTxt->setText(intToString(pNextAttrStrengthenCfg->attrval()));
		m_pProbabilityValTxt->setText(intToString(pNextAttrStrengthenCfg->prob() / 100) + "%");*/

		IconCell *pCostCoinCell = dynamic_cast<IconCell*>(m_pRootLayout->getChildByName("cost_coin_cell"));
		pCostCoinCell->setInfo(vmsg::THING_TYPE_COIN, pNextAttrStrengthenCfg->costcoin(), 0, ccc3(0x00, 0xff, 0x00));
		IconCell *pCostItemCell = dynamic_cast<IconCell*>(m_pRootLayout->getChildByName("cost_item_cell"));
		pCostItemCell->showNumTxt(pNextAttrStrengthenCfg->itemcnt() > 1);
		pCostItemCell->setInfo(vmsg::THING_TYPE_ITEM, pNextAttrStrengthenCfg->itemcnt(), pNextAttrStrengthenCfg->itemid(), ccc3(0x00, 0xff, 0x00));
	}

	m_pStrengthenBtn->setEnabled(pNextAttrStrengthenCfg != NULL);
}

void AttrStrengthenPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pStrengthenBtn->getName()) == 0) {
				const vmsg::CSAttrStrengthenInfo &stAttrStrengthenInfo = P_ATTR_STRENGTHEN_CTRL->getAttrStrengthenInfo();
				unsigned int uLevel = m_uAttrType == vmsg::MAGIC_ATK ? stAttrStrengthenInfo.atkmagics(m_uMagicType) : stAttrStrengthenInfo.defmagics(m_uMagicType);
				AttrStrengthenCfg *pNextAttrStrengthenCfg = ATTR_STRENGTHEN_CFG_MASTER->getAttrStrengthenCfgByLevel(m_uAttrType, uLevel + 1);

				if(pNextAttrStrengthenCfg != NULL) {
					UINT64_t uCoinCost = pNextAttrStrengthenCfg->costcoin();
					UINT64_t uCoin = P_USER_OBJ->getUserInfo()->coin;
					unsigned int uItemCnt = P_BAG_CTRL->getItemCnt(pNextAttrStrengthenCfg->itemid());

					if(!P_TIP_CTRL->CoinCostTip(uCoinCost, this)) {

					} else if(uItemCnt < pNextAttrStrengthenCfg->itemcnt()) {
						const char *pszItemName = ITEM_CFG_MASTER->GetItemName(pNextAttrStrengthenCfg->itemid());
						char text[128];
						snprintf(text, 128, TextStr(TEXT_ITEM_NOT_ENOUGH), pszItemName);
						COMMON_DIALOG->show(text, this, NounStr(OK), NounStr(CANCEL), NULL);
					} else {
						close();
						P_ATTR_STRENGTHEN_CTRL->sendAttrStrengthenRqst(m_uAttrType, m_uMagicType);
					}

				}

			}
			break;
		default:;
	}
}
