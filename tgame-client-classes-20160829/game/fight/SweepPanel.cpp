#include "SweepPanel.h"
#include "bag/BagControl.h"
#include "LevelObj.h"
#include "UserObj.h"
#include "defines.h"
#include "TimeEx.h"
#include "FightControl.h"
#include "LevelControl.h"

#include "WarMapCfgMaster.h"

CSweepPanel::CSweepPanel(){
	m_uSweepNum = 0;
	m_uCanSweepNum = 0;
}

CSweepPanel::~CSweepPanel(){

}

bool CSweepPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/sweep_panel_ui.json");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pStartBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("start_btn"));
		m_pAddBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("add_btn"));
		m_pDownBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("down_btn"));

		m_pNumTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("num_txt"));
		m_pTimeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("time_txt"));
		m_pManualTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("cost_manual_txt"));
		m_pTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("full_tip_txt"));
		m_pTipTxt->setVisible(false);

		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CSweepPanel::touchEventHandler));
		m_pStartBtn->addTouchEventListener(this, toucheventselector(CSweepPanel::touchEventHandler));
		m_pAddBtn->addTouchEventListener(this, toucheventselector(CSweepPanel::touchEventHandler));
		m_pDownBtn->addTouchEventListener(this, toucheventselector(CSweepPanel::touchEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}

void CSweepPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();			
		}else if(strcmp(pszWidgetName, "start_btn") == 0){
			this->close();
			P_FIGHT_CTRL->getFightProxy()->sendSweepWarRqst(_levelID, m_uSweepNum);
			P_LEVEL_CTL->getSweepInfoPanel()->open();

			P_LEVEL_OBJ->setSweepLevelID(_levelID);
			P_LEVEL_OBJ->setSweepNum(m_uSweepNum);
		}else if(strcmp(pszWidgetName, "add_btn") == 0) {
			setSweepNum(true);
		}else if(strcmp(pszWidgetName, "down_btn") == 0) {
			setSweepNum(false);
		}
		break;
	default:
		break;
	}
}

void CSweepPanel::setLevelID(unsigned int var){
	_levelID = var;
	m_uSweepNum = 1;
	updateUI();
}

unsigned int CSweepPanel::getLevelID(){
	return _levelID;
}

void CSweepPanel::updateUI(){
	//背包已满
	if(P_BAG_CTRL->isFull()){
		m_pTipTxt->setVisible(true);
		m_pStartBtn->setBright(false);
		m_pStartBtn->setTouchEnabled(false);
	}else{
		m_pTipTxt->setVisible(false);
		m_pStartBtn->setBright(true);
		m_pStartBtn->setTouchEnabled(true);
	}

	//获取可扫荡的总次数
	google::protobuf::RepeatedPtrField<vmsg::CSWarMapInfo> mapInfos = P_LEVEL_OBJ->getMapInfo().infos();
	WarMapCfg* stCfg = WAR_MAP_CFG->GetWarMapCfgByID(_levelID);
	int idx = _levelID - P_LEVEL_OBJ->getMapInfo().fromid();
	unsigned int remainCnt = stCfg->repeatnum();		//记录剩余可挑战次数

	vmsg::CSWarMapInfo mapInfo;
	if(idx < mapInfos.size() && idx >= 0){  //已经通关
		mapInfo = mapInfos.Get(idx);
		remainCnt = stCfg->repeatnum() - mapInfo.fightcnt();
	}else{
		remainCnt = stCfg->repeatnum();
	}

	unsigned int manualNum = P_USER_OBJ->getUserInfo()->remainManual/stCfg->manual();
	if(remainCnt > manualNum){
		m_uCanSweepNum = manualNum;
	}else{
		m_uCanSweepNum = remainCnt;
	}

	updateSweepInfo();
}

void CSweepPanel::updateSweepInfo(){
	WarMapCfg* stCfg = WAR_MAP_CFG->GetWarMapCfgByID(_levelID);
	m_pNumTxt->setText(intToString(m_uSweepNum));
	m_pTimeTxt->setText(TimeEx::getStringTimeForInt(m_uSweepNum, "ms"));
	m_pManualTxt->setText(intToString(m_uSweepNum*stCfg->manual()));

	m_pAddBtn->setBright(true);
	m_pAddBtn->setTouchEnabled(true);
	m_pDownBtn->setBright(true);
	m_pDownBtn->setTouchEnabled(true);
	if(m_uSweepNum <= 1){
		m_pDownBtn->setBright(false);
		m_pDownBtn->setTouchEnabled(false);
	}

	if(m_uSweepNum >= m_uCanSweepNum){
		m_pAddBtn->setBright(false);
		m_pAddBtn->setTouchEnabled(false);
	}
}

void CSweepPanel::setSweepNum(bool isAdd){
	if(isAdd){
		if(m_uSweepNum < m_uCanSweepNum){
			m_uSweepNum++;
		}
	}else{
		if(m_uSweepNum > 1){
			m_uSweepNum--;
		}
	}

	updateSweepInfo();
}

void CSweepPanel::open(){
	BasePanel::open();
}