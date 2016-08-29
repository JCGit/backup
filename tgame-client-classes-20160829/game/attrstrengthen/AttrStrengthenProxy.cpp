#include "AttrStrengthenProxy.h"

#include "AttrStrengthenControl.h"
#include "CSPlayerMsg.pb.h"
#include "QNetMgr.h"

AttrStrengthenProxy::AttrStrengthenProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUERY_ATTRSTRENGTHEN_INFO, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_ATTRSTRENGTHEN, this);
}

AttrStrengthenProxy::~AttrStrengthenProxy()
{
}

void AttrStrengthenProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_ATTR_STRENGTHEN_CTRL->onRecvMsg(stRsp);
}

void AttrStrengthenProxy::sendQueryAttrStrengthenInfoRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERY_ATTRSTRENGTHEN_INFO;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void AttrStrengthenProxy::sendAttrStrengthenRqst(unsigned int uType, unsigned int uMagicType) {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_ATTRSTRENGTHEN;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSAttrStrengthenRqst *pstAttrStrengthenRqst = stMsg.mutable_playerrequest()->mutable_attrstrengthen_rqst();
	pstAttrStrengthenRqst->set_type(uType);
	pstAttrStrengthenRqst->set_magictype(uMagicType);

	P_NET_MGR->SendPkg(stMsg);
}
