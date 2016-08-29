#include "NihilityProxy.h"
#include "QNetMgr.h"
#include "NihilityControl.h"

using namespace vmsg;

CNihilityProxy::CNihilityProxy(){
	P_NET_MGR->RegCmdListener(CS_CMD_NOTHINGLAND_START, this);
	P_NET_MGR->RegCmdListener(CS_CMD_NOTHINGLAND_END, this);
	P_NET_MGR->RegCmdListener(CS_CMD_INFINIT_GENINFO, this);
	P_NET_MGR->RegCmdListener(CS_CMD_INFINIT_BUYGEN, this);
	P_NET_MGR->RegCmdListener(CS_CMD_NOTHINGLAND_MISSION_REPORT, this);
	P_NET_MGR->RegCmdListener(CS_CMD_NOTHINGLAND_UNFINISHED_QUERY, this);
}

CNihilityProxy::~CNihilityProxy(){

}

void CNihilityProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg){
	if(stMsg.has_errmsg()){
		vmsg::CSCommonErrMsg stErrMsg = stMsg.errmsg();

		if(stErrMsg.rqstcmdid() == CS_CMD_NOTHINGLAND_START) { //进入失败
			P_NIHILITY_CTRL->getNihilityPanel()->closeLoad();
			P_NIHILITY_CTRL->getGenPanel()->closeLoading();
		}
	}else if(stMsg.msg_head().msg_id() == CS_CMD_NOTHINGLAND_START){  //虚无知地战斗开始
		P_NIHILITY_CTRL->doNihilityStartRsp();
	}else if(stMsg.has_infiniterespond()) {
		CSInfiniteRespond* pstRsp = stMsg.mutable_infiniterespond();

		if(pstRsp->has_nothinglandend_rsp()){   //虚无知地战斗结束
			P_NIHILITY_CTRL->doNihilityEndRsp(pstRsp->mutable_nothinglandend_rsp());
		}else if(pstRsp->has_infinitegeninfo_rsp()){  //基因强化查询
			P_NIHILITY_CTRL->doGenInfoRsp(pstRsp->mutable_infinitegeninfo_rsp());
		}else if(pstRsp->has_buygen_rsp()){   //基因强化购买
			P_NIHILITY_CTRL->doBuyGenRsp(pstRsp->mutable_buygen_rsp());
		}else if(pstRsp->has_nothinglandhistory_rsp()){   //查询是否有历史记录，如果有就继续
			P_NIHILITY_CTRL->doNihilityHistoryRsp(pstRsp->nothinglandhistory_rsp());
		}
	}
}

void CNihilityProxy::sendInfiniteGenInfoRqst(){
	unsigned int uCmd = CS_CMD_INFINIT_GENINFO;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CNihilityProxy::sendInfiniteBuyGenRqst(unsigned int idx){
	unsigned int uCmd = CS_CMD_INFINIT_BUYGEN;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	stMsg.mutable_infiniterequest()->mutable_buygen_rqst()->set_idx(idx);
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}


void CNihilityProxy::sendNihilityFightStartRqst(unsigned int isContinue){
	unsigned int uCmd = CS_CMD_NOTHINGLAND_START;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	stMsg.mutable_infiniterequest()->mutable_nothinglandstart_rqst()->set_iscontinue(isContinue);
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CNihilityProxy::sendNihilityFightEndRqst(unsigned int evolutioncnt, unsigned int floorScore, unsigned int coin, unsigned int floor, unsigned int score, CVArray<int, 100> stAwardFloor){
	unsigned int uCmd = CS_CMD_NOTHINGLAND_END;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	CSInfiniteRequest* pstEndRqst = stMsg.mutable_infiniterequest();
	pstEndRqst->mutable_nothinglandend_rqst()->set_evolutioncnt(evolutioncnt);
	pstEndRqst->mutable_nothinglandend_rqst()->set_coin(coin);
	pstEndRqst->mutable_nothinglandend_rqst()->set_floor(floor);
	pstEndRqst->mutable_nothinglandend_rqst()->set_score(score);
	pstEndRqst->mutable_nothinglandend_rqst()->set_floorscore(floorScore);

	for(int i = 0; i < stAwardFloor.Size(); i++){
		pstEndRqst->mutable_nothinglandend_rqst()->add_awardfloor(stAwardFloor[i]);
	}

	P_NET_MGR->SendPkg(stMsg);
}

void CNihilityProxy::sendNihilityMissonReportRqst(unsigned int evolutioncnt, unsigned int floorScore, unsigned int coin, unsigned int floor, unsigned int score, CVArray<int, 100> stAwardFloor){
	unsigned int uCmd = CS_CMD_NOTHINGLAND_MISSION_REPORT;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	CSInfiniteRequest* pstEndRqst = stMsg.mutable_infiniterequest();
	pstEndRqst->mutable_nothinglandmission_report()->set_evolutioncnt(evolutioncnt);
	pstEndRqst->mutable_nothinglandmission_report()->set_coin(coin);
	pstEndRqst->mutable_nothinglandmission_report()->set_floor(floor);
	pstEndRqst->mutable_nothinglandmission_report()->set_score(score);
	pstEndRqst->mutable_nothinglandmission_report()->set_floorscore(floorScore);

	for(int i = 0; i < stAwardFloor.Size(); i++){
		pstEndRqst->mutable_nothinglandmission_report()->add_awardfloor(stAwardFloor[i]);
	}

	P_NET_MGR->SendPkg(stMsg);
}

void CNihilityProxy::sendNihilityUnfinishedQueryRqst(){
	unsigned int uCmd = CS_CMD_NOTHINGLAND_UNFINISHED_QUERY;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

