#include "WeaponLotResultCell.h"
#include "ItemUtils.h"
#include "IconCell.h"
#include "bag/BagControl.h"
#include "bag/ItemInfoPanel.h"
#include "ClientSvrMsg.pb.h"
#include "DebrisCfgMaster.h"
#include "ToolUtils.h"

using namespace vmsg;
WeaponLotResultCell* WeaponLotResultCell::create() {
    WeaponLotResultCell *pItemCell = new WeaponLotResultCell();

    if(pItemCell != NULL && pItemCell->init()) {
        pItemCell->autorelease();
    } else {
        CC_SAFE_DELETE(pItemCell);
    }

    return pItemCell;
}

bool WeaponLotResultCell::init() {
    bool bRet = false;
    do {
        CC_BREAK_IF(!BaseLayout::init());
        this->ignoreContentAdaptWithSize(false);
		setBackGroundColorOpacity(0);

        initLayoutFromJsonFile("ui_v2/weapon_lottery_result_cell.ExportJson");
        addChild(m_pRootLayout);

        m_pNameTxt= dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
        m_pCellBg= dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_bg"));

        m_pCellBg->setTouchEnabled(true);
        m_pCellBg->addTouchEventListener(this, toucheventselector(WeaponLotResultCell::touchEventHandler));

        m_pNameTxt->setText("");

        bRet = true;
    } while(false);

    return bRet;
}



void WeaponLotResultCell::setInfo(const int iType, const int iCnt, const int iCfgID) {

    if (NULL == m_pIconCell) {
        m_pIconCell = IconCell::create();
        m_pIconCell->setAnchorPoint(ccp(0.5f, 0.5f));
        m_pIconCell->showNumTxt(true);
        m_pCellBg->addChild(m_pIconCell);
    }

    //m_pIconCell->setPosition(m_pCellBg->getPosition());
    m_pIconCell->setInfo(iType, iCnt, iCfgID);

    m_pNameTxt->setText(ItemUtils::getItemName(iType, iCfgID));

    CToolUtils::adjustIconBgSize(iType, iCfgID, m_pCellBg);
    CToolUtils::adjustFragIcon(iType, m_pCellBg);
}


void WeaponLotResultCell::touchEventHandler(CCObject *pSender, TouchEventType type) 
{
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();

    switch(type) {
    case TOUCH_EVENT_ENDED:

        CCLOG("[trace]WeaponLotResultCell::touchEventHandler,click cell,cfgid=%d,type=%d", 
            m_pIconCell->getItemCfgid(), m_pIconCell->getItemType());

        /*if((m_pIconCell->getItemType() == THING_TYPE_ITEM || m_pIconCell->getItemType() == THING_TYPE_EQUIP)
            && m_pIconCell->getItemCfgid() > 0 ) {
                
        }*/
        P_BAG_CTRL->getItemInfoPanel()->open();
        P_BAG_CTRL->getItemInfoPanel()->updateInfo(m_pIconCell->getItemCfgid(), m_pIconCell->getItemType(), false);
    default:
        break;
    }
}

void WeaponLotResultCell::hideName(){
	m_pNameTxt->setVisible(false);
}

