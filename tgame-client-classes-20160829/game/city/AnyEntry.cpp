#include "AnyEntry.h"
#include "QEnvSet.h"
#include "CityScene.h"
#include "game/fight/FightControl.h"
#include "game/fight/FightProxy.h"
#include "CfgMaster/WarMapCfgMaster.h"
#include "PopTipLayer.h"
#include "vip/VIPControl.h"
#include "vip/VIPPanel.h"
#include "shop/ShopControl.h"
#include "arena/ArenaControl.h"
#include "qianghun/QianghunControl.h"
#include "game/chat/ChatControl.h"
#include "CityControl.h"
#include "loading/LoadingScreen.h"
#include "weaponlib/WeaponLibControl.h"
#include "friend/FriendControl.h"
#include "fight/LevelControl.h"
#include "mail/MailControl.h"
#include "Global.h"
#include "fight/FightObj.h"
#include "nihility/NihilityControl.h"
#include "NounHelper.h"
#include "game/guide/GuideControl.h"
#include "game/evolution/EvolutionControl.h"
#include "game/guild/GuildControl.h"
#include "movie/MovieControl.h"
#include "equiplib/EquipLibControl.h"
#include "fightguide/FightGuideControl.h"
#include "game/attrstrengthen/AttrStrengthenControl.h"
#include "frag/FragControl.h"
#include "QNetMgr.h"
#include "TextHelper.h"
#include "GameInf.h"
#include "game/warmap/WarMapControl.h"
#include "vip/VIPControl.h"
#include "vip/BuyCoinPanel.h"
#include "signin/SignInControl.h"
#include "login/UserGoldControl.h"
#include "shop/RmbShopPanel.h"
#include "dailyact/DailyActControl.h"
#include "CommonDialog.h"
#include "TextCfgMaster.h"
#include "notification/NotificationControl.h"
#include "bag/BagControl.h"
#include "ModuleID.pb.h"
#include "UserObj.h"
#include "evtlog/EvtLog.h"
#include "act/ActControl.h"
#include "rank/RankControl.h"
#include "dailyinstance/DailyInstanceControl.h"
#include "ChannelInfoCallback.h"
#include "game/lottery/LotteryControl.h"
#include "game/tip/TipControl.h"
#include "gen/GenStrengthenControl.h"
#include "fight/HideAwardLayout.h"
#include "ToolUtils.h"

void AnyEntry::exec() {

	if(QEnvSet::taddy()) {
		//P_VIP_CTRL->getVIPPanel()->open();
		//P_SHOP_CTRL->getShopPanel()->open();
		//P_CHAT_CTRL->getSecretChatPanel()->open();
		//LoadingScreen::getInstance()->open();
		//P_CITY_CTRL->moveToNPC(1);
		//P_CITY_CTRL->getCityScene()->moveToNPC(1);
		//SceneManager::sharedSceneManager()->enterScene(3);
		//P_EVOLUTION_CTRL->openEvolutionPanel();
		//P_CITY_CTRL->getModuleEntryLayout()->unfoldEntry();
		//P_RANK_CTL->getRankPanel()->open();
		//P_ATTR_STRENGTHEN_CTRL->openAttrStrengthenInfoPanel();
		//P_WAR_MAP_CTRL ->getWarMapPanel()->open();
		//P_CITY_CTRL->getCityScreenView()->moduleOpen(vmsg::MODULE_ID_WEAPONLIB);
		P_GUIDE_CTRL->startGuide(GUIDE_ID_WEAPON_LVUP_AGAIN);
		P_CITY_CTRL->getCityScreenView()->unfoldEntry();
		//P_CITY_CTRL->getAwardPromptPanel()->open();
		//P_WAR_MAP_CTRL->getBeStrongerPanel()->open();
		//P_TIP_CTRL->getItemLackPanel()->open();
		//P_BAG_CTRL->getPackageInfoPanel()->showPackage(802);
		//static int i = 10;
		//P_NOTIFICATION_CTRL->getNotificationPanel()->openWithNotification(7);
		/*static int i = 1;
		vmsg::CSWarMapEndRqst stRqst;
		stRqst.set_warmapid(3);
		stRqst.set_evolutionusecnt(5);
		stRqst.set_bossid(0);
		stRqst.set_score(3);
		vmsg::CSKillMonsterInfo *pInfo = stRqst.add_monsterinfos();
		pInfo->set_monsterid(1006);
		pInfo->set_monstercnt(25);
		pInfo = stRqst.add_monsterinfos();
		pInfo->set_monsterid(1007);
		pInfo->set_monstercnt(40);
		pInfo = stRqst.add_monsterinfos();
		pInfo->set_monsterid(1008);
		pInfo->set_monstercnt(1);*/

		/*vmsg::CSWarMapEndRqst stRqst;
		stRqst.set_warmapid(2);
		stRqst.set_evolutionusecnt(5);
		stRqst.set_bossid(0);
		stRqst.set_score(3);
		vmsg::CSKillMonsterInfo *pInfo = stRqst.add_monsterinfos();
		pInfo->set_monsterid(1003);
		pInfo->set_monstercnt(4);
		pInfo = stRqst.add_monsterinfos();
		pInfo->set_monsterid(1004);
		pInfo->set_monstercnt(18);
		pInfo = stRqst.add_monsterinfos();
		pInfo->set_monsterid(1005);
		pInfo->set_monstercnt(24);*/

		//P_FIGHT_CTRL->getFightProxy()->sendWarMapEndRqst(stRqst);
		//P_FIGHT_CTRL->getFightProxy()->sendResetEliteWarMapRqst(2);
		/*P_LOTTERY_CTRL->init();
		P_LOTTERY_CTRL->getLotteryVIPTipPanel()->open();*/

		//string text = "";

		//rapidjson::Document json;
		//unsigned long size;
		////unsigned char *pBytes = CCFileUtils::sharedFileUtils()->getFileData(pszDirCfgJson , "r", &size);
		//std::string load_str = text;
		//json.Parse<0>(load_str.c_str());

		//if (json.HasParseError()) {
		//	CCLOG("------------------------------------error");
		//}


		//if(json.HasMember("channel") && json["channel"].IsString()) {
		//	CCLOG(json["channel"].GetString());
		//}
		return;
	}

	if(QEnvSet::beck() || QEnvSet::linc()){ 
		//P_SHOP_CTRL->getRestrictShopPanel()->open();
		//P_ACT_CTRL->getActPanel()->open();

		//P_LEVEL_CTL->getChoosePanel()->open();

		//P_GEN_CTRL->getGenPanel()->open();
		//P_VIP_CTRL->getVIPPanel()->open();

		/*P_BAG_CTRL->getBuyItemPanel()->open();
		P_BAG_CTRL->getBuyItemPanel()->setCfgID(17006);*/

		P_WEAPON_CTRL->getArrayPanel()->open();
		//P_LEVEL_CTL->getRelivePanel()->open();
		//P_LEVEL_CTL->getFillBulletPanel()->open();

		//P_DAILYACT_CTRL->getDailyActPanel()->open();
		//DAILY_INSTANCE_CTRL->getDailyInstancePanel()->open();
		//P_ARENA_CTRL->getArenaPanel()->open();
		//P_NIHILITY_CTRL->getGenPanel()->open();
		//P_NIHILITY_CTRL->getNihilityPanel()->open();

		//P_CITY_CTRL->getCityScene()->moveToNPC(1);
		//P_LEVEL_CTL->getCardPanel()->open();
		//P_VIP_CTRL->getBuyCoinPanel()->open();
		//P_QIANGHUN_CTRL->getQianghunPanel()->open();
		//P_RANK_CTL->getRankPanel()->open();
		//P_LEVEL_CTL->getCardPanel()->open();

		//P_ENDLESS_CTRL->getEndlessRankPanel()->open();

		//P_ARM_CTRL->getRefitShowPanel()->open();

		//P_LEVEL_CTL->openSweepPanelByLevelID(1);

		//P_ARM_CTRL->getMaterialShowPanel()->open();
		//pScene->moveToNPC(1);

		//POP_TIP_LAYER_MASTER->ShowTipStr("5555555555555555");

		//P_WEAPON_CTRL->getWeaponLibPanel()->open();
		//P_NIHILITY_CTRL->getNihilityPanel()->open();
		//P_FIGHT_GUIDE_CTRL->enterGuideNormalScene();
		//P_FIGHT_GUIDE_CTRL->enterFightGuideScene();

		//POP_TIP_LAYER_MASTER->ShowErrorStr("start buy");

		//P_FIGHT_OBJ->setFightType(GUIDE_FIGHT_TYPE);
		//P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();

		//P_ENDLESS_CTRL->getEndlessRankPanel()->open();

		//static int i = 1;
		//if(i == 1) {
		//	P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();//TODO:DEL
		//	i = 2;
		//} else {
		//	SceneManager::sharedSceneManager()->enterScene(30002);
		//}

		//P_ENDLESS_CTRL->getEndlessRankPanel()->open();

		//P_RANK_CTL->getRankPanel()->open();

		//P_ARM_CTRL->getTestPanel()->open();
		return;
	}

    if(QEnvSet::neil()) {
        execNeil();
        return;

    }
    //SceneManager::sharedSceneManager()->enterScene(50001);
    //return; //暂时作为日常活动入口

	//正式代码在这里
	//P_LEVEL_CTL->getChoosePanel()->open();
	//P_ARENA_CTRL->getArenaPanel()->open();
	//P_QIANGHUN_CTRL->getQianghunPanel()->open();
}

void AnyEntry::execNeil() {
    //P_ARENA_CTRL->getArenaPanel()->open(); //竞技场
    //P_NOTIFICATION_CTRL->getNotificationPanel()->openWithNotification(1);
    //SceneManager::sharedSceneManager()->enterScene(1); 
    //P_GUIDE_CTRL->startGuide(22);
    //P_CITY_CTRL->getModuleEntryLayout()->moduleOpen(5);
    //CCLOG("%s", TextStr(TEXT_NET_RECONNECT));
    //P_VIP_CTRL->getBuyCoinPanel()->open(); //招财符
    //P_SIGN_IN_CTRL->getSignInPanel()->open(); //签到
    //P_WEAPON_LOT_CTRL->getWeaponLotPanel()->open(); //探秘
    //P_LEVEL_CTL->getChoosePanel()->open();
    //P_WEAPON_CTRL->getWeaponLibPanel()->open(); //枪械库
    //P_WEAPON_CTRL->getWeaponSelPanel()->open();
    //P_VIP_CTRL->getVIPPanel()->open(); //vip
    //P_LEVEL_CTL->getFriendHelpPanel()->open();
    //P_FRIEND_CTRL->getProxy()->sendQueryFriendRqst();
    //P_SHOP_CTRL->getBlackShopPanel()->open(); //黑市
    //P_SHOP_CTRL->getRmbShopPanel()->open(); //RMB
    //P_FRAG_CTRL->getFragPanel()->open(); //碎片
    //P_SHOP_CTRL->getRmbShopPanel()->open();

    //P_EQUIP_LIB_CTRL->getEquipLibListPanel()->open(); //装备
    //P_WAR_MAP_CTRL->getWarMapPanel()->open(); //关卡

    POP_TIP_LAYER_MASTER->ShowErrorStr("click");
    //P_CITY_CTRL->showLvUpEffect();
    //P_GAME_INF->sendSysNotify("Title", "txt", 15);
    /*
    std::string strToken = "2.0";
    int aa = -1;
    sscanf(strToken.c_str(), "%d", &aa);
    CCLOG("=============[%s][%d]", strToken.c_str(), aa);
    CCLOG("=============[%s][%d]", strToken.c_str(), aa);
    */
    //test
    static bool bt = true;
    if(bt) {
        P_USER_GOLD_CTRL->GetProxy()->sendAppAuthedBuyRqst("1001", 1);
        CCLOG("start buy");
    } else {
        P_USER_GOLD_CTRL->onAppAuthedBuyDone(true);
        CCLOG("success buy");
    }
    bt = !bt;
    return;


    
}
