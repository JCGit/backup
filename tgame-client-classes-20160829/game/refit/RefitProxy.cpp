#include "RefitProxy.h"

#include "CSDotaItemMsg.pb.h"

#include "basic/QNetMgr.h"

#include "RefitControl.h"

RefitProxy::RefitProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DOTAITEM_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DOTAITEM_COMPOSE, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DOTAITEM_MOSAIC, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DOTAITEM_LVUP, this);
}

RefitProxy::~RefitProxy()
{
}

void RefitProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_REFIT_CTRL->onRecvMsg(stRsp);
}

void RefitProxy::sendRefitQueryRqst() {
	unsigned int uCmd = vmsg::CS_CMD_DOTAITEM_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void RefitProxy::sendRefitComposeRqst(unsigned int uDotaItemID) {
	unsigned int uCmd = vmsg::CS_CMD_DOTAITEM_COMPOSE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSDotaItemRequest *pDotaItemRqst = stMsg.mutable_dotaitemrequest();
	vmsg::CSDotaItemComposeRqst *pDotaItemComposeRqst = pDotaItemRqst->mutable_dotaitemcompose_rqst();
	pDotaItemComposeRqst->set_dotaitemid(uDotaItemID);

	P_NET_MGR->SendPkg(stMsg);
}

void RefitProxy::sendRefitMosaicRqst(unsigned int uPart, unsigned int uDotaItemID) {
	unsigned int uCmd = vmsg::CS_CMD_DOTAITEM_MOSAIC;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSDotaItemRequest *pDotaItemRqst = stMsg.mutable_dotaitemrequest();
	vmsg::CSDotaItemMosaicRqst *pDotaItemMosaicRqst = pDotaItemRqst->mutable_dotaitemmosaic_rqst();
	pDotaItemMosaicRqst->set_part(uPart);
	pDotaItemMosaicRqst->set_dotaitemid(uDotaItemID);

	P_NET_MGR->SendPkg(stMsg);
}

void RefitProxy::sendRefitLvUpRqst(unsigned int uPart) {
	unsigned int uCmd = vmsg::CS_CMD_DOTAITEM_LVUP;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSDotaItemRequest *pDotaItemRqst = stMsg.mutable_dotaitemrequest();
	vmsg::CSDotaItemLvUpRqst *pDotaItemLvUpRqst = pDotaItemRqst->mutable_dotaitemlvup_rqst();
	pDotaItemLvUpRqst->set_part(uPart);

	P_NET_MGR->SendPkg(stMsg);
}
