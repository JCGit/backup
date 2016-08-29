#include "DailyActProxy.h"

#include "ClientSvrMsg.pb.h"
#include "QNetMgr.h"
#include "DailyActControl.h"
#include "CSCVZMsg.pb.h"
#include "GuardActScene.h"
#include "UserObj.h"
#include "SvrNotifyMaster.h"
#include "PopTipLayer.h"
#include "city/CityControl.h"

using namespace vmsg;
using namespace std;

DailyActProxy::DailyActProxy()
{
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_HITMONSTER, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_HITED, this);		
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_DESTROYBASE, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_CONTROL, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_RMCONTROL, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_NOTIFYBASE, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_NOTIFYFAIL, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_NOTIFYHITMONSTER, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_NOTIFYHITED, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_ROOMOK, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_SUBMITMONSTERS, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_NOTIFYMONSTERS, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_NOTIFYRANKINFO, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_NOTIFYWAVETIME, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_NOTIFYOVER, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_NOTIFYMYCONTAWARD, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CVZ_NOTIFYTOTALCONT, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_SERVER_NOTIFY, this); //服务器通告
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ACT_QUERY, this);	
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ACT_STAT_NOTIFY, this);	
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_GETMANUALACTAWARD, this);
}

DailyActProxy::~DailyActProxy()
{
}

void DailyActProxy::sendQueryDailyAct(){
	unsigned int uCmd = CS_CMD_ACT_QUERY;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void DailyActProxy::sendHitMonsterRqst(vmsg::CSCVZHitMonsterRqst& stRqst){
    unsigned int uCmd = CS_CMD_CVZ_HITMONSTER;
    VCli2LogicSvrMsg stMsg;
    MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
        
    CSCVZRequest* pcvzRqst = stMsg.mutable_cvzrequest();
    *(pcvzRqst->mutable_hitmonster_rqst()) = stRqst;

    P_NET_MGR->SendPkg(stMsg);
}



void DailyActProxy::sendHitedRqst(const int iHP) {
    unsigned int uCmd = CS_CMD_CVZ_HITED;
    VCli2LogicSvrMsg stMsg;
    MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    CSCVZRequest* pcvzRqst = stMsg.mutable_cvzrequest();
    pcvzRqst->mutable_hited_rqst()->set_hp(iHP);

    P_NET_MGR->SendPkg(stMsg);
}


void DailyActProxy::sendDestroyBaseRqst(const int iBaseID){
    unsigned int uCmd = CS_CMD_CVZ_DESTROYBASE;
    VCli2LogicSvrMsg stMsg;
    MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    CSCVZRequest* pcvzRqst = stMsg.mutable_cvzrequest();
    pcvzRqst->set_destroybase_rqst(iBaseID);

    P_NET_MGR->SendPkg(stMsg);
}


void DailyActProxy::sendSubmitMonsterPosRqst(const CSCVZMonsters& stMonsterInfo){
    unsigned int uCmd = vmsg::CS_CMD_CVZ_SUBMITMONSTERS;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    CSCVZRequest* pcvzRqst = stMsg.mutable_cvzrequest();
    *(pcvzRqst->mutable_submitmonster_rqst()) = stMonsterInfo;
    

    P_NET_MGR->SendPkg(stMsg);
}


void DailyActProxy::sendSubmitMonsterSkillRqst(const vmsg::CSCVZMonsterInfo& stOneInfo) {
    CSCVZMonsters stMonsterInfo;
    CSCVZMonsterInfo* pOne = stMonsterInfo.add_monsters();
    *pOne = stOneInfo;

    sendSubmitMonsterPosRqst(stMonsterInfo);
}

void DailyActProxy::sendGetManualActAwardRqst(){
	unsigned int uCmd = CS_CMD_PLAYER_GETMANUALACTAWARD;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void DailyActProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg) 
{
    if(CS_CMD_SERVER_NOTIFY == stMsg.msg_head().msg_id()) {
        doSvrNotify(stMsg.servernotifyrespond());
        return;
    }

	//领取体力成功
	if(CS_CMD_PLAYER_GETMANUALACTAWARD == stMsg.msg_head().msg_id()){  
		P_DAILYACT_CTRL->doGetManualRsp();
		return;
	}

	if(stMsg.has_actrespond()){
		CSActRespond& stActRsp = *(stMsg.mutable_actrespond());
		if(stActRsp.has_actquery_rsp()){
			P_DAILYACT_CTRL->doDailyActQueryRsp(stActRsp.actquery_rsp());
		}else if(stActRsp.has_actstat_notify()){
			P_DAILYACT_CTRL->doDailyActNotify(stActRsp.actstat_notify());
		}
	}

    GuardActScene* pScene = P_DAILYACT_CTRL->getGuardActScene();
    if(NULL == pScene) {
        return;
    }

    switch(stMsg.msg_head().msg_id()) {
    case CS_CMD_CVZ_ROOMOK:
        pScene->setRoomReady();
        CCLOG("[trace]DailyActProxy::DoRsp,room is ready");
        return;
    case CS_CMD_CVZ_NOTIFYFAIL:
        pScene->doFail();
        return;
    case CS_CMD_CVZ_NOTIFYOVER:
        pScene->doActOver();
        return;
    }

    if(!(stMsg.has_cvzrespond())) {
        return;
    }
    const CSCVZRespond& rsp = stMsg.cvzrespond();
    if(rsp.has_control_notify()) {
        pScene->doControlNotify(rsp.control_notify());
    } else if(rsp.has_destroybase_notify()) {
        pScene->doDestroyBaseNotify(rsp.destroybase_notify());
    } else if(rsp.has_hitmonster_notify()) {
        pScene->doHitMonsterNotify(rsp.hitmonster_notify());
    } else if(rsp.has_hited_notify()) {
        pScene->doHitedNotify(rsp.hited_notify());
    } else if(rsp.has_monsters_notify()) {
        pScene->doMonstersInfoNotify(rsp.monsters_notify());
    } else if(rsp.has_rankinfo_notify()) {
        pScene->doRankInfoNotify(rsp.rankinfo_notify());
    } else if(rsp.has_nextwaveremaintime()) {
        pScene->doWaveTimeNotify(rsp.nextwaveremaintime());
    } else if(rsp.has_mycontawardcoin_notify()) {
        pScene->doMyContAwardCoinNotify(rsp.mycontawardcoin_notify());
    } else if(rsp.has_totalcont_notify()) {
        pScene->doTotalContNotify(rsp.totalcont_notify());
	}

}



void DailyActProxy::doSvrNotify(const CSServerNotifyRespond& stNotify) {
    const int iNotifyID = stNotify.server_notify().notifyid();
    if(0 == iNotifyID) {
        //表示GM发的通知
        if(stNotify.server_notify().notifystr_size() >= 1) {
            POP_TIP_LAYER_MASTER->ShowNotifyStr(stNotify.server_notify().notifystr(0).c_str(), 15);
        }
        
        return;
    }
    const ServerNotifyCfg* pstCfg = SVR_NOTIFY_CFG_MASTER->getNotify(iNotifyID);
    if(!pstCfg) {
        CCLOG("[warn]DailyActProxy::doSvrNotify, fail to get cfg,notify id %d", iNotifyID);
        return;
    }

	getNotifyStr(stNotify.server_notify());

    //POP_TIP_LAYER_MASTER->ShowNotifyStr(pstCfg->notifycontent().c_str(), 10);
    //POP_TIP_LAYER_MASTER->ShowNotifyStr("aaaaaaaaaaaa", 10);
    if(1 == pstCfg->flagtype()) {
        //日常活动
        bool bIsShowBtn = (pstCfg->startflag() == 1);
        
        //pScene->setActEntryLayoutStat(true);
        
        
    }
}

void DailyActProxy::getNotifyStr(const vmsg::CSServerNotify& stNotify){
	const int iNotifyID = stNotify.notifyid();
	const ServerNotifyCfg* pstCfg = SVR_NOTIFY_CFG_MASTER->getNotify(iNotifyID);
	if(!pstCfg) {
		CCLOG("[warn]DailyActProxy::doSvrNotify, fail to get cfg,notify id %d", iNotifyID);
		return;
	}

	if(stNotify.notifystr_size() <= 0){
		POP_TIP_LAYER_MASTER->ShowNotifyStr(pstCfg->notifycontent().c_str(), 10);
		return;
	}

	string stMark = "{param}";
	string notifyStr = pstCfg->notifycontent();
	for(int i = 0; i < stNotify.notifystr_size(); i++){
		size_t idx = notifyStr.find(stMark, 0);

		CCLOG("notify string === %s", stNotify.notifystr(i).c_str());

		if(idx != string::npos){
			notifyStr.replace(idx, stMark.length(), stNotify.notifystr(i));
		}
	}

	POP_TIP_LAYER_MASTER->ShowNotifyStr(notifyStr.c_str(), 10);
}


