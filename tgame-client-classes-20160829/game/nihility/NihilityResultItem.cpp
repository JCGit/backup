#include "NihilityResultItem.h"
#include "IconCell.h"
#include "ToolUtils.h"
#include "tip/TipControl.h"

CNihilityResultItem::CNihilityResultItem(){

}

CNihilityResultItem::~CNihilityResultItem(){

}

CNihilityResultItem* CNihilityResultItem::create(UILayout *pRefLayout) {
	CNihilityResultItem *pRet = new CNihilityResultItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CNihilityResultItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/nihility_result_item_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

bool CNihilityResultItem::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CNihilityResultItem::initUI(){
	m_pBgImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("cell_bg"));
	m_pBgImg->setTouchEnabled(true);

	m_pCell = IconCell::create();
	m_pBgImg->addChild(m_pCell);

	m_pBgImg->addTouchEventListener(this, toucheventselector(CNihilityResultItem::touchEventHandler));
}

void CNihilityResultItem::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget* pWidget = dynamic_cast<UIWidget*>(pSender);
	const char* pName = pWidget->getName();
	string strName = pWidget->getName();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		if(strcmp(pName, "cell_bg") == 0){
			//显示材料掉落提示面板
			if(!(m_pCell->getItemCfgid() == 0 && m_pCell->getItemType() == 0 && m_pCell->getItemNum() == 0)){
				P_TIP_CTRL->showItemTipPanel(m_pCell->getItemType(), m_pCell->getItemCfgid(), m_pCell->getItemNum(), false);
			}
		}
		break;
	default:
		break;
	}
}

void CNihilityResultItem::setInfo(const int iType, const int iNum, const int iCfgID){
	m_pCell->setInfo(iType, iNum, iCfgID);

	CToolUtils::adjustIconBgSize(iType, iCfgID, m_pBgImg);
	CToolUtils::adjustFragIcon(iType, m_pBgImg);
}

