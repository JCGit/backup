#include "NihilityGenPanel.h"
#include "NihilityGenItem.h"
#include "NihilityControl.h"
#include "fight/FightObj.h"
#include "fight/FightControl.h"
#include "bag/BagControl.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "UserObj.h"
#include "tip/TipControl.h"
#include "firearms/FireArmsObj.h"
#include "manual/ManualControl.h"

#include "GenStrengthenCfgMaster.h"
#include "GlobalCfgMaster.h"

CNihilityGenPanel::CNihilityGenPanel(){
	m_fSliderMinY = 0;
	m_fSliderMaxY = 0;
}

CNihilityGenPanel::~CNihilityGenPanel(){

}

bool CNihilityGenPanel::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/nihility_gen_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	}while(0);

	return bRet;
}

void CNihilityGenPanel::initUI(){
	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pEnterBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("enter_btn"));
	m_pDescBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("rand_bg"));
	m_pSliderImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("slider_img"));
	m_pDescTxt = dynamic_cast<UILabel*>(m_pDescBg->getChildByName("rand_txt"));
	m_pList = dynamic_cast<ListView*>(m_pRootLayout->getChildByName("item_list"));
	m_pList->setItemsMargin(10);
	m_pList->addEventListenerScrollView(this, scrollvieweventselector(CNihilityGenPanel::listScrollEvent));

	m_fSliderMinY = m_pSliderImg->getPositionY();
	m_fSliderMaxY = m_pSliderImg->getPositionY() + m_pList->getSize().height - m_pSliderImg->getSize().height;

	m_pEnterBtn->addTouchEventListener(this, toucheventselector(CNihilityGenPanel::touchEventHandler));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CNihilityGenPanel::touchEventHandler));

	int iSize = GEN_STRENGTHEN_CFG_MASTER->GetCfg().genstrengthencfg_size();

	CNihilityGenItem* pDemo = CNihilityGenItem::create();
	CNihilityGenItem* pItem = NULL;
	for(int i = 0; i < iSize; i++){
		pItem = CNihilityGenItem::create(pDemo->getRootLayout());
		m_pList->pushBackCustomItem(pItem);
	}
}

void CNihilityGenPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	m_pSliderImg->setPositionY(m_fSliderMinY);

	const CSInfiniteGenInfo& stGenInfo = P_NIHILITY_CTRL->getGenInfo();
	unsigned int len = m_pList->getItems()->count();

	CNihilityGenItem* pItem;
	for(unsigned int i = 0; i < len; i++){
		pItem = dynamic_cast<CNihilityGenItem*>(m_pList->getItem(i));
		if(i < (unsigned int)stGenInfo.genlist_size()){
			const CSGen& stGen = stGenInfo.genlist(i);
			if(i == 0){
				updateRandDesc(stGen);
			}
			pItem->setObj(i, stGen);
		}
	}

	GUIDE_SHOW(GUIDE_UI_NOTHING_LAND_ENTER);
}

void CNihilityGenPanel::updateItemByIdx(unsigned int index, const CSGen& info){
	unsigned int len = m_pList->getItems()->count();
	if(index < 0 || index >= len){
		return;
	}

	if(index == 0){
		updateRandDesc(info);
	}

	CNihilityGenItem* pItem = dynamic_cast<CNihilityGenItem*>(m_pList->getItem(index));
	pItem->setObj(index, info);
}

void CNihilityGenPanel::updateRandDesc(const CSGen& info){
	if(info.cnt() == 0){
		m_pDescBg->setVisible(false);
		m_pDescTxt->setText("");
		return;
	}

	m_pDescBg->setVisible(true);
	m_pDescTxt->setText(P_NIHILITY_CTRL->getDescByGenInfo(info));
	m_pDescBg->setSize(CCSizeMake(m_pDescTxt->getSize().width + 40, m_pDescBg->getSize().height));
}

void CNihilityGenPanel::open(){
	BasePanel::open();

	P_NIHILITY_CTRL->sendInfiniteGenInfoRqst();

	this->showLoading();
}

void CNihilityGenPanel::close(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}
	BasePanel::close();
}

CCNode* CNihilityGenPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "enter_btn") {
		if(m_pEnterBtn != NULL && m_pEnterBtn->isRunning() && m_pEnterBtn->isTouchEnabled()) {
			pRet = m_pEnterBtn;
		}
	}else if (name == "change_gene_btn")
	{
		pRet = dynamic_cast<CNihilityGenItem*>(m_pList->getItem(0))->getGeneBtn();
	}

	return pRet;
}

void CNihilityGenPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "enter_btn") == 0) {
			GUIDE_TOUCH(pWidget);
			startFight();
		}else if(strcmp(pszWidgetName, "close_btn") == 0){
			this->close();
		}
		break;
	default:

		break;
	}
}

void CNihilityGenPanel::startFight(){
	if(P_USER_OBJ->checkManual(GLOBAL_CFG_MASTER->GetCfg().infinitemanual()) != 0){
		COMMON_DIALOG->show(TextStr(TEXT_MANUAL_BUY_TIP), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(CNihilityGenPanel::buyManualTip));
		return;
	}

	if(P_ARM_OBJ->isWeaponBagFull())  {
		COMMON_DIALOG->show(TextStr(WEAPON_BAG_FULL));
		return;
	}

	if(P_BAG_CTRL->isFull()){
		COMMON_DIALOG->show(TextStr(TEXT_BAG_FULL_OP_TIP), NULL, NounStr(OK));
		return;
	}


	this->showLoading();
	P_NIHILITY_CTRL->resetNihility();
	P_FIGHT_OBJ->setFightType(NIHILITY_FIGHT_TYPE);
	P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();
}

void CNihilityGenPanel::buyManualTip(){
	P_TIP_CTRL->LowManualBuyTip(this, dialogbtneventselector(CNihilityGenPanel::buyManual));
}

void CNihilityGenPanel::buyManual() {
	P_MANUAL_CTRL->sendBuyManualRqst();
}

void CNihilityGenPanel::listScrollEvent(CCObject* obj, ScrollviewEventType type){
	CCPoint stPos = m_pList->getInnerContainer()->getPosition();
	CCSize stSize = m_pList->getSize();
	CCSize stInSize = m_pList->getInnerContainerSize();
	float per;
	float disY;
	switch (type)
	{
	case SCROLLVIEW_EVENT_SCROLLING:
		per = stPos.y*1.0f/(stSize.height - stInSize.height);
		if(per > 1){
			per = 1;
		}else if(per < 0){
			per = 0;
		}

		disY = m_fSliderMinY - (1 - per)*(m_fSliderMaxY - m_fSliderMinY);
		m_pSliderImg->setPositionY(disY);
		break;
	}
}


