#include "GuildPanel.h"

#include "defines.h"
#include "MemberListItem.h"
#include "GuildDailyActCell.h"
#include "GuildActCell.h"
#include "GuildControl.h"
#include "UserObj.h"
#include "TextHelper.h"
#include "game/login/LoginControl.h"
#include "TimeEx.h"
#include "GlobalTime.h"
#include "GuildMemberItem.h"
#include "game/bag/BagControl.h"
#include "basic/BasicTypes.h"

GuildPanel::GuildPanel()
:m_pBackBtn(NULL)
,m_pGuildNameTxt(NULL)
,m_pGuildLevelImg(NULL)
,m_pGuildActivenessValTxt(NULL)
,m_pGuildNoticeInputTxt(NULL)
,m_pMemberListView(NULL)
,m_pDailyActBtn(NULL)
,m_pGuildAdminBtn(NULL)
,m_pImpeachTipTxt(NULL)
{
}

GuildPanel::~GuildPanel()
{
	
}

GuildPanel* GuildPanel::create() {
	GuildPanel *pRet = new GuildPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);
		setClickOutToClose(true);

		bRet = true;
	} while(0);

	return bRet;
}

void GuildPanel::open() {
	BasePanel::open();

	showLoading();

	P_GUILD_CTRL->sendGuildQueryRqst();

	if(!m_bUIInited) {
		initUI();
	}
}

void GuildPanel::close() {
    BasePanel::close();

    /*m_iImpeachTime = 0;
    this->unschedule(schedule_selector(GuildPanel::onUpdateImpeachTime));

    if(m_pImpeachTipTxt) {
        m_pImpeachTipTxt->setText("");
    }*/
	if(P_USER_OBJ->getUserInfo()->guildID > 0) {
		std::string notice = m_pGuildNoticeInputTxt->getStringValue();
		unsigned int uMyPrivilege = P_GUILD_CTRL->getMemberPrivilege(P_USER_OBJ->getUserInfo()->uin);

		if(uMyPrivilege == vmsg::GUILD_PRIVI_CHAIRMAN) {
			P_GUILD_CTRL->sendGuildChgNoticeRqst(notice);
		}
	}
}

void GuildPanel::updateGuildInfo() {
	if(!m_bUIInited) {
		m_bUIDelayed = true;
		return;
	}

	const vmsg::CSGuildInfoRsp &stGuildInfo = P_GUILD_CTRL->getGuildInfo();

	if(stGuildInfo.has_guildbrief()) {
		const vmsg::CSGuildBrief &stGuildBrief = stGuildInfo.guildbrief();
		std::string url = std::string("guild/guild_level_") + intToString(stGuildBrief.level() / 20) + ".png";

		m_pGuildNameTxt->setText(stGuildBrief.guildname());
		m_pGuildLevelImg->loadTexture(url.c_str(), UI_TEX_TYPE_PLIST);
		m_pGuildActivenessValTxt->setText(intToString(stGuildBrief.exp()));

		m_pGuildNoticeInputTxt->setText(stGuildInfo.notice());

		m_pMemberListView->removeAllItems();

		for(int i = 0; i < stGuildInfo.guildmemberlst_size(); i++) {
			GuildMemberItem *pItem = GuildMemberItem::create();
			//pItem->enableCheckBox(false);
			pItem->updateUI(stGuildInfo.guildmemberlst(i));
			m_pMemberListView->pushBackCustomItem(pItem);
		}

		bool isChairMan = P_USER_OBJ->getUserInfo()->nick == stGuildBrief.chairmanname();

		m_pGuildNoticeInputTxt->setTouchEnabled(isChairMan);
		//m_pGuildAdminBtn->setEnabled(isChairMan); // 成员可以退出部落
	}

    /*this->unschedule(schedule_selector(GuildPanel::onUpdateImpeachTime));
    if(stGuildInfo.impeachuin() > 0 && stGuildInfo.impeachtime() > 0) {
        m_iImpeachTime = stGuildInfo.impeachtime() + MAX_GUILD_IMPEACH_TIME - GlobalTime::getTime();
        if(m_iImpeachTime > 0) {
            string strTip = TextStr(TEXT_GUILD_IMPEACH_TIME) + TimeEx::parseSecondToDate(m_iImpeachTime, "Dhms");
            m_pImpeachTipTxt->setText(strTip);
            this->schedule(schedule_selector(GuildPanel::onUpdateImpeachTime),1);
        } else {
            CCLOG("[error]GuildPanel::updateGuildInfo, invalid impeach time %d,now time %d", 
                stGuildInfo.impeachtime(), GlobalTime::getTime());
        }
        
    } else {
        m_iImpeachTime = 0;
        m_pImpeachTipTxt->setText("");
    }*/

	closeLoading();
}

void GuildPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/GuildPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pBackBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("back_btn"));
	m_pBackBtn->addTouchEventListener(this, toucheventselector(GuildPanel::touchEventHandler));

	m_pGuildNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("guild_name_txt"));

	m_pGuildLevelImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("guild_level_img"));

	m_pGuildActivenessValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("guild_activeness_val_txt"));

	m_pGuildNoticeInputTxt = dynamic_cast<UITextField*>(m_pRootLayout->getChildByName("guild_notice_input_txt"));

	m_pMemberListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("member_list"));
	m_pMemberListView->addEventListenerListView(this, listvieweventselector(GuildPanel::listViewEventHandler));

	m_pDailyActBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("daily_act_btn"));
	m_pDailyActBtn->addTouchEventListener(this, toucheventselector(GuildPanel::touchEventHandler));

	m_pGuildAdminBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("guild_admin_btn"));
	m_pGuildAdminBtn->addTouchEventListener(this, toucheventselector(GuildPanel::touchEventHandler));

	m_bUIInited = true;

	if(m_bUIDelayed) {
		m_bUIDelayed = false;
		updateGuildInfo();
	}
}

void GuildPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pBackBtn->getName()) == 0) {
				close();
			} else if(strcmp(pszWidgetName, m_pDailyActBtn->getName()) == 0) {
				
			} else if(strcmp(pszWidgetName, m_pGuildAdminBtn->getName()) == 0) {
				P_GUILD_CTRL->getGuildAdminPanel()->open();
				close();
			}
			break;
		default:;
	}
}

void GuildPanel::listViewEventHandler(CCObject *pSender, ListViewEventType type) {
	UIListView *pListView = dynamic_cast<UIListView*>(pSender);
	const char *pszListViewName = pListView->getName();

	switch(type) {
		case LISTVIEW_ONSELECTEDITEM_END:
			if(std::strcmp(pszListViewName, m_pMemberListView->getName()) == 0) {
				int nSelectedIdx = m_pMemberListView->getCurSelectedIndex();
				GuildMemberItem *pItem = dynamic_cast<GuildMemberItem*>(m_pMemberListView->getItem(nSelectedIdx));
				UIN_t uin = pItem->getMemberUIN();

				if(uin != P_USER_OBJ->getUserInfo()->uin) {
					//close();
					//P_BAG_CTRL->requestOtherRoleInfo(uin);
				}
			}
			break;
		default:;
	}
}

void GuildPanel::textFieldEventHandler(CCObject *pSender, TextFiledEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	
	switch(type) {
		case TEXTFIELD_EVENT_DETACH_WITH_IME: // 输入结束
			if(std::strcmp(pszWidgetName, m_pGuildNoticeInputTxt->getName()) == 0) {
				std::string notice = m_pGuildNoticeInputTxt->getStringValue();
				P_GUILD_CTRL->sendGuildChgNoticeRqst(notice);
			}
			break;
		default:;
	}
}

/*
void GuildPanel::onUpdateImpeachTime(float dt) {
	m_iImpeachTime--;
	if(m_iImpeachTime <= 0) {
		this->unschedule(schedule_selector(GuildPanel::onUpdateImpeachTime));
		m_pImpeachTipTxt->setText("");
		P_GUILD_CTRL->sendGuildQueryRqst();
		return;
	}

	string strTip = TextStr(TEXT_GUILD_IMPEACH_TIME) + TimeEx::parseSecondToDate(m_iImpeachTime, "Dhms");
	m_pImpeachTipTxt->setText(strTip);
}*/
