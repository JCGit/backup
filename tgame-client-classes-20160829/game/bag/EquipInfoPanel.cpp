#include "EquipInfoPanel.h"

#include "EquipInfoCell.h"
#include "BagControl.h"
#include "UserObj.h"
#include "CfgMaster/EquipmentCfgMaster.h"
#include "NounHelper.h"
#include "basic/CommonDialog.h"
#include "basic/TextHelper.h"
#include "game/city/CityControl.h"
#include "ModuleID.pb.h"
#include "ItemUtils.h"
#include "equiplib/EquipLibControl.h"


EquipInfoPanel::EquipInfoPanel()
:m_pCloseBtn(NULL)
,m_pEquipListView(NULL)
,m_pSaleBtn(NULL)
,m_pEquipBtn(NULL)
,m_pDressedInfoCell(NULL)
,m_pSelectedInfoCell(NULL)
{
}

EquipInfoPanel::~EquipInfoPanel()
{
	CC_SAFE_RELEASE_NULL(m_pDressedInfoCell); // retain at EquipInfoPanel::init
	CC_SAFE_RELEASE_NULL(m_pSelectedInfoCell); // retain at EquipInfoPanel::init
}

EquipInfoPanel* EquipInfoPanel::create() {
	EquipInfoPanel *pRet = new EquipInfoPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EquipInfoPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/EquipInfoPanelUI.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(EquipInfoPanel::touchEventHandler));

		m_pEquipListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("equip_list"));

		m_pSaleBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("sale_btn"));
		m_pSaleBtn->addTouchEventListener(this, toucheventselector(EquipInfoPanel::touchEventHandler));

		m_pEquipBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("equip_btn"));
		m_pEquipBtn->addTouchEventListener(this, toucheventselector(EquipInfoPanel::touchEventHandler));


		m_pCultureBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("equip_culture"));
		//m_pCultureBtn->addTouchEventListener(this, toucheventselector(EquipInfoPanel::touchEventHandler));
		m_pCultureBtn->setVisible(false);
        
		m_pDressedInfoCell = EquipInfoCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("equip_layout")));
		m_pDressedInfoCell->retain(); // release at EquipInfoPanel::~EquipInfoPanel
		m_pDressedInfoCell->setStatus(EquipInfoCell::STATUS_DRESSED);

		m_pSelectedInfoCell = EquipInfoCell::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("equip_layout")));
		m_pSelectedInfoCell->retain(); // release at EquipInfoPanel::~EquipInfoPanel
		m_pSelectedInfoCell->setStatus(EquipInfoCell::STATUS_SELECTED);

		bRet = true;
	} while(0);

	return bRet;
}

CCNode* EquipInfoPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "equip_btn") {
		if(m_pEquipBtn != NULL && m_pEquipBtn->isRunning() && m_pEquipBtn->isVisible() && m_pEquipBtn->isTouchEnabled()) {
			pRet = m_pEquipBtn;
		}
	}

	return pRet;
}

void EquipInfoPanel::updateInfo(const vmsg::CSEquipInfo &stEquipInfo, unsigned int uEquipStatus) {
	m_stEquipInfo = stEquipInfo;
	m_pEquipListView->removeAllItems();

	if(uEquipStatus == EQUIP_IN_BAG) {
		const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());
		unsigned int uPart = pEquipCfg->part();
		unsigned int uBaseAttrVal = pEquipCfg->attr().attrval();
		unsigned int uStrengthenAttrVal = 0;
        /*
		EquipRefineCostCfg stEquipRefineCostCfg;
		if(EQUIP_REFINE_COST_CFG_MASTER->GetCfg(stEquipInfo.refinelv(), pEquipCfg->part(), stEquipRefineCostCfg) == 0) {
			unsigned int color = pEquipCfg->color();
			uStrengthenAttrVal = stEquipRefineCostCfg.colorattradd(color - 1);
		}*/
        uStrengthenAttrVal = ItemUtils::getEquipShowAttrVal(stEquipInfo.equipid(), stEquipInfo.level(), stEquipInfo.quality() );
		//EQUIP_REFINE_COST_CFG_MASTER->GetColorAttrAdd(stEquipInfo.refinelv(),pEquipCfg->part(), pEquipCfg->color()); //强化加属性


		m_pSelectedInfoCell->showAddVal(0);
		m_pEquipBtn->setTitleText(NounStr(NOUN_TAKE_ON));
		const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息

		for(unsigned int i = 0; i < pUserInfo->stEquipLst.size(); i++) {
			const vmsg::CSEquipInfo &stCurEquipInfo = pUserInfo->stEquipLst[i];
			const EquipmentCfg *pCurEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stCurEquipInfo.equipid());
			unsigned int uCurPart = pCurEquipCfg->part();

			if(uPart == uCurPart) {
				unsigned int uCurBaseAttrVal = pCurEquipCfg->attr().attrval();
				unsigned int uCurStrengthenAttrVal = 0;
				
                //unsigned int color = pCurEquipCfg->color();
                //uCurStrengthenAttrVal = EQUIP_REFINE_COST_CFG_MASTER->GetColorAttrAdd(stCurEquipInfo.refinelv(), pCurEquipCfg->part(), color); //强化加属性
                uCurStrengthenAttrVal = ItemUtils::getEquipShowAttrVal(stCurEquipInfo.equipid(), stCurEquipInfo.level(), stCurEquipInfo.quality() );

				m_pSelectedInfoCell->showAddVal(uBaseAttrVal + uStrengthenAttrVal - uCurBaseAttrVal - uCurStrengthenAttrVal);
				m_pDressedInfoCell->updateInfo(stCurEquipInfo);
				m_pEquipListView->pushBackCustomItem(m_pDressedInfoCell);

				m_pEquipBtn->setTitleText(NounStr(NOUN_REPLACE));
				break;
			}
		}

		m_pSelectedInfoCell->updateInfo(stEquipInfo);
		m_pEquipListView->pushBackCustomItem(m_pSelectedInfoCell);

		m_pSaleBtn->setVisible(true);
		m_pSaleBtn->setTouchEnabled(true);
	} else if(uEquipStatus == EQUIP_ON_ROLE) {
		m_pDressedInfoCell->updateInfo(stEquipInfo);
        m_pEquipListView->pushBackCustomItem(m_pDressedInfoCell);

        m_pSelectedInfoCell->showAddVal(0);

		m_pEquipBtn->setTitleText(NounStr(NOUN_TAKE_OFF));

		m_pSaleBtn->setVisible(false);
		m_pSaleBtn->setTouchEnabled(false);

        m_pEquipBtn->setVisible(true);
        m_pEquipBtn->setTouchEnabled(true);
        
    } else if(uEquipStatus == EQUIP_INFO) {
        const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());
		unsigned int uPart = pEquipCfg->part();
		unsigned int uBaseAttrVal = pEquipCfg->attr().attrval(); 

        m_pSelectedInfoCell->showAddVal(0);
        m_pSelectedInfoCell->updateInfo(stEquipInfo);
        m_pEquipListView->pushBackCustomItem(m_pSelectedInfoCell);

        m_pEquipBtn->setVisible(false);
        m_pEquipBtn->setTouchEnabled(false);

        m_pSaleBtn->setVisible(false);
        m_pSaleBtn->setTouchEnabled(false);
    }

	//m_pStrengthenBtn->setEnabled(P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_EQUIP_REFINE));

	GUIDE_SHOW(GUIDE_UI_EQUIP_INFO);
}

void EquipInfoPanel::updateSimpleEquipInfo(const int iEquipCfgID) {
    vmsg::CSEquipInfo stEquipInfo;
    stEquipInfo.set_equipguid(0);
    stEquipInfo.set_level(1);
    stEquipInfo.set_quality(1);
    stEquipInfo.set_equipid(iEquipCfgID);
    updateInfo(stEquipInfo, EquipInfoPanel::EQUIP_INFO);
}

void EquipInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(std::strcmp(pszWidgetName, m_pSaleBtn->getName()) == 0) {
				COMMON_DIALOG->show(TextStr(TEXT_SALE_ITEM), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(EquipInfoPanel::onSaleDialog));
			} else if(std::strcmp(pszWidgetName, m_pEquipBtn->getName()) == 0) {
				if(NounStr(NOUN_TAKE_OFF) == m_pEquipBtn->getTitleText()) {
					close();
					P_BAG_CTRL->sendTakeOffEquipRqst(m_stEquipInfo.equipguid());
				} else {
					GUIDE_TOUCH(pWidget);
                    /*
                    //穿装备不限制等级了
					const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(m_stEquipInfo.equipid());
					if(P_USER_OBJ->getUserInfo()->level >= pEquipCfg->equiplv()) {
						close();
						P_BAG_CTRL->sendTakeOnEquipRqst(P_BAG_CTRL->getIdxByEquipGUID(m_stEquipInfo.equipguid()));
					} else {
						COMMON_DIALOG->show(TextStr(TEXT_LEVEL_UNREACHED), this, NounStr(OK), NounStr(CANCEL), NULL);
					}*/
                    close();
                    P_BAG_CTRL->sendTakeOnEquipRqst(P_BAG_CTRL->getIdxByEquipGUID(m_stEquipInfo.equipguid()));
				}
			}
			else if (std::strcmp(pszWidgetName, m_pCultureBtn->getName()) == 0) {

				//TODO: jacey.
				// 装备属性界面增加《培养》按钮

			}/* else if(std::strcmp(pszWidgetName, m_pStrengthenBtn->getName()) == 0) {
				close();
                P_EQUIP_CTRL->showRefineWithEquip(m_stEquipInfo);
			}*/
			break;
		default:
			break;
	}
}

void EquipInfoPanel::onSaleDialog() {
	int nBagIdx = P_BAG_CTRL->getIdxByEquipGUID(m_stEquipInfo.equipguid());
	P_BAG_CTRL->sendBagSellRqst(nBagIdx);
	close();
}
