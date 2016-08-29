#include "SweepInfoPanel.h"
#include "SweepInfoItem.h"
#include "LevelObj.h"
#include "defines.h"
#include "LevelControl.h"

#include "bag/BagControl.h"

using namespace vmsg;

CSweepInfoPanel::CSweepInfoPanel(){
	m_bIsError = false;
	m_uIDX = 0;
}

CSweepInfoPanel::~CSweepInfoPanel(){

}

bool CSweepInfoPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(false);

		initLayoutFromJsonFile("ui_v2/SweepInfoPanelUI.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pInfoList = dynamic_cast<ListView*>(m_pRootLayout->getChildByName("sweep_info_list"));
		m_pTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("bag_full_tip_txt"));
		m_pCloseBtn->setVisible(false);
		m_pCloseBtn->setTouchEnabled(false);
		m_pTipTxt->setVisible(false);

		/*m_pMinutesTxt = dynamic_cast<UILabelAtlas*>(m_pRootLayout->getChildByName("minute_txt"));
		m_pSecondsTxt = dynamic_cast<UILabelAtlas*>(m_pRootLayout->getChildByName("second_txt"));*/

		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CSweepInfoPanel::touchEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}

void CSweepInfoPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	const CSSweepWarRsp& stSweepInfo = P_LEVEL_OBJ->getSweepInfo();

	m_pInfoList->removeAllItems();
	m_pTipTxt->setVisible(false);
	m_pCloseBtn->setVisible(false);
	m_pCloseBtn->setTouchEnabled(false);

	m_uIDX = 0;
	m_bIsError = false;
	if(stSweepInfo.errorno() != 0){
		m_bIsError = true;
	}

	if(stSweepInfo.awardlst_size() <= 0){
		if(m_bIsError){
			m_pTipTxt->setVisible(true);
		}
		m_pCloseBtn->setVisible(true);
		m_pCloseBtn->setTouchEnabled(true);
		return;
	}

	this->schedule(schedule_selector(CSweepInfoPanel::stepUpdate), 0.8f);
	stepUpdate(0);
}

void CSweepInfoPanel::stepUpdate(float dt){
	const CSSweepWarRsp& stSweepInfo = P_LEVEL_OBJ->getSweepInfo();
	CSweepInfoItem* pItem = CSweepInfoItem::create();
	pItem->setInfo(stSweepInfo.awardlst(m_uIDX), m_uIDX);
	m_pInfoList->pushBackCustomItem(pItem);
	m_pInfoList->sortAllChildren();
	m_pInfoList->scrollToBottom(0.5f, true);

	/*m_pMinutesTxt->setStringValue(intToString(100 + (stSweepInfo.awardlst_size() - m_uIDX - 1) / 60).substr(1, 2));
	m_pSecondsTxt->setStringValue(intToString(100 + (stSweepInfo.awardlst_size() - m_uIDX - 1) % 60).substr(1, 2));*/

	if(m_uIDX >= (unsigned int)stSweepInfo.awardlst_size() - 1){  //×îºóÒ»ÂÖ
		this->unschedule(schedule_selector(CSweepInfoPanel::stepUpdate));

		if(m_bIsError){
			m_pTipTxt->setVisible(true);
		}
		m_pCloseBtn->setVisible(true);
		m_pCloseBtn->setTouchEnabled(true);
		return;
	}

	m_uIDX++;
}

void CSweepInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget* pWidget = dynamic_cast<UIWidget*>(pSender);
	const char* pName = pWidget->getName();
	string strName = pWidget->getName();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		if(strcmp(pName, "close_btn") == 0){
			this->close();

			P_LEVEL_CTL->getEnterPanel()->open();
			P_LEVEL_CTL->getEnterPanel()->setLevelID(P_LEVEL_OBJ->getSweepLevelID());
		}
		break;
	default:
		break;
	}
}

void CSweepInfoPanel::open(){
	BasePanel::open();

	//m_pCloseBtn->setVisible(false);
	m_pTipTxt->setVisible(false);

	this->showLoading();
}

void CSweepInfoPanel::close(){
	BasePanel::close();

	m_pInfoList->removeAllItems();
	this->unschedule(schedule_selector(CSweepInfoPanel::stepUpdate));

	P_BAG_CTRL->sendQueryBagInfoRqst();
}

