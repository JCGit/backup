#include "TopEntryLayout.h"

#include "defines.h"

#include "CfgMaster/EntryGroupCfgMaster.h"

#include "basic/ToolUtils.h"

#include "game/guide/GuideControl.h"

#include "EntryControl.h"
#include "act/ActControl.h"
#include "CfgMaster/OnlineAwardCfgMaster.h"
#include "basic/TimeEx.h"
#include "CfgMaster/NextDayAwardCfgMaster.h"

#define BTN_PREFIX_NAME "btn_" // 按钮前缀
#define MAX_NUM_PER_ROW 7 // 每行最多放多少个
#define BTN_WIDTH 100
#define BTN_HEIGHT 100
#define ROW_MARGIN 8 // 行间距
#define COL_MARGIN 8 // 列间距
#define BTN_TIP_CHILD_TAG 5 // 按钮上数字的tag

TopEntryLayout::TopEntryLayout()
{
	m_removeEnterVec.clear();
}

TopEntryLayout::~TopEntryLayout()
{
}

TopEntryLayout* TopEntryLayout::create(UILayout *pRefLayout) {
	TopEntryLayout *pRet = new TopEntryLayout();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool TopEntryLayout::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);

		bRet = true;
	} while(0);
	
	return bRet;
}

UIButton* TopEntryLayout::getEntryBtn(unsigned int uEntryID) {
	UIButton *pRet = NULL;
	std::map<unsigned int, UIButton*>::const_iterator iter = m_stBtnMap.find(uEntryID);

	if(iter != m_stBtnMap.end()) {
		pRet = iter->second;
	}

	return pRet;
}

void TopEntryLayout::addEntryBtn(unsigned int uEntryID) {
	UIButton *pBtn = getEntryBtn(uEntryID);

	if(pBtn == NULL) {
		pBtn = P_ENTRY_CTRL->createEntryBtn(uEntryID);
		m_stBtnMap[uEntryID] = pBtn;
		pBtn->retain(); 
	}

	if(pBtn->getParent() == NULL) {
		pBtn->setPosition(ccp(m_pRootLayout->getSize().width / 2, m_pRootLayout->getSize().height / 2));
		pBtn->setVisible(false); // 展开时才可见
		pBtn->setTouchEnabled(false);
		pBtn->addTouchEventListener(this, toucheventselector(TopEntryLayout::touchEventHandler));

		m_pRootLayout->addChild(pBtn);
	}
}

void TopEntryLayout::removeEntryBtn(unsigned int uEntryID) {
	UIButton *pBtn = getEntryBtn(uEntryID);

	if(pBtn != NULL && pBtn->getParent() != NULL) {
		pBtn->removeFromParent();
		m_stBtnMap.erase(uEntryID);
		pBtn->release(); 
	}
}

void TopEntryLayout::updateEntry() {
	// update entry
	m_stRenderList.clear();
	const EntryGroupCfg *pEntryGroupCfg = ENTRY_GROUP_CFG_MASTER->getEntryGroupCfg(ENTRY_GROUP_TOP);

	for(int i = 0; i < pEntryGroupCfg->entryid_size(); i++) {
		unsigned int uEntryID = pEntryGroupCfg->entryid(i);
		
		bool pIsOpen = true;
		for (auto pRemoveID : m_removeEnterVec)
		{
			if (uEntryID == pRemoveID)
			{
				pIsOpen = false;
				break;
			}
		}
		if(pIsOpen && P_ENTRY_CTRL->isEntryOpen(uEntryID)) {
			m_stRenderList.push_back(uEntryID);

			addEntryBtn(uEntryID);
		} else {
			removeEntryBtn(uEntryID);
		}
	}

	CCSize size = m_pRootLayout->getSize();
	CCPoint pos = ccp(size.width / 2, size.height / 2);

	for(unsigned int i = 0; i < m_stRenderList.size(); i++) {
		UIButton *pBtn = getEntryBtn(m_stRenderList[i]);

		if(pBtn != NULL) {
			float posX = pos.x - i % MAX_NUM_PER_ROW * (BTN_WIDTH + COL_MARGIN);
			float posY = pos.y - i / MAX_NUM_PER_ROW * (BTN_HEIGHT + ROW_MARGIN);

			pBtn->setPosition(ccp(posX, posY));
			pBtn->setVisible(true);
			pBtn->setTouchEnabled(true);
		}
	}
}

CCPoint TopEntryLayout::calcEntryPos(unsigned int uEntryID) {
	CCSize size = m_pRootLayout->getSize();
	CCPoint pos = ccp(size.width / 2, size.height / 2);

	for(unsigned int i = 0; i < m_stRenderList.size(); i++) {
		if(uEntryID == m_stRenderList[i]) {
			float posX = pos.x - i % MAX_NUM_PER_ROW * (BTN_WIDTH + COL_MARGIN);
			float posY = pos.y - i / MAX_NUM_PER_ROW * (BTN_HEIGHT + ROW_MARGIN);

			pos = ccp(posX, posY);
		}
	}

	return pos;
}

void TopEntryLayout::badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum) {
	UIButton *pBtn = getEntryBtn(uEntryID);

	if(pBtn != NULL) {
		float offsetX = pBtn->getSize().width * 0.32f;
		float offsetY = pBtn->getSize().height * 0.32f;
		//CToolUtils::badgeWidgetWithPoint(pBtn, uBadgeNum, offsetX, offsetY);
		CToolUtils::effectEntryIcon(pBtn, uBadgeNum > 0);
	}
}

void TopEntryLayout::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(string(pszWidgetName).substr(0, string(ENTRY_PREFIX_NAME).size()) == ENTRY_PREFIX_NAME) {
				string btnName = pszWidgetName;
				unsigned int uPrefixSize = string(ENTRY_PREFIX_NAME).size();
				string strEntryID = btnName.substr(uPrefixSize, btnName.size());
				unsigned int uEntryID = atol(strEntryID.c_str());
				GUIDE_TOUCH(pWidget);
				P_ENTRY_CTRL->enter(uEntryID);
			}
			break;
		default:;
	}
}

void TopEntryLayout::updateOnlineAndMorrow()
{
	
	m_removeEnterVec.clear();
	m_leftTimeMap.clear();
	
	UIButton* pOnlineBtn = getEntryBtn(ENTRY_ONLINE_AWARD);
	UIButton* pMorrowBtn = getEntryBtn(ENTRY_MORROW_AWARD);
	vmsg::CSQueryAllPkgStatusRsp pActStatus =  P_ACT_CTRL->getStatusInfo();
	vmsg::OnlinePkgStatus pOnlineStatus;
	vmsg::NextDayPkgStatus pMorrowStatus;
	for (int i = 0; i< pActStatus.allacts_size(); i++)
	{
		vmsg::CSCouponActStatusItem pStatusItem =pActStatus.allacts(i);
		if (pStatusItem.has_onlinepkgstatus())
		{
		 	pOnlineStatus = pStatusItem.onlinepkgstatus();
			
		}
		if (pStatusItem.has_nextdaypkgstatus())
		{
			pMorrowStatus = pStatusItem.nextdaypkgstatus();
		}
	}
	cocos2d::CCLog("[updateOnlineAndMorrow step]_0");
	bool pIsCanSchedule = false;
	
	if(pOnlineStatus.status() == OnlineAward_CFG_MASTER->getOnlinePKGCfg(OnlineAward_CFG_MASTER->getOnlineAwardTypeNum())->level())
	{
		m_removeEnterVec.push_back(ENTRY_ONLINE_AWARD);
		
	}else
	{
		const OnlinePKGCfg* pOnlineCfg = OnlineAward_CFG_MASTER->getOnlinePKGCfg(pOnlineStatus.status()+1);
		int pLeftTime =   pOnlineCfg->onlinetime() - pOnlineStatus.onlinetime();
		if (!pOnlineBtn)
		{
			updateEntry();
			pOnlineBtn = getEntryBtn(ENTRY_ONLINE_AWARD);
		}
		Label* pLab = dynamic_cast<Label*>(pOnlineBtn->getChildByTag(Entry_Time_Lab));
		if (pLeftTime>0)
		{
			//显示倒计时
			pLab->setVisible(true);
			pLab->setText(TimeEx::getStringTimeForInt(pLeftTime));
			m_leftTimeMap[ENTRY_ONLINE_AWARD] = pLeftTime;
			pIsCanSchedule = true;
		}else
		{
			//可领取
			pLab->setVisible(false);
		}
		
		

	}
	cocos2d::CCLog("[updateOnlineAndMorrow step]_1");
	//次日礼包
	if (pMorrowStatus.status()>=0)
	{
		const NextDayCfg* pNextDayCfg = NextDayAward_CFG_MASTER->getNextDayAwardActCfg(1);
		if (!pMorrowBtn)
		{
			updateEntry();
			pMorrowBtn = getEntryBtn(ENTRY_MORROW_AWARD);
		}
		Label* pLab = dynamic_cast<Label*>(pMorrowBtn->getChildByTag(Entry_Time_Lab));
		if(pMorrowStatus.status() >= 1)
		{
			//显示倒计时
			{
				struct tm *tm;
				time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				time(&timep);
#else
				struct cc_timeval now;
				CCTime::gettimeofdayCocos2d(&now, NULL);
				timep = now.tv_sec;
#endif
				tm = localtime(&timep);
				tm->tm_hour = 23;
				tm->tm_min	 = 59;
				tm->tm_sec = 59;
				time_t nextdaytime = mktime(tm);
				int lefttime = nextdaytime - timep;

				cocos2d::CCLog("lefttime:%s",TimeEx::getStringTimeForInt(lefttime).c_str());
				pLab->setVisible(true);
				pLab->setText(TimeEx::getStringTimeForInt(lefttime).c_str());
				m_leftTimeMap[ENTRY_MORROW_AWARD] = lefttime;
				pIsCanSchedule =true;
			}
		}else
		{
			//可领取
			pLab->setVisible(false);

		}
	}else
	{
		m_removeEnterVec.push_back(ENTRY_MORROW_AWARD);
	}
	cocos2d::CCLog("[updateOnlineAndMorrow step]_2");
	updateEntry();

	unschedule(schedule_selector(TopEntryLayout::scheduelOnlineTime));
	if (pIsCanSchedule)
	{
		schedule(schedule_selector(TopEntryLayout::scheduelOnlineTime), 1.0);
	}
	
	//UPDATE ENTRYPanel
	if (P_ACT_CTRL->getOnlinePanel()->isOpen())
	{
		bool pIsCanClose = false;
		for (auto pEntryId : m_removeEnterVec)
		{
			if (pEntryId == ENTRY_ONLINE_AWARD)
			{
				pIsCanClose = true;
				break;
			}
		}
		if (pIsCanClose)
		{
			P_ACT_CTRL->getOnlinePanel()->close();
		}else
		{
			P_ACT_CTRL->getOnlinePanel()->updateUI();
		}
		
	}
	
	if (P_ACT_CTRL->getNextDayPanel()->isOpen())
	{
		bool pIsCanClose = false;
		for (auto pEntryId : m_removeEnterVec)
		{
			if (pEntryId == ENTRY_MORROW_AWARD)
			{
				pIsCanClose = true;
				break;
			}
		}
		if (pIsCanClose)
		{
			P_ACT_CTRL->getNextDayPanel()->close();
		}else
		{
			P_ACT_CTRL->getNextDayPanel()->updateUI();
		}

	}
}

void TopEntryLayout::scheduelOnlineTime(float dt)
{
	
	UIButton* pOnlineBtn = getEntryBtn(ENTRY_ONLINE_AWARD);
	UIButton* pMorrowBtn = getEntryBtn(ENTRY_MORROW_AWARD);
	bool pIsCanQueryStatue = false;
	if (pOnlineBtn)
	{
		Label* pTime = dynamic_cast<Label*>(pOnlineBtn->getChildByTag(Entry_Time_Lab));
		if (pTime->isVisible())
		{
			m_leftTimeMap[ENTRY_ONLINE_AWARD]--;
			if (m_leftTimeMap[ENTRY_ONLINE_AWARD]>=0)
			{
				pTime->setText(TimeEx::getStringTimeForInt(m_leftTimeMap[ENTRY_ONLINE_AWARD]));
			}else
			{
				pTime->setVisible(false);
				pIsCanQueryStatue = true;
			}
			
		}
	}
	if (pMorrowBtn)
	{
		Label* pTime = dynamic_cast<Label*>(pMorrowBtn->getChildByTag(Entry_Time_Lab));
		if (pTime->isVisible())
		{
			m_leftTimeMap[ENTRY_MORROW_AWARD]--;
			if (m_leftTimeMap[ENTRY_MORROW_AWARD]>=0)
			{
				pTime->setText(TimeEx::getStringTimeForInt(m_leftTimeMap[ENTRY_MORROW_AWARD]));
			}else
			{
				pTime->setVisible(false);
				pIsCanQueryStatue = true;
			}
		}
	}
	if (pIsCanQueryStatue)
	{
		unschedule(schedule_selector(TopEntryLayout::scheduelOnlineTime));
		P_ACT_CTRL->sendQueryAllPkgStatusRqst();
	}
}