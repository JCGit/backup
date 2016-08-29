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
		return true; //��������ȫ�򿪣���QEnvSet.h��� #define TGAME_DEV_TEST 1
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

		if(uEntryID == ENTRY_ELITE_WAR) { // ��Ӣ����
			bRet = P_WAR_MAP_CTRL->isEliteWarOpen();
		} else if(uEntryID == ENTRY_SECRET_CHAT) { // ˽��
			bRet = P_CHAT_CTRL->isNotifySecretChat();
		} else if(uEntryID == ENTRY_GET_AWARD) {
			bRet = P_CITY_CTRL->getGetAwardPanel()->getAwardCnt() > 0;
		} else if(uEntryID == ENTRY_BOSS_WAR) {
			bRet = isEntryOpen(ENTRY_DAILY_ACT); // bossս��ÿ�ջ��
		} else if(uEntryID == ENTRY_FIRST_CHARGE_PKG) {
			bRet = P_SHOP_CTRL->getFirstChargePanel()->isMeetAutoOpen();
		}

		bRet = pEntryCfg->locked() == 0 && bRet;
	}

	/*switch(uEntryID) {
		case ENTRY_ROLE: // ��ɫ
			bRet = true;
			break;
		case ENTRY_EQUIP_CULTURE: // װ������
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_EQUIPLVUP);
			break;
		case ENTRY_WEAPON_LIB: // ǹе��
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_WEAPONLIB);;
			break;
		case ENTRY_GROWTH: // �ɳ�
			bRet = isEntryOpen(ENTRY_EVOLUTION)
				|| isEntryOpen(ENTRY_ATTR_STRENGTHEN);
			break;
		case ENTRY_SOCIAL: // �罻
			bRet = isEntryOpen(ENTRY_FRIEND)
				|| isEntryOpen(ENTRY_GUILD);
			break;
		case ENTRY_CHALLENGE: // ��ս
			bRet = isEntryOpen(ENTRY_ARENA)
				|| isEntryOpen(ENTRY_NOTHING_LAND)
				|| isEntryOpen(ENTRY_ELITE_WAR);
			break;
		case ENTRY_BUSINESS: // ����
			bRet = isEntryOpen(ENTRY_BLACK_MARKET)
				|| isEntryOpen(ENTRY_FRAGMENT);
			break;
		case ENTRY_RANK: // ���а�
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_RANK);
			break;
		case ENTRY_SHOP: // �̳�
			bRet = true;
			break;
		case ENTRY_LOTTERY: // �ᱦ���
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_LOTTERY);
			break;
		case ENTRY_WEAPON_BOOK: // ǹ��
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_QIANGHUN);
			break;
		case ENTRY_CHAT: // ����
			bRet = true;
			break;
		case ENTRY_MAIL: // �ʼ�
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_MAIL);
			break;
		case ENTRY_DAILY_TASK: // �ճ�����
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_DAILYTASK);
			break;
		case ENTRY_EVOLUTION: // ����
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_EVOLUTION);
			break;
		case ENTRY_ATTR_STRENGTHEN: // ����ǿ��
			bRet = false;
			break;
		case ENTRY_FRIEND: // ����
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_FRIEND);
			break;
		case ENTRY_GUILD: // ����
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_GUILD);
			break;
		case ENTRY_ARENA: // ������
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_ARENA);
			break;
		case ENTRY_NOTHING_LAND: // ����֮��
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_NOTHINGLAND);
			break;
		case ENTRY_BLACK_MARKET: // ����
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_BLACKMARKET);
			break;
		case ENTRY_FRAGMENT: // ��Ƭ
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_DEBRIS);
			break;
		case ENTRY_SIGN_IN: // ǩ��
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_SIGNIN);;
			break;
		case ENTRY_BUY_COIN: // �һ���
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_BUYCOIN);
			break;
		case ENTRY_DAILY_ACT: // �ճ��
			bRet = true;
			break;
		case ENTRY_SETTINGS: // ����
			bRet = true;
			break;
		case ENTRY_REFIT: // ��װ
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_DOTAITEM);
			break;
		case ENTRY_LIMITED_SHOP: // �޹��̳�
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_PANIC_BUYING_STORE);
			break;
		case ENTRY_ACTIVITIES: // �
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_ACTIVITIES);
			break;
		case ENTRY_ELITE_WAR: // ��Ӣ����
			bRet = P_WAR_MAP_CTRL->isEliteWarOpen();
			break;
		case ENTRY_SECRET_CHAT: // ˽��
			bRet = P_CHAT_CTRL->isNotifySecretChat();
			break;
		case ENTRY_DAILY_WAR: // �ճ�����
			bRet = P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_DAILY_INSTANCE);
			break;
		default:;
	}*/

	return bRet;
}

void EntryControl::enter(unsigned int uEntryID, int uSecondID) {
	//P_NOTIFICATION_CTRL->entered(uEntryID);

	switch(uEntryID) {
		case ENTRY_ROLE: // ��ɫ
			P_BAG_CTRL->getRoleInfoPanel()->open();
			break;
		case ENTRY_EQUIP_CULTURE: // װ������
			P_EQUIP_LIB_CTRL->getEquipLibListPanel()->open();
			break;
		case ENTRY_WEAPON_LIB: // ǹе��
			P_CITY_CTRL->getSubEntryPanel()->openWithType(SubEntryPanel::SUB_ENTRY_WEAPON);
			break;
		case ENTRY_GROWTH: // �ɳ�
			P_CITY_CTRL->getSubEntryPanel()->openWithType(SubEntryPanel::SUB_ENTRY_GROWTH);
			break;
		case ENTRY_SOCIAL: // �罻
			P_CITY_CTRL->getSubEntryPanel()->openWithType(SubEntryPanel::SUB_ENTRY_SOCIAL);
			break;
		case ENTRY_CHALLENGE: // ��ս
			P_CITY_CTRL->getSubEntryPanel()->openWithType(SubEntryPanel::SUB_ENTRY_CHALLENGE);
			break;
		case ENTRY_BUSINESS: // ����
			P_CITY_CTRL->getSubEntryPanel()->openWithType(SubEntryPanel::SUB_ENTRY_BUSINESS);
			break;
		case ENTRY_RANK: // ���а�
			P_RANK_CTL->getRankPanel()->open();
			break;
		case ENTRY_SHOP: // �̳�
			P_SHOP_CTRL->getRmbShopPanel()->open();
			break;
		case ENTRY_LOTTERY: // �ᱦ���
			P_LOTTERY_CTRL->getLotteryPanel()->open();
			break;
		case ENTRY_WEAPON_BOOK: // ǹ��
			P_QIANGHUN_CTRL->getQianghunPanel()->open();
			break;
		case ENTRY_CHAT: // ����
			P_CHAT_CTRL->getChatPanel()->open();
			break;
		case ENTRY_MAIL: // �ʼ�
			P_MAIL_CTRL->getMailUIPanel()->open();
			break;
		case ENTRY_DAILY_TASK: // �ճ�����
			P_TASK_CTRL->getTaskPanel()->open();
			break;
		case ENTRY_EVOLUTION: // ����
			P_EVOLUTION_CTRL->openEvolutionPanel();
			break;
		case ENTRY_ATTR_STRENGTHEN: // ����ǿ��
			P_ATTR_STRENGTHEN_CTRL->openAttrStrengthenInfoPanel();
			break;
		case ENTRY_GENE_STRENGTHEN: // ����ǿ��
			P_GEN_CTRL->getGenPanel()->open();
			break;
		case ENTRY_FRIEND: // ����
			P_FRIEND_CTRL->getFriendPanelUI()->open();
			break;
		case ENTRY_GUILD: // ����
			P_GUILD_CTRL->openGuild();
			break;
		case ENTRY_ARENA: // ������
			P_ARENA_CTRL->getArenaPanel()->open();
			break;
		case ENTRY_NOTHING_LAND: // ����֮��
			P_NIHILITY_CTRL->getNihilityPanel()->open();
			break;
		case ENTRY_BLACK_MARKET: // ����
			P_SHOP_CTRL->getBlackShopPanel()->open();
			break;
		case ENTRY_FRAGMENT: // ��Ƭ
			P_FRAG_CTRL->getFragPanel()->open();
			break;
		case ENTRY_SIGN_IN: // ǩ��
			P_SIGN_IN_CTRL->getSignInPanel()->open();
			break;
		case ENTRY_BUY_COIN: // �һ���
			P_VIP_CTRL->getBuyCoinPanel()->open();
			break;
		case ENTRY_DAILY_ACT: // �ճ��
			P_DAILYACT_CTRL->getDailyActPanel()->open();
			break;
		case ENTRY_SETTINGS: // ����
			P_SET_CTRL->getSetPanel()->open();
			break;
		case ENTRY_REFIT: // ��װ
			P_REFIT_CTRL->getRefitPanel()->open();
			break;
		case ENTRY_LIMITED_SHOP: // �޹��̳�
			P_SHOP_CTRL->getRestrictShopPanel()->open();
			break;
		case ENTRY_ACTIVITIES: // �
			P_ACT_CTRL->getActPanel()->open();
			break;
		case ENTRY_ELITE_WAR: // ��Ӣ����
			P_WAR_MAP_CTRL->gotoEliteWar();
			if (uSecondID>=0)
			{
				P_WAR_MAP_CTRL->getWarMapPanel()->goToEliteByWarID(uSecondID);
			}
			break;
		case ENTRY_SECRET_CHAT: // ˽��
			P_CHAT_CTRL->getSecretChatPanel()->open();
			break;
		case ENTRY_DAILY_WAR: // �ճ�����
			DAILY_INSTANCE_CTRL->getDailyInstancePanel()->open();
			break;
		case ENTRY_LEVEL_UP: // �����ȼ�
			P_WAR_MAP_CTRL->getLevelUpEntryPanel()->open();
			break;
		case ENTRY_WAR: // ��ͨ����
			P_WAR_MAP_CTRL->getWarMapPanel()->open();
			break;
		case ENTRY_BOSS_WAR: // �������
			P_DAILYACT_CTRL->getDailyActPanel()->open();
			break;
		case ENTRY_GET_AWARD: // ��ȡ����
			P_CITY_CTRL->getGetAwardPanel()->open();
			break;
		case ENTRY_WAR_CHALLENGE: // ��ս����
			// unused
			break;
		case ENTRY_WEAPON_CULTURE: // ǹе����
			P_WEAPON_CTRL->getWeaponLibPanel()->open();
			break;
		case ENTRY_WEAPON_ARRAY: // ս��
			P_WEAPON_CTRL->getArrayPanel()->open();
			break;
		case ENTRY_FIRST_CHARGE_PKG: // �׳����
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
		case vmsg::MODULE_ID_BAG: // ����
			uEntryID = ENTRY_ROLE;
			break;
		case vmsg::MODULE_ID_WEAPONLIB: // ǹе��
			uEntryID = ENTRY_WEAPON_LIB;
			break;
		case vmsg::MODULE_ID_EQUIPLVUP: // װ������
			uEntryID = ENTRY_EQUIP_CULTURE;
			break;
		case vmsg::MODULE_ID_EVOLUTION: // ����
			uEntryID = ENTRY_EVOLUTION;
			break;
		case vmsg::MODULE_ID_ATTRSTRENGTHEN: // ����ǿ��
			uEntryID = ENTRY_ATTR_STRENGTHEN;
			break;
		case vmsg::MODULE_ID_FRIEND: // ����
			uEntryID = ENTRY_FRIEND;
			break;
		case vmsg::MODULE_ID_GUILD: // ����
			uEntryID = ENTRY_GUILD;
			break;
		case vmsg::MODULE_ID_ARENA: // ������
			uEntryID = ENTRY_ARENA;
			break;
		case vmsg::MODULE_ID_NOTHINGLAND: // ����֮��
			uEntryID = ENTRY_NOTHING_LAND;
			break;
		case vmsg::MODULE_ID_BLACKMARKET: //  ����
			uEntryID = ENTRY_BLACK_MARKET;
			break;
		case vmsg::MODULE_ID_DEBRIS: // ��Ƭ
			uEntryID = ENTRY_FRAGMENT;
			break;
		case vmsg::MODULE_ID_LOTTERY: // �ᱦ���
			uEntryID = ENTRY_LOTTERY;
			break;
		case vmsg::MODULE_ID_QIANGHUN: // ǹ��
			uEntryID = ENTRY_WEAPON_BOOK;
			break;
		case vmsg::MODULE_ID_MAIL: // �ʼ�
			uEntryID = ENTRY_MAIL;
			break;
		case vmsg::MODULE_ID_RANK: // ���а�
			uEntryID = ENTRY_RANK;
			break;
		case vmsg::MODULE_ID_DAILYTASK: // ÿ������
		case vmsg::MODULE_ID_MILESTONE: // �ɾ�
		case vmsg::MODULE_ID_TASK: // ��������
			uEntryID = ENTRY_DAILY_TASK;
			break;
		case vmsg::MODULE_ID_DOTAITEM: // ��װ
			uEntryID = ENTRY_REFIT;
			break;
		case vmsg::MODULE_ID_ACTIVITIES: // �
			uEntryID = ENTRY_ACTIVITIES;
			break;
		case vmsg::MODULE_ID_DAILY_INSTANCE: // �ճ�����
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
	} else if(uEntryID == ENTRY_GROWTH) { // �ɳ�
		pBtn->loadTextureNormal("entry/btn_growth_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_SOCIAL) { // �罻
		pBtn->loadTextureNormal("entry/btn_social_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_CHALLENGE) { // ��ս
		pBtn->loadTextureNormal("entry/btn_challenge_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_BUSINESS) {
		pBtn->loadTextureNormal("entry/btn_business_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_RANK) {
		pBtn->loadTextureNormal("entry/btn_rank_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_SHOP) {
		pBtn->loadTextureNormal("entry/btn_shop_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_LOTTERY) {
		pBtn->loadTextureNormal("entry/btn_lottery_entry_up.png", UI_TEX_TYPE_PLIST);
	} else if(uEntryID == ENTRY_WEAPON_BOOK) { // ǹ��
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
