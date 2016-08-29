#include "GenStrengthenProxy.h"
#include "QNetMgr.h"
#include "GenStrengthenControl.h"

CGenStrengthenProxy::CGenStrengthenProxy(){
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_ENFORCE_MYSELF_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_CONFIRM_ENFORCE_MYSELF, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_ENHANCE_MYSELF, this);
}

CGenStrengthenProxy::~CGenStrengthenProxy(){

}

void CGenStrengthenProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg){
	if(stMsg.has_playerrespond()){
		vmsg::CSPlayerRespond& stRsp = *(stMsg.mutable_playerrespond());

		if(stRsp.has_playerenforcequery_rsp()){
			P_GEN_CTRL->doQueryRsp(stRsp.playerenforcequery_rsp());
		}else if(stRsp.has_playerenforce_rsp()){
			P_GEN_CTRL->doGenStrengthenRsp(stRsp.playerenforce_rsp());
		}else if(stRsp.has_playerenforceconfirm_rsp()){
			P_GEN_CTRL->doConfirmRsp(stRsp.playerenforceconfirm_rsp());
		}
	}
}

void CGenStrengthenProxy::sendQueryGenStrengthenRqst(){
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_ENFORCE_MYSELF_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void CGenStrengthenProxy::sendGenStrengthenRqst(unsigned int uType){
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_ENHANCE_MYSELF;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	stMsg.mutable_playerrequest()->mutable_playerenforce_rqst()->set_enforcetype(uType);

	P_NET_MGR->SendPkg(stMsg);
}

void CGenStrengthenProxy::sendGenStrengthenConfirmRqst(unsigned int uIsAccept){
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_CONFIRM_ENFORCE_MYSELF;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	stMsg.mutable_playerrequest()->mutable_playerenforceconfirm_rqst()->set_isaccept(uIsAccept);

	P_NET_MGR->SendPkg(stMsg);
}

