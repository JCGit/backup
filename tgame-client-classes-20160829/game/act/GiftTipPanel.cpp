#include "GiftTipPanel.h"
#include "IconCell.h"
#include "ItemCfgMaster.h"
#include "NounHelper.h"
#include "ToolUtils.h"
#include "ClientSvrMsg.pb.h"
#include "ItemUtils.h"
#include "tip/TipControl.h"
#include "CfgMaster/PackageCfgMaster.h"
GiftTipPanel::GiftTipPanel()
{

}

GiftTipPanel::~GiftTipPanel(){

}

bool GiftTipPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());
        
        //setUseBackForClose(true);
        setClickOutToClose(true);

        initLayoutFromJsonFile("ui_v2/GiftTipPanelUI.ExportJson");
        addChild(m_pRootLayout);

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(GiftTipPanel::touchEventHandler));

        m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));

		m_pItemList = dynamic_cast<ListView*>(m_pRootLayout->getChildByName("item_listview"));
		m_pItemList->setItemsMargin(150);
//		m_pItemList->addEventListenerListView(this, listvieweventselector(GiftTipPanel::listViewEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}

void GiftTipPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type)
{

}


void GiftTipPanel::touchEventHandler(CCObject* pSender, TouchEventType type){
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


void GiftTipPanel::open()
{
    BasePanel::open();
    updateUI();
}


void GiftTipPanel::close()
{
    BasePanel::close();
    //reset

}


void GiftTipPanel::updateUI()
{
	//return;
	m_pItemList->removeAllItems();
    CCLOG("[trace]CWeaponRefitTipPanel,type %d, num %d, cfg id %d", m_iType, m_iCfgID);
    const PackageCfg* mPackageCfg = PACKAGE_CFG_MASTER->GetPackageCfgByID(m_iCfgID);
	
	for (int i = 0; i<=mPackageCfg->elmtlst_size(); i++)
	{
		int desi = 0;
		if (i == mPackageCfg->elmtlst_size())
		{
			desi = 1;
		}
		const PackageElmt mElmt = mPackageCfg->elmtlst(i- desi);
		UILayout* pGiftLayout = UILayout::create();
		UIImageView* pImage = UIImageView::create();
		pImage->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
		pImage->setSize(CCSize(100,100));
		pImage->setPositionX(90);
		pGiftLayout->addChild(pImage);
		IconCell* pCell = IconCell::create();
		pCell->showNumTxt(true);
		pCell->setInfo(mElmt.elementtype(),mElmt.elementcntmax(),mElmt.elementid());
		pImage->addChild(pCell);
		Label* pLab = Label::create();
		pLab->setText(ItemUtils::getItemName(mElmt.elementtype(),mElmt.elementid()));
		pLab->setFontSize(22);
		pLab->setPositionY(-65);
		pLab->setColor(ccc3(255,144,0));
		pImage->addChild(pLab);
		CToolUtils::adjustIconBgSize(mElmt.elementtype(), mElmt.elementid(), pImage);
		CToolUtils::adjustFragIcon(mElmt.elementtype(), pImage);

		m_pItemList->pushBackCustomItem(pGiftLayout);
		if (desi == 1)
		{
			pGiftLayout->setVisible(false);
			break;
		}
	}
	const ItemCfg* pItemCfg = ITEM_CFG_MASTER->GetItemCfgByID(m_iCfgID);
	m_pDescTxt->setText(pItemCfg->itemname());
}



