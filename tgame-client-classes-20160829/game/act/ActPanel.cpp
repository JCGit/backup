#include "ActPanel.h"

#include "Macros.pb.h"

#include "basic/ToolUtils.h"

#include "ActControl.h"
#include "ActItem.h"
#include "ActLayout.h"
#include "ActNormalLayout.h"
#include "ActChargeLayout.h"
#include "ActNewYearCarnivalLayout.h"
#include "ActNewYearEnergyLayout.h"
#include "ActNewYearGiftBagLayout.h"
#include "ActCumulativeRechargeLayout.h"
#include "ActCumulativeLoginLayout.h"
#include "ActDailyRechargeLayout.h"
#include "ActGradeGeekLayout.h"
#include "ActFightGeekLayout.h"
#include "ActShareGiftLayout.h"
#include "ActVipGiftLayout.h"
#include "ActDailyConsumeLayout.h"
#include "ActSumConsumeLayout.h"
#include "CfgMaster/ActCfgMaster.h"

CActPanel::CActPanel()
:m_pCloseBtn(NULL)
,m_pActList(NULL)
,m_pActLayout(NULL)
,m_nActSelectedIdx(0)
,m_actSelectType(vmsg::ACT_LIST_TYPE_MAX+1)
{
	
}

CActPanel::~CActPanel(){
	map<unsigned int, ActLayout*>::iterator iter = m_stActLayoutMap.begin();

	while(iter != m_stActLayoutMap.end()) {
		CC_SAFE_RELEASE(iter->second);
	}

	m_stActLayoutMap.clear();
}

bool CActPanel::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/act_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CActPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CActPanel::touchEventHandler));

	m_pActList = dynamic_cast<ListView*>(m_pRootLayout->getChildByName("act_list"));
	m_pActList->setItemsMargin(20);
	m_pActList->addEventListenerListView(this, listvieweventselector(CActPanel::listViewEventHandler));

	m_pActLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("act_layout"));

	m_bUIInited = true;
}

void CActPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	switch(type) {
	case LISTVIEW_ONSELECTEDITEM_END:
		int nSelectedIdx = m_pActList->getCurSelectedIndex();
		setTypeIdx(nSelectedIdx);
		break;
	}
}

ActLayout* CActPanel::getActLayout(unsigned int uActType) {
	ActLayout *pActLayout = NULL;

	map<unsigned int, ActLayout*>::iterator iter = m_stActLayoutMap.find(uActType);

	if(iter != m_stActLayoutMap.end()) {
		pActLayout = iter->second;
	}

	return pActLayout;
}

ActLayout* CActPanel::createActLayout(unsigned int uActType) {
	ActLayout *pActLayout = NULL;


	switch(uActType) {
		case vmsg::PLATFORM_PKG_ACT:
		case vmsg::NEWBIE_PKG_ACT:
			pActLayout = CActNormalLayout::create();
			break;
		case vmsg::FIRST_BUY_PKG_ACT:
			pActLayout = CActChargeLayout::create();
			break;
		case vmsg::NEW_YEAR_ONESHOT_ACT:
			pActLayout = ActNewYearGiftBagLayout::create();
			break;
		case vmsg::NEW_YEAR_7DAY_ACT:
			pActLayout = ActNewYearCarnivalLayout::create();
			break;
		case vmsg::STORE_MANUAL_ACT:
			pActLayout = ActNewYearEnergyLayout::create();
			break;
		case vmsg::SUM_CHARGE_ACT:
			pActLayout = P_ACT_CTRL->getSumRechargePanel();
			break;
		case vmsg::DAILY_LOGIN_ACT:
			pActLayout = ActCumulativeLoginLayout::create();
			break;
		case vmsg::DAILY_CHARGE_ACT:
			pActLayout = ActDailyechargeLayout::create();
			break;
		case vmsg::GRADE_GEEK_ACT:
			pActLayout = GradeGeekLayout::create();
			break;
		case vmsg::FIGHT_GEEK_ACT:
			pActLayout = ActFightGeekLayout::create();
			break;
		case vmsg::SHARE_GIFT_ACT:
			pActLayout = ActShareGiftLayout::create();
			break;
		case vmsg::VIP_GIFT_ACT:
			pActLayout = ActVipGiftLayout::create();
			break;
		case vmsg::DAILY_CONSUME_ACT:
			pActLayout = ActDailyConsumeLayout::create();
			break;
		case vmsg::SUM_CONSUME_ACT:
			pActLayout = ActSumConsumeLayout::create();
			break;
		case vmsg::FIRST_CHARGEDOUBLE_ACT:
			pActLayout = ActShareGiftLayout::create();
			break;
		default:;
	}

	return pActLayout;
}

void CActPanel::setActSelectType(unsigned int type)
{
	m_actSelectType = type;
}

void CActPanel::setTypeIdx(unsigned int idx){
	if(idx < 0 || idx >= m_pActList->getItems()->count()) {
		return;
	}

	ActItem *pActItem = dynamic_cast<ActItem*>(m_pActList->getItem(m_nActSelectedIdx));

	if(pActItem != NULL) {
		pActItem->selected(false);
	}
	if (m_actSelectType <= vmsg::ACT_LIST_TYPE_MAX)
	{
		CCObject* pObj =NULL;
		CCARRAY_FOREACH ( m_pActList->getItems(), pObj)
		{
			ActItem* pItem = dynamic_cast<ActItem*>(pObj);
			if (pItem)
			{
				if (pItem->getInfo().type() == m_actSelectType)
				{
					pItem->selected(true);
					m_nActSelectedIdx = m_pActList->getIndex(pItem);
					m_pActList->getInnerContainer()->setPositionY((m_pActList->getItemsMargin()+pItem->getContentSize().height)*m_nActSelectedIdx);
					break;
				}
				idx++;
			}
		}
		m_actSelectType = vmsg::ACT_LIST_TYPE_MAX +1;
	}else
	{
		pActItem = dynamic_cast<ActItem*>(m_pActList->getItem(idx));
		pActItem->selected(true);
		m_nActSelectedIdx = idx;
	}
	
	
	//update
	updateContent();
}

unsigned int CActPanel::getTypeIdx(){
	return m_nActSelectedIdx;
}

void CActPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
			close();
		}
		break;
	default:;
	}
}

void CActPanel::updateUI(){
	if(!isOpen()) {
		return;
	}

	if(this->getIsInLoading()){
		this->closeLoading();
	}

	m_pActList->removeAllItems();

	//const vmsg::CSQueryAllPkgStatusRsp &stStatusInfo = P_ACT_CTRL->getStatusInfo();
	/*vmsg::CSQueryAllPkgStatusRsp stStatusInfo = P_ACT_CTRL->getStatusInfo();
	vmsg::CSCouponActStatusItem *pStatusItem = stStatusInfo.add_allacts();
	pStatusItem->set_type(3);
	pStatusItem->set_status(1);
	pStatusItem = stStatusInfo.add_allacts();
	pStatusItem->set_type(4);
	pStatusItem->set_status(1);
	pStatusItem = stStatusInfo.add_allacts();
	pStatusItem->set_type(5);
	pStatusItem->set_status(1);*/

	vmsg::CSQueryAllPkgStatusRsp stStatusInfo = P_ACT_CTRL->getStatusInfo();
	vmsg::CSCouponActStatusItem* pStatusItem = stStatusInfo.add_allacts();//客户端永久活动
	pStatusItem->set_type(vmsg::FIRST_CHARGEDOUBLE_ACT);
	pStatusItem->set_status(1);
	for (auto pActId = ACT_CFG_MASTER->m_stID2IDX.begin(); pActId!= ACT_CFG_MASTER->m_stID2IDX.end(); pActId++)
	{
		for(int i = 0; i < stStatusInfo.allacts_size(); i++) {
			const vmsg::CSCouponActStatusItem &stItemInfo = stStatusInfo.allacts(i);
			if (stItemInfo.type() == ONLINE_PKG_AWARD || stItemInfo.type() == NEXTDAY_PKG_AWARD)
			{
				continue;
			}
			if (pActId->second == stItemInfo.type())
			{
				ActItem *pActItem = ActItem::create();
				m_pActList->pushBackCustomItem(pActItem);
				pActItem->setInfo(stItemInfo);
				break;
			}
			
		}
	}
	
	
	
	setTypeIdx(m_nActSelectedIdx > 0 && m_nActSelectedIdx < stStatusInfo.allacts_size() ? m_nActSelectedIdx : 0);
}

void CActPanel::updateCurType(){
	ActItem *pActItem = dynamic_cast<ActItem*>(m_pActList->getItem(m_nActSelectedIdx));

	if(pActItem != NULL) {
		CSCouponActStatusItem stItemInfo = pActItem->getInfo();
		stItemInfo.set_status(P_ACT_CTRL->getItemInfoByType(stItemInfo.type()).status());
		pActItem->setInfo(stItemInfo);
		
		updateContent();
	}
}

void CActPanel::updateContent(){
	ActItem *pActItem = dynamic_cast<ActItem*>(m_pActList->getItem(m_nActSelectedIdx));

	if(pActItem != NULL) {
		//礼包类型
		const CSCouponActStatusItem& stItemInfo = pActItem->getInfo();

		m_pActLayout->removeAllChildren();
		ActLayout *pActLayout = getActLayout(stItemInfo.type());

		if(pActLayout == NULL) {
			pActLayout = createActLayout(stItemInfo.type());
			m_stActLayoutMap.insert(make_pair(stItemInfo.type(), pActLayout));
			CC_SAFE_RETAIN(pActLayout);
		}

		m_pActLayout->addChild(pActLayout);
		pActLayout->updateInfo(stItemInfo);
	}
}

void CActPanel::refresh(){
	BasePanel::refresh();

	P_ACT_CTRL->sendQueryAllPkgStatusRqst();
	this->showLoading();
}

void CActPanel::open(){
	BasePanel::open();
	CToolUtils::loadArmatureRGBA4444("effect/ut00090/ut00090.ExportJson");;
	P_ACT_CTRL->sendQueryAllPkgStatusRqst();
	this->showLoading();

}

void CActPanel::close(){
	BasePanel::close();
	CToolUtils::removeArmatureFile("effect/ut00090/ut00090.ExportJson");
	m_nActSelectedIdx = -1;
}
