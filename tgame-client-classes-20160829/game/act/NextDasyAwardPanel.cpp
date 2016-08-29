#include "NextDasyAwardPanel.h"

#include "Macros.pb.h"

#include "basic/ToolUtils.h"

#include "ActControl.h"
#include "ActRewardItem.h"
#include "basic/TimeEx.h"
#include "CfgMaster/NextDayAwardCfgMaster.h"
#include "IconCell.h"
#include "game/tip/TipControl.h"
#include "ItemUtils.h"
#define  ICON_NAME "icon_name"

NextDasyAwardPanel::NextDasyAwardPanel()
{
	
}

NextDasyAwardPanel::~NextDasyAwardPanel(){
}

bool NextDasyAwardPanel::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/OnlineLayoutUI.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void NextDasyAwardPanel::initUI(){

	m_descLab = dynamic_cast<Label*>(m_pRootLayout->getChildByName("desc_txt"));
	m_getTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("get_text_0"));
	m_getTxtTime = dynamic_cast<Label*>(m_pRootLayout->getChildByName("get_txt_time"));
	m_closeBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	//m_closeBtn->addTouchEventListener(this,toucheventselector(OnlineAwardPanel::touchEventHandler));	

	m_Item0 = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("item_1"));
	m_Item1 = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("item_2"));
	

	m_RewardBtnLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("get_Btn"));
	m_getBtn = RewardStateBtn::create();
	m_RewardBtnLayout->addChild(m_getBtn);
	m_RewardBtnLayout->setScale(1.2);

	
	m_descLab->setText(TextStr(TEXT_NEXTDAY_AWARD));

	m_bUIInited = true;
}




/*
void OnlineAwardPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		//if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
			close();
		//}
		break;
	default:;
	}
}
*/
void NextDasyAwardPanel::updateUI(){
	if(!isOpen()) {
		return;
	}

	if(this->getIsInLoading()){
		this->closeLoading();
	}
	m_leftTime = 0;

	vmsg::CSQueryAllPkgStatusRsp pActStatus =  P_ACT_CTRL->getStatusInfo();
	vmsg::NextDayPkgStatus pNextDasyStatus;
	for (int i = 0; i< pActStatus.allacts_size(); i++)
	{
		vmsg::CSCouponActStatusItem pStatusItem =pActStatus.allacts(i);
		if (pStatusItem.has_nextdaypkgstatus())
		{
			pNextDasyStatus = pStatusItem.nextdaypkgstatus();
		}
	}

	const NextDayCfg* pNextDayCfg = NextDayAward_CFG_MASTER->getNextDayAwardActCfg(1);
	unschedule(schedule_selector(NextDasyAwardPanel::scheduleTime));
	if(pNextDasyStatus.status() >= 1)
	{
		m_getTxtTime->setVisible(true);
		m_getTxt->setVisible(true);
		//显示倒计时
		{
			struct tm *tm;
			time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			time(&timep);
#else
			struct timeval now;
			gettimeofday(&now, NULL);
			timep = now.tv_sec;
#endif
			tm = localtime(&timep);
			tm->tm_hour = 23;
			tm->tm_min	 = 59;
			tm->tm_sec = 59;
			time_t nextdaytime = mktime(tm);
			m_leftTime = nextdaytime - timep;

			cocos2d::CCLog("lefttime:%s",TimeEx::getStringTimeForInt(m_leftTime).c_str());
			m_getTxtTime->setVisible(true);
			m_getTxtTime->setText(TimeEx::getStringTimeForInt(m_leftTime).c_str());
			schedule(schedule_selector(NextDasyAwardPanel::scheduleTime),1.0);
			m_getBtn->setState(RewardStateBtn::REWARD_BTN_UNREACHABLE);
		}
	}else
	{
		//可领取
		m_getTxtTime->setVisible(false);
		m_getTxt->setVisible(false);
		m_getBtn->setTag(1);
		m_getBtn->setState(RewardStateBtn::REWARD_BTN_CANGET);
	}
	
	for (int i = 0; i<pNextDayCfg->elmtlst_size(); i++)
	{
		ImageView* pItem = m_Item0;
		if (i == 1)
		{
			pItem = m_Item1;
		}
		IconCell* p_icon = IconCell::create();
		p_icon->setName(ICON_NAME);
		p_icon->showNumTxt();
		pItem->removeAllChildren();
		pItem->addChild(p_icon);
		p_icon->setInfo(pNextDayCfg->elmtlst(i).elementtype(),
			pNextDayCfg->elmtlst(i).elementcnt(),
			pNextDayCfg->elmtlst(i).elementid());
		UILabel* pLab = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(CCString::createWithFormat("item_name_%d",i+1)->getCString()));
		pLab->setText(ItemUtils::getItemName(pNextDayCfg->elmtlst(i).elementtype(),
			pNextDayCfg->elmtlst(i).elementid()));
		CToolUtils::adjustIconBgSize(pNextDayCfg->elmtlst(i).elementtype(), pNextDayCfg->elmtlst(i).elementid(),pItem);
		CToolUtils::adjustFragIcon(pNextDayCfg->elmtlst(i).elementtype(), pItem);
	}

	if (pNextDayCfg->elmtlst_size() == 2)
	{
		m_Item0->setEnabled(true);
		m_Item1->setEnabled(true);
		UILabel* pLab = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(CCString::createWithFormat("item_name_%d",1)->getCString()));
		pLab->setVisible(true);
		UILabel* pLab2 = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(CCString::createWithFormat("item_name_%d",2)->getCString()));
		pLab2->setVisible(true);
	}else
	{
		m_Item1->setEnabled(false);
		UILabel* pLab = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(CCString::createWithFormat("item_name_%d",2)->getCString()));
		pLab->setVisible(false);
	}
	
}

bool NextDasyAwardPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

	return true;
}
void NextDasyAwardPanel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	this->onTouch(pTouch->getLocation());
}

void NextDasyAwardPanel::onTouch(const CCPoint& stGlobalPt)
{
	if (m_closeBtn->hitTest(stGlobalPt))
	{
		close();
	}else
	{
		/*
		for (int i = 0; i<2 ; i++)
		{
			ImageView* pItem = m_Item0;
			if (i == 1)
			{
				pItem = m_Item1;
			}
			if (pItem->hitTest(stGlobalPt) && pItem->isEnabled())
			{
				IconCell* p_cell = dynamic_cast<IconCell*>(pItem->getChildByName(ICON_NAME) );
				P_TIP_CTRL->showItemTipPanel(p_cell->getItemType(),p_cell->getItemCfgid(),p_cell->getItemNum());
			}
			
		}*/
		
		if (m_getBtn->hitTest(stGlobalPt)&& m_getBtn->getState() == RewardStateBtn::REWARD_BTN_CANGET)
		{
			this->showLoading();
			P_ACT_CTRL->sendNextDayPkgRqst(m_getBtn->getTag());
		}
	}

}

void NextDasyAwardPanel::refresh(){
	BasePanel::refresh();

	P_ACT_CTRL->sendQueryAllPkgStatusRqst();
	this->showLoading();
}

void NextDasyAwardPanel::open(){
	BasePanel::open();

	P_ACT_CTRL->sendQueryAllPkgStatusRqst();
	this->showLoading();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, false);
}

void NextDasyAwardPanel::close(){
	BasePanel::close();
	this->closeLoading();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void NextDasyAwardPanel::scheduleTime(float dt)
{
	if (--m_leftTime >= 0)
	{
		m_getTxtTime->setText(TimeEx::getStringTimeForInt(m_leftTime));
	}else
	{
		unschedule(schedule_selector(NextDasyAwardPanel::scheduleTime));
		P_ACT_CTRL->sendQueryAllPkgStatusRqst();
	}
}
