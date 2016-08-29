#include "FightProxy.h"
#include "ClientSvrMsg.pb.h"
#include "QNetMgr.h"
#include "FightControl.h"
#include "FightObj.h"
#include "UserObj.h"
#include "LevelObj.h"
#include "LevelControl.h"
#include "SceneManager.h"
#include "BaseMapScene.h"
#include "city/CityControl.h"
#include "city/CityProxy.h"
#include "QEnvSet.h"
#include "fightendless/FightEndlessControl.h"
#include "nihility/NihilityControl.h"
#include "fightguide/FightGuideScene.h"
#include "fightguide/FightGuideControl.h"
#include "basic/CommonDialog.h"
#include "basic/TextHelper.h"
#include "bag/BagControl.h"
#include "dailyinstance/DailyInstanceControl.h"

#include "WarMapCfgMaster.h"
#include "EliteWarMapCfgMaster.h"
#include "game/warmap/WarMapControl.h"
#include "basic/PopTipLayer.h"
#include "CfgMaster/PackageCfgMaster.h"
#include "game/Global.h"

CFightProxy::CFightProxy(){
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_WARMAP_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_WARMAP_START, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_WARMAP_END, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_WARMAP_CARD, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUERY_FIGHTINFO, this);	//战斗时查询人物详细信息
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_INVITE_FIGHT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_WARMAP_SWEEP, this);

	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ELITEWARMAP_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ELITEWARMAP_START, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ELITEWARMAP_END, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ELITEWARMAP_RESET, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ELITEWARMAP_SWEEP, this);

	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BUY_BULLET, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_RELIVE_HERO, this);

	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BUY_INSTANCE, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CLAIM_STAR_AWARD, this);
}

CFightProxy::~CFightProxy(){

}

void CFightProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg) {
	if(stMsg.has_errmsg()){

	}else if(stMsg.has_warmaprespond()) {
		vmsg::CSWarMapRespond& stWarMapRsp = *(stMsg.mutable_warmaprespond());

		if(stWarMapRsp.has_warmapquery_rsp()){
			P_LEVEL_OBJ->setMapInfo(stWarMapRsp.warmapquery_rsp());
			//P_LEVEL_CTL->getChoosePanel()->open();
			//P_LEVEL_CTL->getChoosePanel()->updateUI(); //old

			if(P_WAR_MAP_CTRL->getWarMapPanel() && P_WAR_MAP_CTRL->getWarMapPanel()->isOpen()) {
				P_WAR_MAP_CTRL->getWarMapPanel()->updateUI();
			}
		}else if(stWarMapRsp.has_warmapstart_rsp()){

			P_CITY_CTRL->getProxy()->stopOSD();
			if(P_FIGHT_OBJ->getFightType() == NORMAL_FIGHT_TYPE){   //普通关卡
				P_LEVEL_CTL->getEnterPanel()->close();
				P_LEVEL_CTL->getFriendHelpPanel()->close();
				P_WAR_MAP_CTRL->getWarMapPanel()->close();

				P_FIGHT_OBJ->setFightMapID(stWarMapRsp.warmapstart_rsp().warmapid());
				WarMapCfg* warCfg = WAR_MAP_CFG->GetWarMapCfgByID(stWarMapRsp.warmapstart_rsp().warmapid());
				SceneManager::sharedSceneManager()->enterScene(warCfg->mapid());
			}
			
		}else if(stWarMapRsp.has_warmapend_rsp()){
			P_FIGHT_OBJ->setAwardInfo(stWarMapRsp.warmapend_rsp());
			//POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_COIN, 0, stWarMapRsp.warmapend_rsp().awardcoin());
			//POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_EXP, 0, stWarMapRsp.warmapend_rsp().awardexp());
			P_FIGHT_CTRL->doNormalFightEnd();
		}else if(stWarMapRsp.has_warmapcard_rsp()){
			P_LEVEL_OBJ->setCardInfo(stWarMapRsp.warmapcard_rsp());

			P_LEVEL_CTL->getCardPanel()->updateCurCard();
		}else if(stWarMapRsp.has_sweepwar_rsp()){   //扫荡
			P_LEVEL_OBJ->setSweepInfo(stWarMapRsp.sweepwar_rsp());

			P_LEVEL_CTL->getSweepInfoPanel()->updateUI();

			if(P_LEVEL_CTL->getEnterPanel()->isOpen()){
				P_LEVEL_CTL->getEnterPanel()->updateUI();
			}
		} else if(stWarMapRsp.has_queryelitewarmap_rsp()) { // 查询精英副本
			P_LEVEL_OBJ->setEliteWarInfo(stWarMapRsp.queryelitewarmap_rsp());
			P_LEVEL_OBJ->setMaxEliteWarMapID(stWarMapRsp.queryelitewarmap_rsp().maxelitewarmapid());
			
			if(P_WAR_MAP_CTRL->getWarMapPanel()->isOpen()) {
				P_WAR_MAP_CTRL->getWarMapPanel()->updateUI();
			}

			if(P_LEVEL_CTL->getEliteWarEntryPanel()->isOpenWithQuery()) {
				P_LEVEL_CTL->getEliteWarEntryPanel()->open();
			}
		} else if(stWarMapRsp.has_startelitewarmap_rsp()) { // 开始精英副本
			P_LEVEL_OBJ->setIsHasFriend(false);
			P_FIGHT_OBJ->setFightMapID(stWarMapRsp.startelitewarmap_rsp().elitewarmapid());

			if(P_FIGHT_OBJ->getFightType() == ELITE_FIGHT_TYPE){   //精英关卡
				P_LEVEL_CTL->getEliteWarEntryPanel()->close();
				P_WAR_MAP_CTRL->getWarMapPanel()->close();

				unsigned int eliteID = stWarMapRsp.startelitewarmap_rsp().elitewarmapid();
				const EliteWarMapCfg* stCfg = ELITE_WAR_MAP_CFG_MASTER->getEliteWarMapCfg(eliteID);
				SceneManager::sharedSceneManager()->enterScene(stCfg->mapid());
			}

		} else if(stWarMapRsp.has_endelitewarmap_rsp()) { // 结束精英副本
			P_FIGHT_OBJ->setEliteEndInfo(stWarMapRsp.endelitewarmap_rsp());
			P_FIGHT_CTRL->doEliteFightEnd();

		} else if(stWarMapRsp.has_resetelitewarmap_rsp()) { // 重置精英副本
			P_LEVEL_OBJ->setEliteWarInfo(stWarMapRsp.resetelitewarmap_rsp());
			P_LEVEL_OBJ->setMaxEliteWarMapID(stWarMapRsp.resetelitewarmap_rsp().maxelitewarmapid());

			if(P_WAR_MAP_CTRL->getWarMapPanel()->isOpen()) {
				P_WAR_MAP_CTRL->getWarMapPanel()->updateUI();
			}
		} else if(stWarMapRsp.has_sweepelitewar_rsp()) { // 扫荡精英副本
			P_LEVEL_OBJ->setEliteWarInfo(stWarMapRsp.sweepelitewar_rsp().info());
			P_LEVEL_OBJ->setMaxEliteWarMapID(stWarMapRsp.sweepelitewar_rsp().info().maxelitewarmapid());
			
			P_LEVEL_OBJ->setEliteSweepInfo(stWarMapRsp.sweepelitewar_rsp());
			
			if(stWarMapRsp.sweepelitewar_rsp().errorno() > 0 && stWarMapRsp.sweepelitewar_rsp().awardlist_size() == 0) {
				COMMON_DIALOG->show(TextStr(TEXT_BAG_FULL_OP_TIP));
			} else {
				P_LEVEL_CTL->getWarSweepInfoPanel()->open();
				P_LEVEL_CTL->getWarSweepInfoPanel()->updateUI();
			}

			if(P_WAR_MAP_CTRL->getWarMapPanel()->isOpen()) {
				P_WAR_MAP_CTRL->getWarMapPanel()->updateUI();
			}
		}else if(stWarMapRsp.has_buybullet_rsp()){  //购买子弹返回
			P_FIGHT_CTRL->doBuyBulletRsp(stWarMapRsp.buybullet_rsp());
		}else if(stWarMapRsp.has_relive_rsp()){     //复活返回
			P_FIGHT_CTRL->doReliveRsp(stWarMapRsp.relive_rsp());
		}else if(stWarMapRsp.has_buyinstance_rsp()){     //购买次数返回
			P_LEVEL_OBJ->doBuyFightCntRsp(stWarMapRsp.buyinstance_rsp());

			if(P_LEVEL_CTL->getEnterPanel()->isOpen()){
				P_LEVEL_CTL->getEnterPanel()->updateUI();
			}
		} else if(stWarMapRsp.has_claimstaraward_rsp()) {
			vmsg::CSWarMapQueryRsp &stWarMapQueryRsp = P_LEVEL_OBJ->getMapInfo();

			stWarMapQueryRsp.mutable_warstarawards()->CopyFrom(stWarMapRsp.claimstaraward_rsp().warstarawards());

			for(int i = 0; i < stWarMapRsp.claimstaraward_rsp().award_size(); i++) {
				const vmsg::CSAwardElement &stAwardElmt = stWarMapRsp.claimstaraward_rsp().award(i);
				
				if(stAwardElmt.elementtype() == vmsg::THING_TYPE_COIN) {
					// 金币已有提示
				} else if(stAwardElmt.elementtype() == vmsg::THING_TYPE_WEAPON) {
					P_GLOBAL->showAdvItemEff(vmsg::THING_TYPE_WEAPON, stAwardElmt.elementid()); //显示高级物品获得特效
				} else {
					POP_TIP_LAYER_MASTER->ShowAwardTip(stAwardElmt.elementtype(), stAwardElmt.elementid(), stAwardElmt.elementcnt());
				}
			}

			if(P_WAR_MAP_CTRL->getWarMapPanel()->isOpen()) {
				P_WAR_MAP_CTRL->getWarMapPanel()->updateUI();
			}
		}
	}else if(stMsg.has_playerrespond()){
		vmsg::CSPlayerRespond playerRsp = stMsg.playerrespond();
		if(playerRsp.has_querydetailinfo_rsp()){  //角色详细信息
			P_FIGHT_OBJ->resetCost();			
			P_USER_OBJ->updateUserDetailInfo(playerRsp.querydetailinfo_rsp());
			P_FIGHT_OBJ->setRealWeapon(P_USER_OBJ->getUserInfo()->weapon);
			P_BAG_CTRL->sendQueryBagInfoRqst();

			if(P_FIGHT_OBJ->getFightType() == NORMAL_FIGHT_TYPE){   //普通关卡
				//发送战斗开始协议
				unsigned int levelID = P_LEVEL_OBJ->getEnterLevelID();
				sendWarMapStartRqst(levelID);
				P_WAR_MAP_CTRL->setWarMapID(levelID);
			}else if(P_FIGHT_OBJ->getFightType() == ENDLESS_FIGHT_TYPE){    //无尽模式
				P_CITY_CTRL->getProxy()->stopOSD();
				P_ENDLESS_CTRL->sendEndlessStartRqst();
			}else if(P_FIGHT_OBJ->getFightType() == NIHILITY_FIGHT_TYPE){    //虚无之地
				P_CITY_CTRL->getProxy()->stopOSD();
				P_NIHILITY_CTRL->sendNihilityFightStartRqst();
			}else if(P_FIGHT_OBJ->getFightType() == GUIDE_FIGHT_TYPE){    //指引战斗关卡
				P_CITY_CTRL->getProxy()->stopOSD();
				SceneManager::sharedSceneManager()->enterScene(FIGHT_GUIDE_SCENE_ID);
			}else if(P_FIGHT_OBJ->getFightType() == GUIDE_NORMAL_FIGHT_TYPE){    //指引正常战斗关卡
				P_CITY_CTRL->getProxy()->stopOSD();
				SceneManager::sharedSceneManager()->enterScene(FIGHT_GUIDE_NORMAL_SCENE_ID);
			}else if(P_FIGHT_OBJ->getFightType() == ELITE_FIGHT_TYPE){    //精英副本
				unsigned int eliteWarMapID = P_LEVEL_OBJ->getEnterLevelID();
				sendStartEliteWarMapRqst(eliteWarMapID);
				P_WAR_MAP_CTRL->setEliteWarMapID(eliteWarMapID);
			}else if(P_FIGHT_OBJ->getFightType() == DAILY_FIGHT_TYPE){    //日常副本
				DAILY_INSTANCE_CTRL->getDailyInstancePanel()->close();
				P_CITY_CTRL->getProxy()->stopOSD();
				SceneManager::sharedSceneManager()->enterScene(DAILY_INSTANCE_CTRL->getWarMapID());
			}
		}
	}else if(stMsg.has_friendrespond()){
		vmsg::CSFriendRespond& stFriendRsp = *(stMsg.mutable_friendrespond());
		if(stFriendRsp.has_invitefriend_rsp()){  //好友助战
			P_LEVEL_CTL->doInviteFriendFightRsp(stFriendRsp.invitefriend_rsp().friendfightinfo());
		}
	}
}

void CFightProxy::sendQueryWarMapInfoRqst(unsigned int fromID, unsigned int toID){
	unsigned int uCmd = vmsg::CS_CMD_WARMAP_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);
	
	vmsg::CSWarMapQueryRqst* stWarMapQuery = stMsg.mutable_warmaprequest()->mutable_warmapquery_rqst();
	stWarMapQuery->set_fromid(fromID);
	stWarMapQuery->set_toid(toID);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendWarMapStartRqst(unsigned int warMapID){
	unsigned int uCmd = vmsg::CS_CMD_WARMAP_START;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSWarMapStartRqst* stWarMapStart = stMsg.mutable_warmaprequest()->mutable_warmapstart_rqst();
	stWarMapStart->set_warmapid(warMapID);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendWarMapEndRqst(vmsg::CSWarMapEndRqst& stWarEndRqst){
	unsigned int uCmd = vmsg::CS_CMD_WARMAP_END;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSWarMapEndRqst* stWarMapEnd = stMsg.mutable_warmaprequest()->mutable_warmapend_rqst();
	*stWarMapEnd = stWarEndRqst;
	
	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendWarMapCardRqst(unsigned int warMapID, unsigned int isFree){
	unsigned int uCmd = vmsg::CS_CMD_WARMAP_CARD;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSWarMapCardRqst* stCardRqst =  stMsg.mutable_warmaprequest()->mutable_warmapcard_rqst();
	stCardRqst->set_warmapid(warMapID);
	stCardRqst->set_isfree(isFree);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendQueryPlayerFightInfo(){
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERY_FIGHTINFO;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendInviteFriendFightRqst(UIN_t friendUin){
	unsigned int uCmd = vmsg::CS_CMD_FRIEND_INVITE_FIGHT;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSInviteFriendFightRqst* stInviteRqst = stMsg.mutable_friendrequest()->mutable_invitefight_rqst();
	stInviteRqst->set_frienduin(friendUin);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendSweepWarRqst(unsigned int warMapID, unsigned int round){
	unsigned int uCmd = vmsg::CS_CMD_WARMAP_SWEEP;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSSweepWarRqst* stSweepRqst = stMsg.mutable_warmaprequest()->mutable_sweepwar_rqst();
	stSweepRqst->set_warmapid(warMapID);
	stSweepRqst->set_sweepround(round);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendQueryEliteWarMapRqst(unsigned int uEliteWarChapterID) {
	unsigned int uCmd = vmsg::CS_CMD_ELITEWARMAP_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSWarMapRequest *pWarMapRqst = stMsg.mutable_warmaprequest();
	vmsg::CSQueryEliteWarMapRqst *pQueryEliteWarMapRqst = pWarMapRqst->mutable_queryelitewarmap_rqst();
	pQueryEliteWarMapRqst->set_elitewarid(uEliteWarChapterID);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendStartEliteWarMapRqst(unsigned int uEliteWarMapID) {
	unsigned int uCmd = vmsg::CS_CMD_ELITEWARMAP_START;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSWarMapRequest *pWarMapRqst = stMsg.mutable_warmaprequest();
	vmsg::CSStartEliteWarMapRqst *pStartEliteWarMapRqst = pWarMapRqst->mutable_startelitewarmap_rqst();
	pStartEliteWarMapRqst->set_elitewarmapid(uEliteWarMapID);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendEndEliteWarMapRqst(unsigned int uEliteWarMapID, unsigned int uEvolutionUseCnt, unsigned int uScore) {
	unsigned int uCmd = vmsg::CS_CMD_ELITEWARMAP_END;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSWarMapRequest *pWarMapRqst = stMsg.mutable_warmaprequest();
	vmsg::CSEndEliteWarMapRqst *pEndEliteWarMapRqst = pWarMapRqst->mutable_endelitewarmap_rqst();
	pEndEliteWarMapRqst->set_elitewarmapid(uEliteWarMapID);
	pEndEliteWarMapRqst->set_evolutionusecnt(uEvolutionUseCnt);
	pEndEliteWarMapRqst->set_score(uScore);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendResetEliteWarMapRqst(unsigned int uEliteWarChapterID) {
	unsigned int uCmd = vmsg::CS_CMD_ELITEWARMAP_RESET;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSWarMapRequest *pWarMapRqst = stMsg.mutable_warmaprequest();
	vmsg::CSResetEliteWarMapRqst *pResetEliteWarMapRqst = pWarMapRqst->mutable_resetelitewarmap_rqst();
	pResetEliteWarMapRqst->set_elitewarid(uEliteWarChapterID);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendSweepEliteWarMapRqst(unsigned int uEliteWarChapterID) {
	unsigned int uCmd = vmsg::CS_CMD_ELITEWARMAP_SWEEP;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSWarMapRequest *pWarMapRqst = stMsg.mutable_warmaprequest();
	vmsg::CSSweepEliteWarRqst *pSweepEliteWarMapRqst = pWarMapRqst->mutable_sweepelitewarmap_rqst();
	pSweepEliteWarMapRqst->set_elitewarid(uEliteWarChapterID);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendBuyBulletRqst(unsigned int uIsUseItem, unsigned int uItemID, int uBagIdx){
	unsigned int uCmd = vmsg::CS_CMD_BUY_BULLET;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSWarMapRequest *pWarMapRqst = stMsg.mutable_warmaprequest();
	vmsg::CSBuyBulletRqst *pBuyRqst = pWarMapRqst->mutable_buybullet_rqst();
	pBuyRqst->set_isuseitem(uIsUseItem);

	if(uItemID != 0){
		pBuyRqst->set_itemid(uItemID);
		pBuyRqst->set_itembagidx(uBagIdx);
	}

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendReliveRqst(unsigned int uIsUseItem, unsigned int uItemID, int uBagIdx){
	unsigned int uCmd = vmsg::CS_CMD_RELIVE_HERO;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSWarMapRequest *pWarMapRqst = stMsg.mutable_warmaprequest();
	vmsg::CSReliveRqst *pReliveqst = pWarMapRqst->mutable_relive_rqst();
	pReliveqst->set_isuseitem(uIsUseItem);

	if(uIsUseItem != 0){
		pReliveqst->set_itemid(uItemID);
		pReliveqst->set_itembagidx(uBagIdx);
	}

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendBuyInstanceRqst(unsigned int uWarID){
	unsigned int uCmd = vmsg::CS_CMD_BUY_INSTANCE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSWarMapRequest *pWarMapRqst = stMsg.mutable_warmaprequest();
	vmsg::CSBuyInstanceRqst *pBuyInstanceRqst = pWarMapRqst->mutable_buyinstance_rqst();
	pBuyInstanceRqst->set_count(1);
	pBuyInstanceRqst->set_warid(uWarID);

	P_NET_MGR->SendPkg(stMsg);
}

void CFightProxy::sendGetStarAwardRqst(unsigned int uWarID, unsigned int uStarCnt) {
	unsigned int uCmd = vmsg::CS_CMD_CLAIM_STAR_AWARD;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSWarMapRequest *pWarMapRqst = stMsg.mutable_warmaprequest();
	vmsg::CSWarmapStarAwardRqst *pGetStarAwardRqst = pWarMapRqst->mutable_claimstaraward_rqst();
	pGetStarAwardRqst->set_warid(uWarID);
	pGetStarAwardRqst->set_starcnt(uStarCnt);

	P_NET_MGR->SendPkg(stMsg);
}
