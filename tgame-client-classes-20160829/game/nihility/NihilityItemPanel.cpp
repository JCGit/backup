#include "NihilityItemPanel.h"
#include "bag/BagControl.h"
#include "NihilityItemLayout.h"
#include "NihilityControl.h"
#include "fight/FightObj.h"
#include "fight/FightControl.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "NounHelper.h"

#include "NothinglandSweepItemCfgMaster.h"

CNihilityItemPanel::CNihilityItemPanel()
:m_pItemDemo(NULL)
,m_pSelectItemID(0)
{

}

CNihilityItemPanel::~CNihilityItemPanel(){
	CC_SAFE_RELEASE_NULL(m_pItemDemo);
}

bool CNihilityItemPanel::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/nihility_tool_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	}while(0);

	return bRet;
}

void CNihilityItemPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pEnterBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("enter_btn"));


	m_pList = dynamic_cast<ListView*>(m_pRootLayout->getChildByName("item_list"));
	m_pList->setItemsMargin(10);
	m_pList->addEventListenerListView(this, listvieweventselector(CNihilityItemPanel::listEventHandle));

	m_pItemDemo = CNihilityItemLayout::create();
	m_pItemDemo->retain();

	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CNihilityItemPanel::touchEventHandler));
	m_pEnterBtn->addTouchEventListener(this, toucheventselector(CNihilityItemPanel::touchEventHandler));
}

void CNihilityItemPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	vector<unsigned int> stItemVec;
	NothinglandSweepItemCfgSet stCfgSet = NIHLITY_SWEEP_ITEM_CFG_MASTER->GetCfg();
	for(int i = 0; i < stCfgSet.nothinglandsweepitemcfg_size(); i++){
		unsigned int uiCfgID = stCfgSet.nothinglandsweepitemcfg(i).itemid();
		if(P_BAG_CTRL->getItemCnt(uiCfgID) > 0){
			stItemVec.push_back(uiCfgID);
		}
	}

	//É¾³ý¶àÓàµÄÔªËØ
	while(int(m_pList->getItems()->count()) > stItemVec.size()){
		m_pList->removeLastItem(); 
	}

	CNihilityItemLayout* pItem;
	for(unsigned int idx = 0; idx < stItemVec.size(); idx++){
		pItem = dynamic_cast<CNihilityItemLayout*>(m_pList->getItem(idx));
		if(pItem == NULL){
			pItem = CNihilityItemLayout::createWithLayout(m_pItemDemo->getRootLayout());
			m_pList->pushBackCustomItem(pItem);
		}

		pItem->setCfgID(stItemVec.at(idx));
	}

	if(stItemVec.size() > 0){
		m_pEnterBtn->setEnabled(true);
	}else{
		m_pEnterBtn->setEnabled(false);
	}
}

void CNihilityItemPanel::listEventHandle(CCObject* pSender, ListViewEventType type){
	CNihilityItemLayout* pItem = (CNihilityItemLayout*)m_pList->getItem(m_pList->getCurSelectedIndex());
	if(!pItem){
		return;
	}

	switch (type)
	{
	case LISTVIEW_ONSELECTEDITEM_END:
		if(pItem->getSelected()){
			pItem->setIsEnable(true);
			pItem->setSelected(false);
			m_pSelectItemID = 0;
			setOtherEnable(pItem, true);
		}else{
			pItem->setIsEnable(true);
			pItem->setSelected(true);
			m_pSelectItemID = pItem->getCfgID();
			setOtherEnable(pItem, false);
		}
		break;
	default:
		break;
	}
}

void CNihilityItemPanel::setOtherEnable(CNihilityItemLayout* stItem, bool bEnable){
	CNihilityItemLayout* pItem;
	for(unsigned int idx = 0; idx < m_pList->getItems()->count(); idx++){
		pItem = dynamic_cast<CNihilityItemLayout*>(m_pList->getItem(idx));
		if(pItem->m_uID != stItem->m_uID){
			pItem->setIsEnable(bEnable);
			pItem->setSelected(false);
		}
	}
}

void CNihilityItemPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget* pWidget = dynamic_cast<UIWidget*>(pSender);
	const char* pName = pWidget->getName();
	string strName = pWidget->getName();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		if(strcmp(pName, "close_btn") == 0){
			this->close();
			P_NIHILITY_CTRL->getNihilityPanel()->open();
		}else if(strcmp(pName, "enter_btn") == 0){
			sendStart();
		}
		break;
	default:
		break;
	}
}

void CNihilityItemPanel::sendStart(){
	if(P_BAG_CTRL->isFull()){
		COMMON_DIALOG->show(TextStr(TEXT_BAG_FULL_OP_TIP), NULL, NounStr(OK));
		return;
	}

	if(m_pSelectItemID == 0){
		COMMON_DIALOG->show(TextStr(TEXT_NIHILITY_USE_ITEM_TIP), NULL, NounStr(OK));
		return;
	}

	this->showLoading();

	P_NIHILITY_CTRL->resetNihility();
	P_NIHILITY_CTRL->setNeedRush(1);
	//P_NIHILITY_CTRL->setSweepCfgID(m_pSelectItemID);
	P_FIGHT_OBJ->setFightType(NIHILITY_FIGHT_TYPE);
	P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();
}

void CNihilityItemPanel::open(){
	BasePanel::open();

	P_BAG_CTRL->sendQueryBagInfoRqst();

	this->showLoading();
}

void CNihilityItemPanel::close(){
	BasePanel::close();

	if(this->getIsInLoading()){
		this->closeLoading();
	}
}

