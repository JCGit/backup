#include "GuildProxy.h" 

#include "ClientSvrMsg.pb.h"
#include "CSGuildMsg.pb.h"
#include "QNetMgr.h"
#include "GuildControl.h"
#include "GuildListPanel.h"

using namespace vmsg;
GuildProxy::GuildProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_QUERY_GUILDLST, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_SEARCH, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_CREATE, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_QUERY_GUILD, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_JOIN, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_REPLY_JOIN, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_QUERY_MYREQ, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_ALL_GUILD_REQ, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_DELETE_MYGUILDREQ, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_CHANGE_NOTICE, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_CHG_PRIVI, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_ASSIGN_CHAIRMAN, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_KICK, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_IMPEACH, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_LEAVE, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_DISMISS, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GUILD_STAT_NOTIFY, this);
}

GuildProxy::~GuildProxy()
{
}

void GuildProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_GUILD_CTRL->onRecvMsg(stRsp);
}

void GuildProxy::fault(const vmsg::CSCommonErrMsg &err) {
    if(vmsg::CS_CMD_GUILD_QUERY_GUILD == err.rqstcmdid() 
        && 200016 == err.errorcode()) {
        //还没有加入部落
        P_GUILD_CTRL->getGuildListPanel()->open();
    }
}

void GuildProxy::sendQueryGuildListRqst()
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_QUERY_GUILDLST;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildSearchRqst(const std::string& strName)
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_SEARCH;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSGuildRequest *pstGuildRqst = stMsg.mutable_guildrequest();
	pstGuildRqst->mutable_guildsearch_rqst()->set_searchname(strName);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildCreateRqst(const std::string& strName)
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_CREATE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSGuildRequest *pstGuildRqst = stMsg.mutable_guildrequest();
	pstGuildRqst->mutable_guildcreate_rqst()->set_guildname(strName);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildQueryRqst()
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_QUERY_GUILD;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendQueryOtherGuildRqst(const UNIT64_t uGuildGUID) {
	unsigned int uCmd = vmsg::CS_CMD_GUILD_QUERY_GUILD;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	stMsg.mutable_guildrequest()->set_queryotherguild_rqst(uGuildGUID);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildJoinRqst(const UNIT64_t uGuildGUID)
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_JOIN;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSGuildRequest *pstGuildRqst = stMsg.mutable_guildrequest();
	pstGuildRqst->set_guildjoin_rqst(uGuildGUID);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildJoinReplyRqst(const vector<UIN_t> &stPlayerLst, bool isAgree)
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_REPLY_JOIN;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    CSGuildRequest *pReplyRqst = stMsg.mutable_guildrequest();
    
	for(unsigned int i = 0; i < stPlayerLst.size(); i++) {
		pReplyRqst->mutable_guildjoinreply_rqst()->add_joinuinlst(stPlayerLst[i]);
	}

    int iReply = isAgree ? vmsg::GUILD_REPLY_JOIN_AGREE : vmsg::GUILD_REPLY_JOIN_DENY;
    pReplyRqst->mutable_guildjoinreply_rqst()->set_reply(iReply);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildQueryMyJoinRqst()
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_QUERY_MYREQ;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildQueryJoinListRqst()
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_ALL_GUILD_REQ;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildDelMyJoinRqst(const UNIT64_t uGuildGUID)
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_DELETE_MYGUILDREQ;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSGuildRequest *pstGuildRqst = stMsg.mutable_guildrequest();
	pstGuildRqst->set_deletemyguildreq_rqst(uGuildGUID);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildChgNoticeRqst(const std::string& strNocice)
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_CHANGE_NOTICE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSGuildRequest *pstGuildRqst = stMsg.mutable_guildrequest();
	pstGuildRqst->mutable_chgguildnotice_rqst()->set_notice(strNocice);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildChgPriviRqst(const UIN_t uUin, const int nNewPrive)
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_CHG_PRIVI;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSGuildRequest *pstGuildRqst = stMsg.mutable_guildrequest();
	pstGuildRqst->mutable_guildchgprivi_rqst()->set_chguin(uUin);
	pstGuildRqst->mutable_guildchgprivi_rqst()->set_newprivi(nNewPrive);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildAssignChairmanRqst(const UIN_t uUin)
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_ASSIGN_CHAIRMAN;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSGuildRequest *pstGuildRqst = stMsg.mutable_guildrequest();
	pstGuildRqst->set_assignchairman_rqst(uUin);

	P_NET_MGR->SendPkg(stMsg);

}

void GuildProxy::sendGuildKickRqst(const UIN_t uUin)
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_KICK;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSGuildRequest *pstGuildRqst = stMsg.mutable_guildrequest();
	pstGuildRqst->set_guildkick_rqst(uUin);

	P_NET_MGR->SendPkg(stMsg);

}

void GuildProxy::sendGuildImpeachRqst()
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_IMPEACH;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildLeaveRqst()
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_LEAVE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void GuildProxy::sendGuildDismissRqst()
{
	unsigned int uCmd = vmsg::CS_CMD_GUILD_DISMISS;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}
