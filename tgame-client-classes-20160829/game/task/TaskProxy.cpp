#include "TaskProxy.h"

#include "TaskControl.h"
#include "CSTaskMsg.pb.h"
#include "QNetMgr.h"
#include "UserObj.h"

TaskProxy::TaskProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_TALKWITHNPC, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_TASK_RECEIVE, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_TASK_COMMIT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_TASK_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_TASK_NEWSTAT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DAILYTASK_COMMIT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DAILYTASK_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DAILYTASK_NEWSTAT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DAILYTASK_REFLASH, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_MILESTONE_COMMIT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_MILESTONE_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_MILESTONE_NEWSTAT, this);
}

TaskProxy::~TaskProxy()
{
}


void TaskProxy::fault(const vmsg::CSCommonErrMsg &err) {
    if(P_TASK_CTRL->getTaskPanel()) {
        P_TASK_CTRL->getTaskPanel()->closeLoading();
    }
}
void TaskProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_TASK_CTRL->onRecvMsg(stRsp);
}

void TaskProxy::sendTalkWithNPC(unsigned int uNPCID) {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_TALKWITHNPC;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSPlayerRequest *pstPlayerRqst = stMsg.mutable_playerrequest();
	pstPlayerRqst->set_talkwithnpc_rqst(uNPCID);

	P_NET_MGR->SendPkg(stMsg);
}

void TaskProxy::sendRecvTaskRqst() {
	unsigned int uCmd = vmsg::CS_CMD_TASK_RECEIVE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void TaskProxy::sendCommitTaskRqst() {
	unsigned int uCmd = vmsg::CS_CMD_TASK_COMMIT;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void TaskProxy::sendQueryTaskRqst() {
	unsigned int uCmd = vmsg::CS_CMD_TASK_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void TaskProxy::sendCommitDailyTaskRqst(unsigned int uTaskID) {
	unsigned int uCmd = vmsg::CS_CMD_DAILYTASK_COMMIT;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSTaskRequest *pstTaskRqst = stMsg.mutable_taskrequest();
	pstTaskRqst->set_commitdailytask_rqst(uTaskID);

	P_NET_MGR->SendPkg(stMsg);
}

void TaskProxy::sendQueryDailyTaskRqst() {
	unsigned int uCmd = vmsg::CS_CMD_DAILYTASK_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void TaskProxy::sendRefreshDailyTaskRqst() {
	unsigned int uCmd = vmsg::CS_CMD_DAILYTASK_REFLASH;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void TaskProxy::sendCommitMilestoneRqst(unsigned int uTaskID) {
	unsigned int uCmd = vmsg::CS_CMD_MILESTONE_COMMIT;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSTaskRequest *pstTaskRqst = stMsg.mutable_taskrequest();
	pstTaskRqst->set_commitmilestone_rqst(uTaskID);

	P_NET_MGR->SendPkg(stMsg);
}

void TaskProxy::sendQueryMilestoneRqst() {
	unsigned int uCmd = vmsg::CS_CMD_MILESTONE_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void TaskProxy::sendTriggerDailyTaskRqst(unsigned int u_Cmd)
{
	unsigned int uCmd = u_Cmd;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSTaskRequest *pstTaskRqst = stMsg.mutable_taskrequest();
	pstTaskRqst->set_commitdailytask_rqst(vmsg::DAILY_TYPE_SHARE_CNT);

	P_NET_MGR->SendPkg(stMsg);
}