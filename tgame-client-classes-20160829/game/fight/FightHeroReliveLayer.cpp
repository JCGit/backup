#include "FightHeroReliveLayer.h"
#include "fight/FightControl.h"
#include "bag/BagControl.h"
#include "tip/TipControl.h"
#include "fight/FightObj.h"
#include "ImageNumber.h"
#include "TextHelper.h"
#include "defines.h"

#include "GlobalCfgMaster.h"

const int CFightHeroReliveLayer::HERO_RELIVE_COUNT_DOWN = 10;
const string CFightHeroReliveLayer::FIGHT_HERO_RELIVE_EVENT = "fight_hero_relive_event";
const string CFightHeroReliveLayer::FIGHT_HERO_CANCEL_RELIVE_EVENT = "fight_hero_cancel_relive_event";

CFightHeroReliveLayer::CFightHeroReliveLayer(){
	m_uReliveCost = 0;
	m_bIsCostGold = false;

	m_pUILayout = NULL;
	m_pLayer = NULL;
}

CFightHeroReliveLayer::~CFightHeroReliveLayer(){

}

bool CFightHeroReliveLayer::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

		float largeOffset = 100.0f;
		CCSize largeSize = CCSizeMake(size.width + largeOffset * 2, size.height + largeOffset * 2);

		setSize(size);
		setPosition(point);

		m_pLayer = UILayout::create();
		m_pLayer->setTouchEnabled(true);
		m_pLayer->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		m_pLayer->setBackGroundColorType(LAYOUT_COLOR_SOLID);
		m_pLayer->setBackGroundColorOpacity(0x70);
		m_pLayer->setSize(largeSize);
		addChild(m_pLayer);

		m_pUILayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("ui_v2/fight_hero_relive_ui.ExportJson"));
		m_pUILayout->setPosition(ccp(size.width/2 - m_pUILayout->getSize().width/2, size.height/2));
		m_pLayer->addChild(m_pUILayout);

		m_pDesTxt = dynamic_cast<Label*>(m_pUILayout->getChildByName("label"));

		m_pGlodIcon = dynamic_cast<ImageView*>(m_pUILayout->getChildByName("gold_icon"));
		m_pGoldNumTxt = dynamic_cast<Label*>(m_pGlodIcon->getChildByName("num_txt"));

		m_pReliveIcon = dynamic_cast<ImageView*>(m_pUILayout->getChildByName("relive_icon"));
		m_pReliveNumTxt = dynamic_cast<Label*>(m_pReliveIcon->getChildByName("num_txt"));

		m_pReviveBtn = dynamic_cast<Button*>(m_pUILayout->getChildByName("revive_btn"));
		m_pReviveBtn->addTouchEventListener(this, toucheventselector(CFightHeroReliveLayer::touchEventHandler));

		m_pCancelBtn = dynamic_cast<Button*>(m_pUILayout->getChildByName("cancel_btn"));
		m_pCancelBtn->addTouchEventListener(this, toucheventselector(CFightHeroReliveLayer::touchEventHandler));

		m_pTimeTxt = ImageNumber::create(30);
		m_pTimeTxt->setAnchorPoint(ccp(0.5f, 0.0f));
		CCPoint pt = m_pDesTxt->getPosition();
		pt.y = pt.y + m_pDesTxt->getSize().height + 100;
		m_pTimeTxt->setPosition(pt);
		m_pUILayout->addNode(m_pTimeTxt);
		m_pTimeTxt->setNum("10");

		bRet = true;
	} while(0);

	return bRet;
}

void CFightHeroReliveLayer::startCountDown(){
	unsigned int uReliveCfgID = vmsg::ITEM_RELIVE_ID;		//复活币ID 
	unsigned int uReliveNum = P_BAG_CTRL->getItemCnt(uReliveCfgID);
	if(uReliveNum <= 0){
		m_bIsCostGold = true;
		m_uReliveCost = P_FIGHT_OBJ->getReliveCost();

		if(m_uReliveCost == 0){  //使用首次复活消耗
			m_uReliveCost = GLOBAL_CFG_MASTER->GetCfg().relive1stcost();
		}
	}else{
		m_bIsCostGold = false;
	}

	if(m_bIsCostGold){
		m_pDesTxt->setVisible(false);
		m_pGlodIcon->setVisible(true);
		m_pReviveBtn->setTitleText(TextStr(TEXT_RELIVE_BTN_GOLD_LABEL_TIP));
	}else{
		m_pDesTxt->setVisible(true);
		m_pGlodIcon->setVisible(false);
		m_pReviveBtn->setTitleText(TextStr(TEXT_RELIVE_BTN_LABEL_TIP));
	}

	m_pGoldNumTxt->setText("x" + intToString(m_uReliveCost));
	m_pReliveNumTxt->setText("x" + intToString(uReliveNum));

	timeFlag = HERO_RELIVE_COUNT_DOWN;
	m_pTimeTxt->setNum(intToString(timeFlag));
	this->schedule(schedule_selector(CFightHeroReliveLayer::updateTime),1);
}

void CFightHeroReliveLayer::updateTime(float dt){
	if (timeFlag > 1){ 
		timeFlag--;
		m_pTimeTxt->setNum(intToString(timeFlag));
	}else{
		gameFailed();
		this->unschedule(schedule_selector(CFightHeroReliveLayer::updateTime));
	}
}

void CFightHeroReliveLayer::gameFailed(){
	this->removeFromParentAndCleanup(true);
	DispatchEvent(FIGHT_HERO_CANCEL_RELIVE_EVENT, this);
}

void CFightHeroReliveLayer::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, "revive_btn") == 0) { 
			doRelive();
		}else if(std::strcmp(pszWidgetName, "cancel_btn") == 0) { 
			gameFailed();
		}
		break;
	}
}

void CFightHeroReliveLayer::doRelive(){
	if(m_bIsCostGold){  //使用星钻
		P_TIP_CTRL->GoldCostTip(m_uReliveCost, this, dialogbtneventselector(CFightHeroReliveLayer::reliveFun), TextStr(TEXT_RELIVE_COST_GOLD_TIP));
	}else{
		unsigned int uReliveCfgID = vmsg::ITEM_RELIVE_ID;		//复活币ID 
		unsigned int uBagIdx = P_BAG_CTRL->getIdxByItemCfgID(uReliveCfgID);
		P_FIGHT_CTRL->getFightProxy()->sendReliveRqst(1, uReliveCfgID, uBagIdx);
	}
}

void CFightHeroReliveLayer::reliveFun(){
	P_FIGHT_CTRL->getFightProxy()->sendReliveRqst(0);
}

void CFightHeroReliveLayer::doReliveDone(){
	this->removeFromParentAndCleanup(true);
	DispatchEvent(FIGHT_HERO_RELIVE_EVENT, this);
}


