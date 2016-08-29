#include "LotteryProxy.h"

#include "basic/QNetMgr.h"

#include "LotteryControl.h"

LotteryProxy::LotteryProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_LOT_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_LOT, this);
}

LotteryProxy::~LotteryProxy()
{
}

void LotteryProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_LOTTERY_CTRL->onRecvMsg(stRsp);
}

void LotteryProxy::sendQueryLotteryRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_LOT_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void LotteryProxy::sendLotteryRqst(unsigned int uLotteryType) {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_LOT;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSPlayerRequest *pPlayerRqst = stMsg.mutable_playerrequest();
	vmsg::CSWeaponLotRqst *pLotteryRqst = pPlayerRqst->mutable_weaponlot_rqst();
	pLotteryRqst->set_type(uLotteryType);

	P_NET_MGR->SendPkg(stMsg);
}
