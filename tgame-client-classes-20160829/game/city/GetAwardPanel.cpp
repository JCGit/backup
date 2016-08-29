#include "GetAwardPanel.h"

#include "defines.h"

#include "vlib/base/CVTime.h"

#include "basic/ImageNumber.h"
#include "basic/TextHelper.h"

#include "CfgMaster/GlobalCfgMaster.h"
#include "CfgMaster/VIPCfgMaster.h"
#include "CfgMaster/ItemCfgMaster.h"

#include "game/Global.h"
#include "game/UserObj.h"
#include "game/player/PlayerControl.h"
#include "game/notification/NotificationControl.h"

#include "CityControl.h"
#include "AutoPopControl.h"

#define SWEEP_CARD_ID 1

GetAwardPanel::GetAwardPanel()
:m_pCloseBtn(NULL)
,m_pAwardDescTxt(NULL)
,m_pAwardIconImg(NULL)
,m_pAwardCntImgTxt(NULL)
,m_pGetBtn(NULL)
{
}

GetAwardPanel::~GetAwardPanel()
{
}

GetAwardPanel* GetAwardPanel::create() {
	GetAwardPanel *pRet = new GetAwardPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GetAwardPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void GetAwardPanel::open() {
	BasePanel::open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	getAwardCnt();

	popAward();

	closeLoading();
}

void GetAwardPanel::close() {
	BasePanel::close();

	if(!m_bIsGetAwardBeforeClose) {
		P_CITY_CTRL->getCityScreenView()->updateEntry();

		if(m_stAwardInfos.size() > 0) {
			PUSH_NOTIFICATION(ENTRY_GET_AWARD, NOTIFY_TAG_GET_AWARD, 1);
		} else {
			PUSH_NOTIFICATION(ENTRY_GET_AWARD, NOTIFY_TAG_GET_AWARD, 0);
		}
	}

	P_AUTO_POP_CTRL->onClosePanel(this);
}
bool GetAwardPanel::isMeetAutoOpen() {
	bool bRet = false;

	do {
		CC_BREAK_IF(getAwardCnt() == 0);

		bRet = true;
	} while(0);

	return bRet;
}

unsigned int GetAwardPanel::getAwardCnt() {
	if(m_bIsOpen) {
		return m_stAwardInfos.size();
	}

	m_stAwardInfos.clear();

	// 月卡每日奖励
	const vmsg::CSMonthCardInfo &stMonthCardInfo = P_USER_OBJ->getUserInfo()->stMonthCardInfo;
	unsigned int uSvrTime = P_GLOBAL->getSvrTime();
	unsigned int uEndTime = stMonthCardInfo.endtime();
	unsigned int uAwardTime = stMonthCardInfo.lastawardtime();

	if(uEndTime > uSvrTime && !CVTimeUtils::IsSameDay(uAwardTime, uSvrTime)) { // 月卡没过期且今天还没领奖
		AwardInfo_t awardInfo;
		awardInfo.desc = TextStr(TEXT_MONTH_CARD_AWARD);
		awardInfo.icon = "share/icon_gold.png";
		awardInfo.plisticon = true;
		awardInfo.cnt = stMonthCardInfo.gold();
		m_stAwardInfos.push_back(awardInfo);
	}

	// VIP每日奖励
	if(!P_USER_OBJ->getUserInfo()->isVIPDailyAwardClaimed) {
		const ItemCfg *pItemCfg = ITEM_CFG_MASTER->GetItemCfgByID(SWEEP_CARD_ID);
		unsigned int uVIPLv = P_USER_OBJ->getUserInfo()->vipLv;
		const VIPCfg *pVIPCfg = VIP_CFG_MASTER->getVIPCfg(vmsg::VIP_MODULE_SWEEP_CARD);

		AwardInfo_t awardInfo;
		awardInfo.desc = TextStr(TEXT_VIP_DAILY_AWARD);
		awardInfo.icon = pItemCfg->itemicon();
		awardInfo.plisticon = false;
		awardInfo.cnt = pVIPCfg->vipparam(uVIPLv);
		m_stAwardInfos.push_back(awardInfo);
	}

	return m_stAwardInfos.size();
}

void GetAwardPanel::popAward() {
	if(m_stAwardInfos.size() > 0) {
		AwardInfo_t stAwardInfo = m_stAwardInfos.back();
		m_stAwardInfos.pop_back();

		m_pAwardDescTxt->setText(stAwardInfo.desc);

		if(stAwardInfo.plisticon) {
			m_pAwardIconImg->loadTexture(stAwardInfo.icon.c_str(), UI_TEX_TYPE_PLIST);
		} else {
			m_pAwardIconImg->loadTexture(stAwardInfo.icon.c_str());
		}

		m_pAwardCntImgTxt->setNum(intToString(stAwardInfo.cnt), 0.0f, "x");
	} else {
		close();
	}
}

void GetAwardPanel::claimed() {
	m_bIsGetAwardBeforeClose = false;
	
	if(!m_bIsOpen) { // 面板已关闭，更新入口
		P_CITY_CTRL->getCityScreenView()->updateEntry();

		if(m_stAwardInfos.size() > 0) {
			PUSH_NOTIFICATION(ENTRY_GET_AWARD, NOTIFY_TAG_GET_AWARD, 1);
		} else {
			PUSH_NOTIFICATION(ENTRY_GET_AWARD, NOTIFY_TAG_GET_AWARD, 0);
		}
	}
}

void GetAwardPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/GetAwardPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(GetAwardPanel::touchEventHandler));

	m_pAwardDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_desc_txt"));

	m_pAwardIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("award_icon_img"));

	m_pAwardCntImgTxt = ImageNumber::create(28);
	m_pAwardCntImgTxt->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pAwardCntImgTxt->setPosition(m_pAwardIconImg->getPosition() + ccp(m_pAwardIconImg->getSize().width / 2 + 8, 0));
	m_pRootLayout->addNode(m_pAwardCntImgTxt);
	m_pAwardCntImgTxt->setNum("0", 0.0f, "x");

	m_pGetBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("get_btn"));
	m_pGetBtn->addTouchEventListener(this, toucheventselector(GetAwardPanel::touchEventHandler));

	m_bUIInited = true;
}

void GetAwardPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				popAward();
			} else if(pSender == m_pGetBtn) {
				if(strcmp(m_pAwardDescTxt->getStringValue(), TextStr(TEXT_MONTH_CARD_AWARD)) == 0) {
					P_PLAYER_CTRL->sendGetMonthCardAwardRqst();
					m_bIsGetAwardBeforeClose = true;
				} else if(strcmp(m_pAwardDescTxt->getStringValue(), TextStr(TEXT_VIP_DAILY_AWARD)) == 0) {
					P_PLAYER_CTRL->sendGetVIPDailyAwardRqst();
					m_bIsGetAwardBeforeClose = true;
				}

				popAward();
			}
			break;
		default:;
	}
}
