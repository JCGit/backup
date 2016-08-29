#include "BossWarProxy.h"
#include "QNetMgr.h"
#include "BossWarControl.h"

using namespace vmsg;

CBossWarProxy::CBossWarProxy(){
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BOSS_CHALLENGE_QUERY, this);	
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BOSS_CHALLENGE_DAMAGE_REPORT, this);		
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BOSS_CHALLENGE_DAMAGE_NOTIFY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BOSS_CHALLANGE_STATE_NOTIFY, this);

	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BOSS_CHALLENGE_DELEGATE, this);		
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BOSS_CHALLENGE_ATTEND_OR_LEAVE, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BOSS_CHALLENGE_RELIVE, this);
}

CBossWarProxy::~CBossWarProxy(){

}

void CBossWarProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg){
	P_BOSS_WAR_CTRL->onRecvMsg(stMsg);
}

void CBossWarProxy::sendQueryBossChallengeRqst(){
	unsigned int uCmd = CS_CMD_BOSS_CHALLENGE_QUERY;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CBossWarProxy::sendDamageReportRqst(vmsg::CSBossChallengeBigDamageReport& stRqst){
	unsigned int uCmd = CS_CMD_BOSS_CHALLENGE_DAMAGE_REPORT;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	CSBossChallengeRqst* pcvzRqst = stMsg.mutable_bosschallengerqst();
	*(pcvzRqst->mutable_damagereport()) = stRqst;

	P_NET_MGR->SendPkg(stMsg);
}

void CBossWarProxy::sendAttendOrLeaveRqst(unsigned int uLeave){
	unsigned int uCmd = CS_CMD_BOSS_CHALLENGE_ATTEND_OR_LEAVE;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	CSBossChallengeRqst* pcvzRqst = stMsg.mutable_bosschallengerqst();
	pcvzRqst->set_attendorleave(uLeave);

	P_NET_MGR->SendPkg(stMsg);
}

void CBossWarProxy::sendDelegateRqst(unsigned int uDelegate){
	unsigned int uCmd = CS_CMD_BOSS_CHALLENGE_DELEGATE;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	CSBossChallengeRqst* pcvzRqst = stMsg.mutable_bosschallengerqst();
	pcvzRqst->set_delegatetype(uDelegate);

	P_NET_MGR->SendPkg(stMsg);
}

void CBossWarProxy::sendReliveRqst(){
	unsigned int uCmd = CS_CMD_BOSS_CHALLENGE_RELIVE;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}
