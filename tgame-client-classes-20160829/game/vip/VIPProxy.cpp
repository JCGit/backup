#include "VIPProxy.h"

#include "VIPControl.h"
#include "QNetMgr.h"
#include "CSPlayerMsg.pb.h"

VIPProxy::VIPProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUERYVIPINFOS, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_VIP_AWARD, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_VIP_NOTIFY, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUERYBUYCOIN, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_BUYCOIN, this);
}

VIPProxy::~VIPProxy()
{
}

void VIPProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_VIP_CTRL->onRecvMsg(stRsp);
}

void VIPProxy::sendQueryVIPInfoRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERYVIPINFOS;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void VIPProxy::sendGetVIPAwardRqst(unsigned int uVIPLv) {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_VIP_AWARD;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSPlayerRequest *pstPlayerRqst = stMsg.mutable_playerrequest();
	pstPlayerRqst->set_getvipaward_rqst(uVIPLv);

	P_NET_MGR->SendPkg(stMsg);
}


void VIPProxy::sendQueryBuyCoinRqst() {
    unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERYBUYCOIN;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    P_NET_MGR->SendPkg(stMsg);
}


void VIPProxy::sendBuyCoinRqst() {
    unsigned int uCmd = vmsg::CS_CMD_PLAYER_BUYCOIN;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    P_NET_MGR->SendPkg(stMsg);
}


