#include "WeaponRefitTipPanel.h"
#include "IconCell.h"
#include "ItemCfgMaster.h"
#include "NounHelper.h"
#include "ToolUtils.h"
#include "ClientSvrMsg.pb.h"
#include "ItemUtils.h"
#include "WeaponLibControl.h"
#include "tip/TipControl.h"

CWeaponRefitTipPanel::CWeaponRefitTipPanel()
{

}

CWeaponRefitTipPanel::~CWeaponRefitTipPanel(){

}

bool CWeaponRefitTipPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());
        
        //setUseBackForClose(true);
        setClickOutToClose(true);

        initLayoutFromJsonFile("ui_v2/weapon_refit_tip_panel.ExportJson");
        addChild(m_pRootLayout);

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(CWeaponRefitTipPanel::touchEventHandler));

        m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
        m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));


        m_pDropTipBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tip_btn"));
        m_pDropTipBtn->addTouchEventListener(this, toucheventselector(CWeaponRefitTipPanel::touchEventHandler));

        m_pImgCell = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell"));
        m_pCell = IconCell::create();
        m_pCell->setPosition(m_pImgCell->getPosition());
        m_pCell->setZOrder(3);
        m_pImgCell->setZOrder(4);
        m_pRootLayout->addChild(m_pCell);


		bRet = true;
	} while (0);

	return bRet;
}


void CWeaponRefitTipPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			close();			
        } else if(strcmp(pszWidgetName, "tip_btn") == 0) {
            P_TIP_CTRL->getItemDropTipListPanel()->showItemInfo(m_iType, m_iCfgID);
        }
		break;
	default:

		break;
	}
}


void CWeaponRefitTipPanel::open()
{
    BasePanel::open();
    updateUI();
}


void CWeaponRefitTipPanel::close()
{
    BasePanel::close();
    //reset

}


void CWeaponRefitTipPanel::updateUI()
{
    CCLOG("[trace]CWeaponRefitTipPanel,type %d, num %d, cfg id %d", m_iType, m_iNum, m_iCfgID);
    m_pCell->setInfo(m_iType, m_iNum, m_iCfgID);

    int iColor = ItemUtils::getItemColor(m_iType, m_iCfgID);
    m_pImgCell->loadTexture(ItemUtils::getColorBgImg(iColor).c_str(), UI_TEX_TYPE_PLIST);
    
    CToolUtils::adjustIconBgSize(m_iType, m_iCfgID, m_pImgCell);
    CToolUtils::adjustFragIcon(m_iType, m_pImgCell);

    m_pNameTxt->setText(ItemUtils::getItemName(m_iType, m_iCfgID));
    m_pDescTxt->setText(CToolUtils::splitUTFStr(ItemUtils::getItemDesc(m_iType, m_iCfgID), 10) );

    switch (m_iType)
    {
    case vmsg::THING_TYPE_MONTHCARD:
    case vmsg::THING_TYPE_GEN:
        m_bCanOpenDropTip = false;
        break;
    default:
        break;
    }

    m_pDropTipBtn->setEnabled(false);
    m_pDropTipBtn->setVisible(false);

}



