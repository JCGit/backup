#include "EntryControl.h"

#include "QEnvSet.h"

#include "CfgMaster/EntryCfgMaster.h"

#include "ModuleID.pb.h"

#include "game/bag/BagControl.h"
#include "game/equiplib/EquipLibControl.h"
#include "game/weaponlib/WeaponLibControl.h"
#include "game/rank/RankControl.h"
#include "game/task/TaskControl.h"
#include "game/shop/ShopControl.h"
#include "game/qianghun/QianghunControl.h"
#include "game/chat/ChatControl.h"
#include "game/mail/MailControl.h"
#include "game/evolution/EvolutionControl.h"
#include "game/attrstrengthen/AttrStrengthenControl.h"
#include "game/friend/FriendControl.h"
#include "game/guild/GuildControl.h"
#include "game/arena/ArenaControl.h"
#include "game/nihility/NihilityControl.h"
#include "game/frag/FragControl.h"
#include "game/dailyact/DailyActControl.h"
#include "game/signin/SignInControl.h"
#include "game/vip/VIPControl.h"
#include "game/vip/BuyCoinPanel.h"
#include "game/notification/NotificationControl.h"
#include "set/SetSysControl.h"
#include "game/refit/RefitControl.h"
#include "CityControl.h"
#include "shop/RmbShopPanel.h"
#include "game/warmap/WarMapControl.h"
#include "act/ActControl.h"
#include "game/dailyinstance/DailyInstanceControl.h"
#include "game/lottery/LotteryControl.h"
#include "game/gen/GenStrengthenControl.h"

EntryControl::EntryControl()
{
}

EntryControl::~EntryControl()
{
}

bool EntryControl::isEntryOpen(unsigned int uEntryID) {
	bool bRet = false;

#ifdef TGAME_DEV_TEST
	if(TGAME_DEV_TEST == 1) {
		return true; //开发环境全打开，在QEnvSet.h里加 #define TGAME_DEV_TEST 1
	}
#endif

	const EntryCfg *pEntryCfg = ENTRY_CFG_MASTER->getEntryCfg(uEntryID);

	if(pEntryCfg != NULL) {
		bool hasChildrenOpen = false;
		bool hasModuleOpen = false;

		for(int i = 0; i < pEntryCfg->childentryid_size(); i++) {
			unsigned int uChildEntryID = pEntryCfg->childentryid(i);
			
			if(isEntryOpen(uChildEntryID)) {
				hasChildrenOpen = true;
				break;
			}
		}

		for(int i = 0; i < pEntryCfg->moduleid_size(); i++) {
			unsigned int uModuleID = pEntryCfg->moduleid(i);

			if(P_CITY_CTRL->isModuleOpen(uModuleID)) {
				hasModuleOpen = true;
				break;
			}
		}

		(pEntryCfg->childentryid_size() > 0 && hasChildrenOpen) || (pEntryCfg->moduleid_size() > 0 && hasModuleOpen);
		
		bRet = (pEntryCfg->childentryid_size() == 0 && pEntryCfg->moduleid_size() == 0)
			|| (pEntryCfg->childentryid_size() > 0 && hasChildrenOpen)
			|| (pEntryCfg->moduleid_size() > 0 && hasModuleOpen);

		if(uEntryID == ENTRY_ELITE_WAR) { // 精英副本
			bRet = P_WAR_MAP_CTRL->isEliteWarOpen();
		} else if(uEntryID == ENTRY_SECRET_CHAT) { // 私聊
			bRet = P_CHAT_CTRL->isNotifySecretChat();
		} else if(uEntryID == ENTRY_GET_AWARD) {
			bRet = P_CITY_CTRL->getGetAwardPanel()->getAwardCnt() > 0;
		} else if(uEntryID == ENTRY_BOSS_WAR) {
			bRet = isEntryOpen(ENTRY_DAILY_ACT); // boss战在每日活动里
		} else if(uEntryID == ENTRY_FIRST_CHARGE_PKG) {
			bRet = P_SHOP_CTRL->getFirstChargePanel()->isMeetAutoOpen();
		}

		bRet = pEntryCfg->locked() == 0 && bRet;
	}

	/*switch(uEntryID) {
		case ENTRY_ROLE: // 角色
			bRet = true;
			break;
		case ENTRY_EQUIP_CULTURE: // 装备养成
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_EQUIPLVUP);
			break;
		case ENTRY_WEAPON_LIB: // 枪械库
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_WEAPONLIB);;
			break;
		case ENTRY_GROWTH: // 成长
			bRet = isEntryOpen(ENTRY_EVOLUTION)
				|| isEntryOpen(ENTRY_ATTR_STRENGTHEN);
			break;
		case ENTRY_SOCIAL: // 社交
			bRet = isEntryOpen(ENTRY_FRIEND)
				|| isEntryOpen(ENTRY_GUILD);
			break;
		case ENTRY_CHALLENGE: // 挑战
			bRet = isEntryOpen(ENTRY_ARENA)
				|| isEntryOpen(ENTRY_NOTHING_LAND)
				|| isEntryOpen(ENTRY_ELITE_WAR);
			break;
		case ENTRY_BUSINESS: // 交易
			bRet = isEntryOpen(ENTRY_BLACK_MARKET)
				|| isEntryOpen(ENTRY_FRAGMENT);
			break;
		case ENTRY_RANK: // 排行榜
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_RANK);
			break;
		case ENTRY_SHOP: // 商城
			bRet = true;
			break;
		case ENTRY_LOTTERY: // 夺宝奇兵
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_LOTTERY);
			break;
		case ENTRY_WEAPON_BOOK: // 枪魂
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_QIANGHUN);
			break;
		case ENTRY_CHAT: // 聊天
			bRet = true;
			break;
		case ENTRY_MAIL: // 邮件
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_MAIL);
			break;
		case ENTRY_DAILY_TASK: // 日常任务
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_DAILYTASK);
			break;
		case ENTRY_EVOLUTION: // 进化
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_EVOLUTION);
			break;
		case ENTRY_ATTR_STRENGTHEN: // 属性强化
			bRet = false;
			break;
		case ENTRY_FRIEND: // 好友
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_FRIEND);
			break;
		case ENTRY_GUILD: // 部落
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_GUILD);
			break;
		case ENTRY_ARENA: // 竞技场
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_ARENA);
			break;
		case ENTRY_NOTHING_LAND: // 虚无之地
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_NOTHINGLAND);
			break;
		case ENTRY_BLACK_MARKET: // 黑市
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_BLACKMARKET);
			break;
		case ENTRY_FRAGMENT: // 碎片
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_DEBRIS);
			break;
		case ENTRY_SIGN_IN: // 签到
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_SIGNIN);;
			break;
		case ENTRY_BUY_COIN: // 兑换所
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_BUYCOIN);
			break;
		case ENTRY_DAILY_ACT: // 日常活动
			bRet = true;
			break;
		case ENTRY_SETTINGS: // 设置
			bRet = true;
			break;
		case ENTRY_REFIT: // 改装
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_DOTAITEM);
			break;
		case ENTRY_LIMITED_SHOP: // 限购商城
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_PANIC_BUYING_STORE);
			break;
		case ENTRY_ACTIVITIES: // 活动
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_ACTIVITIES);
			break;
		case ENTRY_ELITE_WAR: // 精英副本
			bRet = P_WAR_MAP_CTRL->isEliteWarOpen();
			break;
		case ENTRY_SECRET_CHAT: // 私聊
			bRet = P_CHAT_CTRL->isNotifySecretChat();
			break;
		case ENTRY_DAILY_WAR: // 日常副本
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_DAILY_INSTANCE);
			break;
		default:;
	}*/

	return bRet;
}

void EntryControl::enter(unsigned int uEntryID, int uSecondID) {
	//P_NOTIFICATION_CTRL->entered(uEntryID);

	switch(uEntryID) {
		case ENTRY_ROLE: // 角色
			P_BAG_CTRL->getRoleInfoPanel()->open();
			break;
		case ENTRY_EQUIP_CULTURE: // 装备养成
			P_EQUIP_LIB_CTRL->getEquipLibListPanel()->open();
			break;
		case ENTRY_WEAPON_LIB: // 枪械库
			P_CITY_CTRL->getSubEntryPanel()->openWithType(SubEntryPanel::SUB_ENTRY_WEAPON);
			break;
		case ENTRY_GROWTH: // 成长
			P_CITY_CTRL->getSubEntryPanel()->openWithType(SubEntryPanel::SUB_ENTRY_GROWTH);
			break;
		case ENTRY_SOCIAL: // 社交
			P_CITY_CTRL->getSubEntryPanel()->openWithType(SubEntryPanel::SUB_ENTRY_SOCIAL);
			break;
		case ENTRY_CHALLENGE: // 挑战
			P_CITY_CTRL->getSubEntryPanel()->openWithType(SubEntryPanel::SUB_ENTRY_CHALLENGE);
			break;
		case ENTRY_BUSINESS: // 交易
			P_CITY_CTRL->getSubEntryPanel()->openWithType(SubEntryPanel::SUB_ENTRY_BUSINESS);
			break;
		case ENTRY_RANK: // 排行榜
			P_RANK_CTL->getRankPanel()->open();
			break;
		case ENTRY_SHOP: // 商城
			P_SHOP_CTRL->getRmbShopPanel()->open();
			break;
		case ENTRY_LOTTERY: // 夺宝奇兵
			P_LOTTERY_CTRL->getLotteryPanel()->open();
			break;
		case ENTRY_WEAPON_BOOK: // 枪魂
			P_QIANGHUN_CTRL->getQianghunPanel()->open();
			break;
		case ENTRY_CHAT: // 聊天
			P_CHAT_CTRL->getChatPanel()->open();
			break;
		case ENTRY_MAIL: // 邮件
			P_MAIL_CTRL->getMailUIPanel()->open();
			break;
		case ENTRY_DAILY_TASK: // 日常任务
			P_TASK_CTRL->getTaskPanel()->open();
			break;
		case ENTRY_EVOLUTION: // 进化
			P_EVOLUTION_CTRL->openEvolutionPanel();
			break;
		case ENTRY_ATTR_STRENGTHEN: // 属性强化
			P_ATTR_STRENGTHEN_CTRL->openAttrStrengthenInfoPanel();
			break;
		case ENTRY_GENE_STRENGTHEN: // 基因强化
			P_GEN_CTRL->getGenPanel()->open();
			break;
		case ENTRY_FRIEND: // 好友
			P_FRIEND_CTRL->getFriendPanelUI()->open();
			break;
		case ENTRY_GUILD: // 部落
			P_GUILD_CTRL->openGuild();
			break;
		case ENTRY_ARENA: // 竞技场
			P_ARENA_CTRL->getArenaPanel()->open();
			break;
		case ENTRY_NOTHING_LAND: // 虚无之地
			P_NIHILITY_CTRL->getNihilityPanel()->open();
			break;
		case ENTRY_BLACK_MARKET: // 黑市
			P_SHOP_CTRL->getBlackShopPanel()->open();
			break;
		case ENTRY_FRAGMENT: // 碎片
			P_FRAG_CTRL->getFragPanel()->open();
			break;
		case ENTRY_SIGN_IN: // 签到
			P_SIGN_IN_CTRL->getSignInPanel()->open();
			break;
		case ENTRY_BUY_COIN: // 兑换所
			P_VIP_CTRL->getBuyCoinPanel()->open();
			break;
		case ENTRY_DAILY_ACT: // 日常活动
			P_DAILYACT_CTRL->getDailyActPanel()->open();
			break;
		case ENTRY_SETTINGS: // 设置
			P_SET_CTRL->getSetPanel()->open();
			break;
		case ENTRY_REFIT: // 改装
			P_REFIT_CTRL->getRefitPanel()->open();
			break;
		case ENTRY_LIMITED_SHOP: // 限购商城
			P_SHOP_CTRL->getRestrictShopPanel()->open();
			break;
		case ENTRY_ACTIVITIES: // 活动
			P_ACT_CTRL->getActPanel()->open();
			break;
		case ENTRY_ELITE_WAR: // 精英副本
			P_WAR_MAP_CTRL->gotoEliteWar();
			if (uSecondID>=0)
			{
				P_WAR_MAP_CTRL->getWarMapPanel()->goToEliteByWarID(uSecondID);
			}
			break;
		case ENTRY_SECRET_CHAT: // 私聊
			P_CHAT_CTRL->getSecretChatPanel()->open();
			break;
		case ENTRY_DAILY_WAR: // 日常副本
			DAILY_INSTANCE_CTRL->getDailyInstancePanel()->open();
			break;
		case ENTRY_LEVEL_UP: // 提升等级
			P_WAR_MAP_CTRL->getLevelUpEntryPanel()->open();
			break;
		case ENTRY_WAR: // 普通副本
			P_WAR_MAP_CTRL->getWarMapPanel()->open();
			break;
		case ENTRY_BOSS_WAR: // 灭霸入侵
			P_DAILYACT_CTRL->getDailyActPanel()->open();
			break;
		case ENTRY_GET_AWARD: // 领取奖励
			P_CITY_CTRL->getGetAwardPanel()->open();
			break;
		case ENTRY_WAR_CHALLENGE: // 挑战副本
			// unused
			break;
		case ENTRY_WEAPON_CULTURE: // 枪械培养
			P_WEAPON_CTRL->getWeaponLibPanel()->open();
			break;
		case ENTRY_WEAPON_ARRAY: // 战备
			P_WEAPON_CTRL->getArrayPanel()->open();
			break;
		case ENTRY_FIRST_CHARGE_PKG: // 首充礼包
			P_SHOP_CTRL->getFirstChargePanel()->open();
			break;
		case ENTRY_VIP:	// vip
			P_VIP_CTRL->getVIPPanel()->open();
			break;
		case ENTRY_ONLINE_AWARD:
			P_ACT_CTRL->getOnlinePanel()->open();
			break;
		case ENTRY_MORROW_AWARD:
			P_ACT_CTRL->getNextDayPanel()->open();
			break;
		default:;
	}
}

unsigned int EntryControl::getEntryIDByModuleID(unsigned int uModuleID) {
	unsigned int uEntryID = 0;

	uEntryID = ENTRY_CFG_MASTER->getEntryIDByModuleID(uModuleID);

	/*switch(uModuleID) {
		case vmsg::MODULE_ID_BAG: // 背包
			uEntryID = ENTRY_ROLE;
			break;
		case vmsg::MODULE_ID_WEAPONLIB: // 枪械库
			uEntryID = ENTRY_WEAPON_LIB;
			break;
		case vmsg::MODULE_ID_EQUIPLVUP: // 装备升级
			uEntryID = ENTRY_EQUIP_CULTURE;
			break;
		case vmsg::MODULE_ID_EVOLUTION: // 进化
			uEntryID = ENTRY_EVOLUTION;
			break;
		case vmsg::MODULE_ID_ATTRSTRENGTHEN: // 属性强化
			uEntryID = ENTRY_ATTR_STRENGTHEN;
			break;
		case vmsg::MODULE_ID_FRIEND: // 好友
			uEntryID = ENTRY_FRIEND;
			break;
		case vmsg::MODULE_ID_GUILD: // 部落
			uEntryID = ENTRY_GUILD;
			break;
		case vmsg::MODULE_ID_ARENA: // 竞技场
			uEntryID = ENTRY_ARENA;
			break;
		case vmsg::MODULE_ID_NOTHINGLAND: // 虚无之地
			uEntryID = ENTRY_NOTHING_LAND;
			break;
		case vmsg::MODULE_ID_BLACKMARKET: //  黑市
			uEntryID = ENTRY_BLACK_MARKET;
			break;
		case vmsg::MODULE_ID_DEBRIS: // 碎片
			uEntryID = ENTRY_FRAGMENT;
			break;
		case vmsg::MODULE_ID_LOTTERY: // 夺宝奇兵
			uEntryID = ENTRY_LOTTERY;
			break;
		case vmsg::MODULE_ID_QIANGHUN: // 枪魂
			uEntryID = ENTRY_WEAPON_BOOK;
			break;
		case vmsg::MODULE_ID_MAIL: // 邮件
			uEntryID = ENTRY_MAIL;
			break;
		case vmsg::MODULE_ID_RANK: // 排行榜
			uEntryID = ENTRY_RANK;
			break;
		case vmsg::MODULE_ID_DAILYTASK: // 每日任务
		case vmsg::MODULE_ID_MILESTONE: // 成就
		case vmsg::MODULE_ID_TASK: // 主线任务
			uEntryID = ENTRY_DAILY_TASK;
			break;
		case vmsg::MODULE_ID_DOTAITEM: // 改装
			uEntryID = ENTRY_REFIT;
			break;
		case vmsg::MODULE_ID_ACTIVITIES: // 活动
			uEntryID = ENTRY_ACTIVITIES;
			break;
		case vmsg::MODULE_ID_DAILY_INSTANCE: // 日常副本
			uEntryID = ENTRY_DAILY_WAR;
			break;
		default:;
	}*/

	return uEntryID;
}

Button* EntryControl::createEntryBtn(unsigned int uEntryID) {
	Button *pBtn = Button::create();
	string btnName = string(ENTRY_PREFIX_NAME) + intToString(uEntryID);
	pBtn->setName(btnName.c_str());

	const EntryCfg *pEntryCfg = ENTRY_CFG_MASTER->getEntryCfg(uEntryID);
	string iconURL = string("icon/entry/") + pEntryCfg->entryicon() + ".png";
	pBtn->loadTextureNormal(iconURL.c_str());

	if (uEntryID == ENTRY_ONLINE_AWARD || uEntryID == ENTRY_MORROW_AWARD)
	{
		Label* pTimeLab = Label::create();
		pTimeLab->setFontName("fonts/04b_03b.ttf");
		pBtn->addChild(pTimeLab,1,Entry_Time_Lab);
		pTimeLab->setText("00:00:00");
		pTimeLab->setFontSize(22);
		pTimeLab->setColor(ccc3(255,244,0));
		pTimeLab->setPositionY(-60);
	}
	/*if(uEntryID == ENTRY_ROLE) {
		pBtn->loadTextureNormal("entry/btn_role_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_EQUIP_CULTURE) {
		pBtn->loadTextureNormal("entry/btn_equip_culture_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_WEAPON_LIB) {
		pBtn->loadTextureNormal("entry/btn_weapon_lib_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_GROWTH) { // 成长
		pBtn->loadTextureNormal("entry/btn_growth_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_SOCIAL) { // 社交
		pBtn->loadTextureNormal("entry/btn_social_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_CHALLENGE) { // 挑战
		pBtn->loadTextureNormal("entry/btn_challenge_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_BUSINESS) {
		pBtn->loadTextureNormal("entry/btn_business_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_RANK) {
		pBtn->loadTextureNormal("entry/btn_rank_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_SHOP) {
		pBtn->loadTextureNormal("entry/btn_shop_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_LOTTERY) {
		pBtn->loadTextureNormal("entry/btn_lottery_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_WEAPON_BOOK) { // 枪魂
		pBtn->loadTextureNormal("entry/btn_qianghun_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_CHAT) {

	} else if(uEntryID == ENTRY_MAIL) {

	} else if(uEntryID == ENTRY_DAILY_TASK) {
		pBtn->loadTextureNormal("entry/btn_daily_task_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_SIGN_IN) {
		pBtn->loadTextureNormal("entry/btn_sign_in_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_BUY_COIN) {
		pBtn->loadTextureNormal("entry/btn_buy_coin_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_DAILY_ACT) {
		pBtn->loadTextureNormal("entry/btn_daily_act_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_SETTINGS) {
		
	} else if(uEntryID == ENTRY_REFIT) {
		pBtn->loadTextureNormal("entry/btn_refit_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_LIMITED_SHOP) {
		pBtn->loadTextureNormal("entry/btn_limited_shop_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_ACTIVITIES) {
		pBtn->loadTextureNormal("entry/btn_activities_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_DAILY_WAR) {
		pBtn->loadTextureNormal("entry/btn_daily_war_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_EVOLUTION) {
		pBtn->loadTextureNormal("entry/btn_evolution_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_ATTR_STRENGTHEN) {
		pBtn->loadTextureNormal("entry/btn_attr_strengthen_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_FRIEND) {
		pBtn->loadTextureNormal("entry/btn_friend_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_GUILD) {
		pBtn->loadTextureNormal("entry/btn_guild_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_ARENA) {
		pBtn->loadTextureNormal("entry/btn_arena_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_NOTHING_LAND) {
		pBtn->loadTextureNormal("entry/btn_nothing_land_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_ELITE_WAR) {
		pBtn->loadTextureNormal("entry/btn_elite_war_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_BLACK_MARKET) {
		pBtn->loadTextureNormal("entry/btn_black_market_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_FRAGMENT) {
		pBtn->loadTextureNormal("entry/btn_fragment_entry_up.png", UI_TEX_TYPE_PLIST);
	}*/

	return pBtn;
}
