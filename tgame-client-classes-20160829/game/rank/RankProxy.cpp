#include "RankProxy.h"
#include "QNetMgr.h"
#include "UserObj.h"
#include "RankControl.h"

using namespace vmsg;

CRankProxy::CRankProxy(){
	P_NET_MGR->RegCmdListener(CS_CMD_RANK_QUERY, this);
	P_NET_MGR->RegCmdListener(CS_CMD_RANK_QUERY_MYRANK, this);
	P_NET_MGR->RegCmdListener(CS_CMD_PLAYER_QUERY_AWARDRANK, this);
	P_NET_MGR->RegCmdListener(CS_CMD_PLAYER_FETCH_RANKAWARD, this);
}

CRankProxy::~CRankProxy(){

}

void CRankProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg){
	if(stMsg.has_rankrespond()){
		vmsg::CSRankRespond& stRankRsp = *(stMsg.mutable_rankrespond());

		if(stRankRsp.has_rankquery_rsp()){
			P_RANK_CTL->doRankQueryRsp(stRankRsp.mutable_rankquery_rsp());
		}else if(stRankRsp.has_rankquerymyrank_rsp()){
			P_RANK_CTL->doRankQueryMyRankRsp(stRankRsp.mutable_rankquerymyrank_rsp());
		}
	}else if(stMsg.has_playerrespond()){
		CSPlayerRespond& stPlayerRsp = *(stMsg.mutable_playerrespond());

		if(stPlayerRsp.has_queryawardrank_rsp()){
			P_RANK_CTL->doAwardRankRsp(stPlayerRsp.mutable_queryawardrank_rsp());
		}else if(stPlayerRsp.has_fetchrankaward_rsp()){
			P_RANK_CTL->doAwardRankRsp(stPlayerRsp.mutable_fetchrankaward_rsp());
		}
	}
}

void CRankProxy::sendRankQueryRqst(unsigned int rankType, int startPos, int endPos){
	unsigned int uCmd = vmsg::CS_CMD_RANK_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

	vmsg::CSRankQueryRqst* pstRankRqst = stMsg.mutable_rankrequest()->mutable_rankquery_rqst();
	pstRankRqst->set_ranktype(rankType);
	pstRankRqst->set_startpos(startPos);
	pstRankRqst->set_stoppos(endPos);

	P_NET_MGR->SendPkg(stMsg);
}

void CRankProxy::sendRankQueryMyRankRqst(unsigned int rankType){
	unsigned int uCmd = vmsg::CS_CMD_RANK_QUERY_MYRANK;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

	vmsg::CSRankQueryMyRankRqst* pstRankRqst = stMsg.mutable_rankrequest()->mutable_rankquerymyrank_rqst();
	pstRankRqst->set_ranktype(rankType);

	P_NET_MGR->SendPkg(stMsg);
}

void CRankProxy::sendQueryAwardRankRqst(){
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERY_AWARDRANK;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

	P_NET_MGR->SendPkg(stMsg);
}

void CRankProxy::sendFetchRankAwardRqst(unsigned int rankType){
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_FETCH_RANKAWARD;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

	vmsg::CSFetchRankAwardRqst* pstFetchRqst = stMsg.mutable_playerrequest()->mutable_fetchrankaward_rqst();
	pstFetchRqst->set_ranktype(rankType);

	P_NET_MGR->SendPkg(stMsg);
}