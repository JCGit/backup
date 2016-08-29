#include "OnlineAwardPanel.h"

#include "Macros.pb.h"

#include "basic/ToolUtils.h"

#include "ActControl.h"
#include "ActRewardItem.h"
#include "basic/TimeEx.h"
#include "CfgMaster/OnlineAwardCfgMaster.h"
#include "IconCell.h"

#include "game/tip/TipControl.h"
#include "ItemUtils.h"
#define  ICON_NAME "icon_name"

OnlineAwardPanel::OnlineAwardPanel()
{
	
}

OnlineAwardPanel::~OnlineAwardPanel(){
}

bool OnlineAwardPanel::init(){
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

void OnlineAwardPanel::initUI(){

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

	
	m_descLab->setText(TextStr(TEXT_ONLINE_AWARD));

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
void OnlineAwardPanel::updateUI(){
	if(!isOpen()) {
		return;
	}

	if(this->getIsInLoading()){
		this->closeLoading();
	}
	m_leftTime = 0;

	vmsg::CSQueryAllPkgStatusRsp pActStatus =  P_ACT_CTRL->getStatusInfo();
	vmsg::OnlinePkgStatus pOnlineStatus;
	for (int i = 0; i< pActStatus.allacts_size(); i++)
	{
		vmsg::CSCouponActStatusItem pStatusItem =pActStatus.allacts(i);
		if (pStatusItem.has_onlinepkgstatus())
		{
			pOnlineStatus = pStatusItem.onlinepkgstatus();
		}
	}

	const OnlinePKGCfg* pOnlineCfg = OnlineAward_CFG_MASTER->getOnlinePKGCfg(pOnlineStatus.status()+1);
	m_leftTime =   pOnlineCfg->onlinetime() - pOnlineStatus.onlinetime();
	
	unschedule(schedule_selector(OnlineAwardPanel::scheduleTime));
	if (m_leftTime>0)
	{
		//显示倒计时
		m_getTxtTime->setVisible(true);
		m_getTxt->setVisible(true);
		m_getTxtTime->setText(TimeEx::getStringTimeForInt(m_leftTime));
		schedule(schedule_selector(OnlineAwardPanel::scheduleTime),1.0);
		m_getBtn->setState(RewardStateBtn::REWARD_BTN_UNREACHABLE);
	}else
	{
		//可领取
		m_getTxtTime->setVisible(false);
		m_getTxt->setVisible(false);
		m_getBtn->setTag(pOnlineStatus.status()+1);
		m_getBtn->setState(RewardStateBtn::REWARD_BTN_CANGET);
	}

	for (int i = 0; i<pOnlineCfg->elmtlst_size(); i++)
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
		p_icon->setInfo(pOnlineCfg->elmtlst(i).elementtype(),
			pOnlineCfg->elmtlst(i).elementcnt(),
			pOnlineCfg->elmtlst(i).elementid());
		UILabel* pLab = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName(CCString::createWithFormat("item_name_%d",i+1)->getCString()));
		pLab->setText(ItemUtils::getItemName(pOnlineCfg->elmtlst(i).elementtype(),
			pOnlineCfg->elmtlst(i).elementid()));
		CToolUtils::adjustIconBgSize(pOnlineCfg->elmtlst(i).elementtype(), pOnlineCfg->elmtlst(i).elementid(),pItem);
		CToolUtils::adjustFragIcon(pOnlineCfg->elmtlst(i).elementtype(), pItem);
	}

	if (pOnlineCfg->elmtlst_size() == 2)
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

bool OnlineAwardPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

	return true;
}
void OnlineAwardPanel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	this->onTouch(pTouch->getLocation());
}

void OnlineAwardPanel::onTouch(const CCPoint& stGlobalPt)
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
		}
		*/
		if (m_getBtn->hitTest(stGlobalPt)&& m_getBtn->getState() == RewardStateBtn::REWARD_BTN_CANGET)
		{
			this->showLoading();
			P_ACT_CTRL->sendOnlinePkgRqst(m_getBtn->getTag());
		}
	}

}

void OnlineAwardPanel::refresh(){
	BasePanel::refresh();

	P_ACT_CTRL->sendQueryAllPkgStatusRqst();
	this->showLoading();
}

void OnlineAwardPanel::open(){
	BasePanel::open();

	P_ACT_CTRL->sendQueryAllPkgStatusRqst();
	this->showLoading();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, false);
}

void OnlineAwardPanel::close(){
	BasePanel::close();
	this->closeLoading();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void OnlineAwardPanel::scheduleTime(float dt)
{
	if (--m_leftTime >= 0)
	{
		m_getTxtTime->setText(TimeEx::getStringTimeForInt(m_leftTime));
	}else
	{
		unschedule(schedule_selector(OnlineAwardPanel::scheduleTime));
		P_ACT_CTRL->sendQueryAllPkgStatusRqst();
	}
}
