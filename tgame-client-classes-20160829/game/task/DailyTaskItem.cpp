#include "DailyTaskItem.h"

#include "vlib/base/CVTime.h"

#include "CfgMaster/DailyTaskCfgMaster.h"
#include "CfgMaster/DailyTaskAwardCfgMaster.h"
#include "CfgMaster/MilestoneCfgMaster.h"
#include "CfgMaster/GlobalCfgMaster.h"

#include "ClientSvrMsg.pb.h"

#include "basic/CCColorUtil.h"
#include "basic/NounHelper.h"
#include "basic/TextHelper.h"
#include "basic/TimeEx.h"
#include "basic/CommonDialog.h"

#include "game/Global.h"
#include "game/UserObj.h"
#include "game/city/CityControl.h"
#include "game/nihility/NihilityControl.h"
#include "game/equiplib/EquipLibControl.h"
#include "game/weaponlib/WeaponLibControl.h"
#include "game/friend/FriendControl.h"
#include "game/guild/GuildControl.h"
#include "game/attrstrengthen/AttrStrengthenControl.h"
#include "game/arena/ArenaControl.h"
#include "game/city/EntryControl.h"
#include "game/player/PlayerControl.h"
#include "game/warmap/WarMapControl.h"
#include "control/AudioManager.h"
#include "game/tip/TipControl.h"
#include "game/manual/ManualControl.h"

#include "Resources.h"
#include "defines.h"
#include "TaskControl.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"
#include "../act/ActControl.h"

DailyTaskItem::DailyTaskItem()
:m_pIconImg(NULL)
,m_pTaskNameTxt(NULL)
,m_pTaskDescTxt(NULL)
,m_pTaskIntroTxt(NULL)
//,m_pAwardTypeImg(NULL)
,m_pIconAwardImg(NULL)
,m_pAwardValTxt(NULL)
,m_pProgressValTxt(NULL)
,m_pGotoBtn(NULL)
,m_pStampGetImg(NULL)
,m_uTaskID(0)
,m_uTaskStat(0)
,m_bIsMonthCard(false)
{
}

DailyTaskItem::~DailyTaskItem()
{
}

DailyTaskItem* DailyTaskItem::create() {
	DailyTaskItem *pRet = new DailyTaskItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool DailyTaskItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/DailyTaskItemUI.ExportJson");
		addChild(m_pRootLayout);

		setName("daily_task_item");
		addTouchEventListener(this, toucheventselector(DailyTaskItem::touchEventHandler));

		m_pIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_img"));

		m_pTaskNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("task_name_txt"));

		m_pTaskDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("task_desc_txt"));

		m_pTaskIntroTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("task_intro_txt"));

		//m_pAwardTypeImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("award_type_img"));

		m_pIconAwardImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_award_img"));

		m_pAwardValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_val_txt"));

		m_pProgressValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("progress_val_txt"));

		m_pGotoBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("goto_btn"));
		m_pGotoBtn->addTouchEventListener(this, toucheventselector(DailyTaskItem::touchEventHandler));

		m_pStampGetImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("stamp_get_img"));

		bRet = true;
	} while(0);

	return bRet;
}

void DailyTaskItem::updateUI(const vmsg::CSTask& stTask) {
	m_uTaskID = stTask.taskid();
	m_uTaskStat = stTask.taskstat();

	DailyTaskCfg *pDailyTaskCfg = DAILY_TASK_CFG_MASTER->getDailyTaskCfgByID(stTask.taskid());

	std::string url = ICON_PATH + pDailyTaskCfg->dailytaskicon() + ".png";
	m_pIconImg->loadTexture(url.c_str());
	m_pTaskNameTxt->setText(pDailyTaskCfg->dailytaskname());
	m_pTaskDescTxt->setText(pDailyTaskCfg->dailytaskdesc());
	m_pTaskIntroTxt->setText(pDailyTaskCfg->dailytaskintro());

	unsigned int uLevel = P_USER_OBJ->getUserInfo()->level;
	DailyTaskAwardCfg *pDailyTaskAwardCfg = DAILY_TASK_AWARD_CFG_MASTER->getDailyTaskAwardCfg(uLevel, pDailyTaskCfg->mainawardtype());

	if(pDailyTaskCfg->mainawardtype() == vmsg::THING_TYPE_COIN) {
		m_pIconAwardImg->loadTexture("share/icon_coin.png", UI_TEX_TYPE_PLIST);
		m_pAwardValTxt->setText(intToString(pDailyTaskAwardCfg->award().coin()));
	} else if(pDailyTaskCfg->mainawardtype() == vmsg::THING_TYPE_GOLD) {
		m_pIconAwardImg->loadTexture("share/icon_gold.png", UI_TEX_TYPE_PLIST);
		m_pAwardValTxt->setText(intToString(pDailyTaskAwardCfg->award().gold()));
	} else if(pDailyTaskCfg->mainawardtype() == vmsg::THING_TYPE_EXP) {
		m_pIconAwardImg->loadTexture("share/icon_exp.png", UI_TEX_TYPE_PLIST);
		m_pAwardValTxt->setText(intToString(pDailyTaskAwardCfg->award().exp()));
	}else if (pDailyTaskCfg->mainawardtype() == vmsg::THING_TYPE_MANUAL)
	{
		m_pIconAwardImg->loadTexture("share/icon_mannual.png", UI_TEX_TYPE_PLIST);
		m_pAwardValTxt->setText(intToString(pDailyTaskCfg->award().awardcnt()));
	}
	
	bool isTaskFinished = false;
	bool isTaskOver = false;

	if(pDailyTaskCfg->target().targettype() == vmsg::DAILY_TYPE_ATKDEF_CNT) {
		if(stTask.r1() > pDailyTaskCfg->target().r2()) {
			m_pProgressValTxt->setText(intToString(pDailyTaskCfg->target().r2()) + "/" + intToString(pDailyTaskCfg->target().r2()));
		} else {
			m_pProgressValTxt->setText(intToString(stTask.r1()) + "/" + intToString(pDailyTaskCfg->target().r2()));
		}
		
		isTaskFinished = stTask.r1() >= pDailyTaskCfg->target().r2() && stTask.taskstat() == vmsg::TASK_STAT_FINISH;
		isTaskOver = stTask.r1() >= pDailyTaskCfg->target().r2() && stTask.taskstat() == vmsg::TASK_STAT_OVER;
	} else {
		if(stTask.r1() > pDailyTaskCfg->target().r1()) {
			m_pProgressValTxt->setText(intToString(pDailyTaskCfg->target().r1()) + "/" + intToString(pDailyTaskCfg->target().r1()));
		} else {
			m_pProgressValTxt->setText(intToString(stTask.r1()) + "/" + intToString(pDailyTaskCfg->target().r1()));
		}
		
		isTaskFinished = stTask.r1() >= pDailyTaskCfg->target().r1() && stTask.taskstat() == vmsg::TASK_STAT_FINISH;
		isTaskOver = stTask.r1() >= pDailyTaskCfg->target().r1() && stTask.taskstat() == vmsg::TASK_STAT_OVER;
	}

	/*m_pGotoBtn->setTouchEnabled(false);
	m_pGotoBtn->setVisible(false);*/
	m_pGotoBtn->setTouchEnabled(!isTaskFinished);
	m_pGotoBtn->setVisible(!isTaskFinished);

	m_pStampGetImg->setVisible(isTaskFinished);
}

void DailyTaskItem::updateUI() {
	m_bIsMonthCard = true;

	const vmsg::CSMonthCardInfo &stMonthCardInfo = P_USER_OBJ->getUserInfo()->stMonthCardInfo;

	std::string url = ICON_PATH + "month_card" + ".png";
	m_pIconImg->loadTexture(url.c_str());
	m_pTaskNameTxt->setText(TextStr(TEXT_MONTH_CARD));
	m_pTaskDescTxt->setText(TextStr(TEXT_BUY_MONTH_CARD));
	char text[100];
	snprintf(text, 100, TextStr(TEXT_MONTH_CARD_AWARD_DESC), stMonthCardInfo.gold());
	m_pTaskIntroTxt->setText(text);

	// 月卡未过期且上次领奖不在今天
	unsigned int uSvrTime = P_GLOBAL->getSvrTime();
	unsigned int uEndTime = stMonthCardInfo.endtime();
	unsigned int uAwardTime = stMonthCardInfo.lastawardtime();
	bool hasMonthCardAward = uEndTime > uSvrTime && !CVTimeUtils::IsSameDay(uAwardTime, uSvrTime);

	m_pIconAwardImg->loadTexture("share/icon_gold.png", UI_TEX_TYPE_PLIST);
	m_pAwardValTxt->setText(intToString(stMonthCardInfo.gold()));

	m_pProgressValTxt->setText(intToString(hasMonthCardAward ? 1 : 0) + "/" + intToString(1));

	m_pGotoBtn->setTouchEnabled(!hasMonthCardAward);
	m_pGotoBtn->setVisible(!hasMonthCardAward);

	m_pStampGetImg->setVisible(hasMonthCardAward);
	setTouchEnabled(hasMonthCardAward);
}

bool DailyTaskItem::isTaskFinished() {
	std::string text = m_pProgressValTxt->getStringValue();

	std::string::size_type pos = text.find("/");

	return std::atoi(text.substr(0, pos).c_str()) >= std::atoi(text.substr(pos + 1, text.size() - pos - 1).c_str());
}

unsigned int DailyTaskItem::getTaskID() {
	return m_uTaskID;
}

unsigned int DailyTaskItem::getTaskStat() {
	return m_uTaskStat;
}

Widget* DailyTaskItem::getGoToButton()
{
	Widget* pWidget = NULL;
	if (m_pGotoBtn->isVisible())
	{
		pWidget = m_pGotoBtn;
	}else
	{
		pWidget = m_pStampGetImg;
	}
	return pWidget;
}

void DailyTaskItem::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, getName()) == 0) {
				if(m_bIsMonthCard) {
					string url = SOUND_RES_PATH + "ut00028.ogg";
					AudioManager::preloadEffect(url.c_str());
					AudioManager::playEffect("ut00028.ogg");
					//P_PLAYER_CTRL->sendGetMonthCardAward();
				} else if(m_uTaskStat == vmsg::TASK_STAT_FINISH) {
					GUIDE_TOUCH(getGoToButton());
					string url = SOUND_RES_PATH + "ut00028.ogg";
					AudioManager::preloadEffect(url.c_str());
					AudioManager::playEffect("ut00028.ogg");
					P_TASK_CTRL->sendCommitDailyTaskRqst(m_uTaskID);
				}
			} else if(strcmp(pszWidgetName, m_pGotoBtn->getName()) == 0) {
				if(m_bIsMonthCard) { // 月卡
					P_TASK_CTRL->getTaskPanel()->close();
                    P_SHOP_CTRL->getRmbShopPanel()->open();
					break;
				}
				GUIDE_TOUCH(getGoToButton());
				DailyTaskCfg *pDailyTaskCfg = DAILY_TASK_CFG_MASTER->getDailyTaskCfgByID(m_uTaskID);

				if(pDailyTaskCfg != NULL) {
					P_TASK_CTRL->getTaskPanel()->close();

					switch(pDailyTaskCfg->systype()) {
						case vmsg::DAILY_SYS_WARMAP:
							P_CITY_CTRL->moveToBattlePortal();
							break;
						case vmsg::DAILY_SYS_INFINIT:
							P_ENTRY_CTRL->enter(ENTRY_NOTHING_LAND);
							break;
						case vmsg::DAILY_SYS_EQUIP:
							P_ENTRY_CTRL->enter(ENTRY_EQUIP_CULTURE);
							break;
						case vmsg::DAILY_SYS_WEAPON:
							P_ENTRY_CTRL->enter(ENTRY_WEAPON_LIB);
							break;
						case vmsg::DAILY_SYS_FRIEND:
							P_ENTRY_CTRL->enter(ENTRY_FRIEND);
							break;
						case vmsg::DAILY_SYS_GOLD:
							buyManual();
							break;
						case vmsg::DAILY_SYS_GUILD:
							P_ENTRY_CTRL->enter(ENTRY_GUILD);
							break;
						case vmsg::DAILY_SYS_ATKDEF:
							P_ENTRY_CTRL->enter(ENTRY_ATTR_STRENGTHEN);
							break;
						case vmsg::DAILY_SYS_EXPLORE:
							P_ENTRY_CTRL->enter(ENTRY_LOTTERY);
							break;
						case vmsg::DAILY_SYS_ARENA:
							P_ENTRY_CTRL->enter(ENTRY_ARENA);
							break;
						case vmsg::DAILY_SYS_ELITE:
							P_WAR_MAP_CTRL->gotoEliteWar();
							break;
						case vmsg::DAILY_SYS_SHARE:
							P_ACT_CTRL->getActPanel()->setActSelectType(vmsg::SHARE_GIFT_ACT);
							P_ENTRY_CTRL->enter(ENTRY_ACTIVITIES);
							break;
						default:;
					}
				}
			}
			break;
		default:;
	}
}

void DailyTaskItem::buyManual() {
	P_TIP_CTRL->LowManualBuyTip(this, dialogbtneventselector(DailyTaskItem::onBuyManualConfirm));
}

void DailyTaskItem::onBuyManualConfirm() {
	P_MANUAL_CTRL->sendBuyManualRqst();
}
