#include "ArenaProxy.h"
#include "QNetMgr.h"
#include "ArenaControl.h"

using namespace vmsg;

CArenaProxy::CArenaProxy(){
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_QUERY_RECORD, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_FIND_OPPONENT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_GET_AWARD, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_BUY_FIGHT_NUM, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_START_FIGHT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_END_FIGHT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_STORE_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_STORE_BUY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_STORE_REFRESH, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_OPPONENT_INFO, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_RANK_QUERY_ARENAARMYRANK, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ARENA_CLEANCD, this);
}

CArenaProxy::~CArenaProxy(){

}

void CArenaProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg){
	if(stMsg.has_errmsg()){

	}else if(stMsg.msg_head().msg_id() == CS_CMD_ARENA_CLEANCD){  //Çå³ýcd»Ø±¨
		P_ARENA_CTRL->doCleanCDRsp();
	}else if(stMsg.has_arenarespond()) {
		CSArenaRespond& stArenaRsp = *(stMsg.mutable_arenarespond());

		if(stArenaRsp.has_arenaquery_rsp()){
			P_ARENA_CTRL->doArenaQueryRsp(stArenaRsp.arenaquery_rsp());
		}else if(stArenaRsp.has_record_rsp()){
			P_ARENA_CTRL->doRecordRsp(stArenaRsp.record_rsp());
		}else if(stArenaRsp.has_findopponent_rsp()){
			P_ARENA_CTRL->doFindOpponentRsp(stArenaRsp.findopponent_rsp());
		}else if(stArenaRsp.has_getaward_rsp()){
			P_ARENA_CTRL->doGetAwardRsp();
		}else if(stArenaRsp.has_buynum_rsp()){
			P_ARENA_CTRL->doBuyFightNumRsp(stArenaRsp.buynum_rsp());
		}else if(stArenaRsp.has_storequery_rsp()){
			P_ARENA_CTRL->doStoreInfoRsp(stArenaRsp.storequery_rsp());
		}else if(stArenaRsp.has_storerefresh_rsp()){
			P_ARENA_CTRL->doStoreInfoRsp(stArenaRsp.storerefresh_rsp());
		}else if(stArenaRsp.has_storebuy_rsp()){
			P_ARENA_CTRL->doStoreInfoRsp(stArenaRsp.storebuy_rsp());
		}else if(stArenaRsp.has_startfight_rsp()){
			P_ARENA_CTRL->doArenaFightStartRsp(stArenaRsp.startfight_rsp());
		}else if(stArenaRsp.has_endfight_rsp()){
			P_ARENA_CTRL->doArenaFightOverRsp(stArenaRsp.endfight_rsp());
		}
	}else if(stMsg.has_rankrespond()){
		vmsg::CSRankRespond& stRankRsp = *(stMsg.mutable_rankrespond());

		if(stRankRsp.has_rankqueryarenaarmyrank_rsp()){
			P_ARENA_CTRL->doArenaRankRsp(stRankRsp.rankqueryarenaarmyrank_rsp());
		}
	}
}

void CArenaProxy::sendArenaQueryRqst(){
	unsigned int uCmd = CS_CMD_ARENA_QUERY;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendArenaQueryRecordRqst(){
	unsigned int uCmd = CS_CMD_ARENA_QUERY_RECORD;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendArenaFindOpponentRqst(){
	unsigned int uCmd = CS_CMD_ARENA_FIND_OPPONENT;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendArenaGetAwardRqst(){
	unsigned int uCmd = CS_CMD_ARENA_GET_AWARD;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendArenaBuyFightNumRqst(){
	unsigned int uCmd = CS_CMD_ARENA_BUY_FIGHT_NUM;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendArenaStartFightRqst(const UIN_t uin){
	unsigned int uCmd = CS_CMD_ARENA_START_FIGHT;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	CSArenaRequest *pstRqst = stMsg.mutable_arenarequest();
	pstRqst->set_startfightuin_rqst(uin);

	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendArenaEndFightRqst(const UIN_t uin, unsigned int isMyWin){
	unsigned int uCmd = CS_CMD_ARENA_END_FIGHT;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	CSArenaRequest *pstRqst = stMsg.mutable_arenarequest();
	pstRqst->mutable_endfightuin_rqst()->set_opponentuin(uin);
	pstRqst->mutable_endfightuin_rqst()->set_ismywin(isMyWin);

    const int iRecordTime = P_ARENA_CTRL->getArenaObj()->getFightBackInfo();
    pstRqst->mutable_endfightuin_rqst()->set_isfightback(iRecordTime > 0);
    pstRqst->mutable_endfightuin_rqst()->set_recordtime(iRecordTime);
   
	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendStoreQueryRqst(){
	unsigned int uCmd = CS_CMD_ARENA_STORE_QUERY;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendStoreBuyRqst(unsigned int itemIdx, unsigned int itemCfgID){
	unsigned int uCmd = CS_CMD_ARENA_STORE_BUY;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	CSArenaRequest *pstRqst = stMsg.mutable_arenarequest();
	pstRqst->mutable_arenastorebuy_rqst()->set_itemidx(itemIdx);
	pstRqst->mutable_arenastorebuy_rqst()->set_itemcfgid(itemCfgID);

	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendStoreRefreshRqst(){
	unsigned int uCmd = CS_CMD_ARENA_STORE_REFRESH;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendQueryArenaArmyRankRqst(unsigned int start, unsigned int stop, unsigned int cnt, const UIN_t uin){
	unsigned int uCmd = CS_CMD_RANK_QUERY_ARENAARMYRANK;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	vmsg::CSRankQueryArenaArmyRankRqst* pstRankRqst = stMsg.mutable_rankrequest()->mutable_rankqueryarenaarmyrank_rqst();
	pstRankRqst->set_start(start);
	pstRankRqst->set_stop(stop);
	pstRankRqst->set_cnt(cnt);
	if(uin != 0){
		pstRankRqst->set_uin(uin);
	}

	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CArenaProxy::sendCleanCDRqst(){
	unsigned int uCmd = CS_CMD_ARENA_CLEANCD;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

