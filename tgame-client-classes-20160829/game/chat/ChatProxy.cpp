#include "ChatProxy.h"

#include "QNetMgr.h"
#include "ChatControl.h"

ChatProxy::ChatProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CHAT_SENDMSG, this);
}

ChatProxy::~ChatProxy()
{
}

void ChatProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_CHAT_CTRL->onRecvMsg(stRsp);
}

void ChatProxy::sendChatRqst(const unsigned int uChannelType, const string &msg) {
	unsigned int uCmd = vmsg::CS_CMD_CHAT_SENDMSG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSChatRequest *pstChatRqst = stMsg.mutable_chatrequest();
	vmsg::CSChatSendMsgRqst *pstSendMsgRqst = pstChatRqst->mutable_sendmsg_rqst();
	pstSendMsgRqst->set_channeltype(uChannelType);
	pstSendMsgRqst->set_msgcontent(msg);

	P_NET_MGR->SendPkg(stMsg);
}

void ChatProxy::sendSecretChatRqst(const UIN_t uin, const string &msg) {
	unsigned int uCmd = vmsg::CS_CMD_CHAT_SENDMSG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSChatRequest *pstChatRqst = stMsg.mutable_chatrequest();
	vmsg::CSChatSendMsgRqst *pstSendMsgRqst = pstChatRqst->mutable_sendmsg_rqst();
	pstSendMsgRqst->set_channeltype(vmsg::CHAT_CHANNEL_SECRET);
	pstSendMsgRqst->set_receiveuin(uin);
	pstSendMsgRqst->set_msgcontent(msg);

	P_NET_MGR->SendPkg(stMsg);
}
