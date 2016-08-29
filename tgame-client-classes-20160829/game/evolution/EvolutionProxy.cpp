#include "EvolutionProxy.h"

#include "EvolutionControl.h"
#include "basic/QNetMgr.h"
#include "CSPlayerMsg.pb.h"

EvolutionProxy::EvolutionProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_EVOLUTION_SKILL, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_CHANGE_EVOLUTION_SKILL, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUERY_EVOLUTION_INFO, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUERY_EVOLUTIONSKILL_INFO, this);
}

EvolutionProxy::~EvolutionProxy()
{
}

void EvolutionProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_EVOLUTION_CTRL->onRecvMsg(stRsp);
}

void EvolutionProxy::sendEvolutionSkillRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_EVOLUTION_SKILL;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void EvolutionProxy::sendChangeEvolutionSkillRqst(unsigned int uSkillID) {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_CHANGE_EVOLUTION_SKILL;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSPlayerRequest *pstPlayerRqst = stMsg.mutable_playerrequest();
	pstPlayerRqst->mutable_changeevolutionskill_rqst()->set_useskillidx(uSkillID);

	P_NET_MGR->SendPkg(stMsg);
}

void EvolutionProxy::sendQueryEvolutionInfoRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERY_EVOLUTION_INFO;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void EvolutionProxy::sendQueryEvolutionSkillInfoRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERY_EVOLUTIONSKILL_INFO;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}
