#include "PlayerProxy.h"

#include "QNetMgr.h"

#include "PlayerControl.h"

PlayerProxy::PlayerProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_GETMONTHCARDAWARD, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_PUSHMONTHCARD, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_VIP_DAILY_AWARD, this);
}

PlayerProxy::~PlayerProxy()
{
}

void PlayerProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_PLAYER_CTRL->onRecvMsg(stRsp);
}

void PlayerProxy::sendGetMonthCardAwardRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_GETMONTHCARDAWARD;

	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSPlayerRequest *pPlayerRqst = stMsg.mutable_playerrequest();

	P_NET_MGR->SendPkg(stMsg);
}

void PlayerProxy::sendGetVIPDailyAwardRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_VIP_DAILY_AWARD;

	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSPlayerRequest *pPlayerRqst = stMsg.mutable_playerrequest();

	P_NET_MGR->SendPkg(stMsg);
}
