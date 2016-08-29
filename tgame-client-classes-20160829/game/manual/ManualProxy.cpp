#include "ManualProxy.h"

#include "ManualControl.h"
#include "basic/QNetMgr.h"
#include "UserObj.h"

ManualProxy::ManualProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUERY_MANUAL, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_MANUAL_BUY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_MANUAL_QUERYBUY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_MANUAL_NOTIFY, this);
}

ManualProxy::~ManualProxy()
{
}

void ManualProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_MANUAL_CTRL->onRecvMsg(stRsp);
}

void ManualProxy::sendQueryManualRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERY_MANUAL;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void ManualProxy::sendBuyManualRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_MANUAL_BUY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void ManualProxy::sendQueryBuyManualRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_MANUAL_QUERYBUY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}
