#include "StrengthenPanel.h"
#include "StrengthenItem.h"
#include "FightEndlessControl.h"
#include "fight/FightControl.h"
#include "fight/FightObj.h"

using namespace vmsg;

CStrengthenPanel::CStrengthenPanel()
:m_pEnterBtn(NULL)
,m_pDescTxt(NULL)
,m_pList(NULL)
{

}

CStrengthenPanel::~CStrengthenPanel(){

}

bool CStrengthenPanel::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/strengthen_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	}while(0);

	return bRet;
}

void CStrengthenPanel::initUI(){
	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pEnterBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("enter_btn"));
	m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));
	m_pLayer = dynamic_cast<UIPanel*>(m_pRootLayout->getChildByName("list_layer"));
	m_pList = dynamic_cast<ListView*>(m_pLayer->getChildByName("list"));
	m_pList->setItemsMargin(12);
	m_pDescTxt->setText("");

	m_pEnterBtn->addTouchEventListener(this, toucheventselector(CStrengthenPanel::touchEventHandler));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CStrengthenPanel::touchEventHandler));

	CStrengthenItem* pDemo = CStrengthenItem::create();
	CStrengthenItem* pItem;
	for(unsigned int i = 0; i < 5; i++){
		pItem = CStrengthenItem::create(pDemo->getRootLayout());
		m_pList->pushBackCustomItem(pItem);
	}
}

void CStrengthenPanel::updateUI(){
	const CSInfiniteGenInfo& stGenInfo = P_ENDLESS_CTRL->getGenInfo();
	int len = m_pList->getItems()->count();
	CStrengthenItem* pItem;

	for(int i = 0; i < len; i++){
		pItem = dynamic_cast<CStrengthenItem*>(m_pList->getItem(i));
		const CSGen& stGen = stGenInfo.genlist(i);
		if(i == 0){
			updateRandDesc(stGen);
		}
		pItem->setObj(i, stGen);
	}
}

void CStrengthenPanel::updateItemByIdx(unsigned int index, const CSGen& info){
	unsigned int len = m_pList->getItems()->count();
	if(index < 0 || index >= len){
		return;
	}

	if(index == 0){
		updateRandDesc(info);
	}

	CStrengthenItem* pItem = dynamic_cast<CStrengthenItem*>(m_pList->getItem(index));
	pItem->setObj(index, info);
}

void CStrengthenPanel::updateRandDesc(const CSGen& info){
	if(info.cnt() == 0){
		m_pDescTxt->setText("");
		return;
	}

	m_pDescTxt->setText(P_ENDLESS_CTRL->getDescByGenInfo(info));
}

void CStrengthenPanel::open(){
	BasePanel::open();

	P_ENDLESS_CTRL->sendInfiniteGenInfoRqst();
}

void CStrengthenPanel::close(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}
	BasePanel::close();
}

void CStrengthenPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "enter_btn") == 0) {
			P_FIGHT_OBJ->setFightType(ENDLESS_FIGHT_TYPE);
			P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();

			this->showLoading();
		}else if(strcmp(pszWidgetName, "close_btn") == 0){
			this->close();
		}
		break;
	default:

		break;
	}
}