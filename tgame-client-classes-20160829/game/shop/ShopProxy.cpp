#include "ShopProxy.h"

#include "ShopControl.h"
#include "QNetMgr.h"

ShopProxy::ShopProxy()
{
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_STORE_BUY, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUERY_BLACKMARKET, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_BUY_BLACKMARKET, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_RESET_BLACKMARKET, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PANIC_BUYING, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PANIC_BUYING_QUERY, this);
}

ShopProxy::~ShopProxy()
{
}

void ShopProxy::fault(const vmsg::CSCommonErrMsg &err) {
    P_SHOP_CTRL->setRMBBuyLoading(false);
}

void ShopProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	P_SHOP_CTRL->onRecvMsg(stRsp);
}

void ShopProxy::sendStoreBuyRqst(unsigned int uType, unsigned int uCfgID, unsigned int uCnt) {
	unsigned int uCmd = vmsg::CS_CMD_STORE_BUY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSStoreRequest *pstStoreRqst = stMsg.mutable_storerequest();
	vmsg::CSStoreBuyRqst *pstStoreBuyRqst = pstStoreRqst->mutable_storebuy_rqst();
	pstStoreBuyRqst->set_type(uType);
	pstStoreBuyRqst->set_cfgid(uCfgID);
	pstStoreBuyRqst->set_cnt(uCnt);

	P_NET_MGR->SendPkg(stMsg);
}



void ShopProxy::sendBlackShopQueryRqst() {
    unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERY_BLACKMARKET;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    P_NET_MGR->SendPkg(stMsg);
}
void ShopProxy::sendBlackShopResetRqst() {
    unsigned int uCmd = vmsg::CS_CMD_PLAYER_RESET_BLACKMARKET;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    P_NET_MGR->SendPkg(stMsg);
}

void ShopProxy::sendBlackShopBuyRqst(const int iItemIndex) {
    unsigned int uCmd = vmsg::CS_CMD_PLAYER_BUY_BLACKMARKET;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    vmsg::CSPlayerRequest *pstRqst = stMsg.mutable_playerrequest();
    pstRqst->set_buyblackmarket_rqst(iItemIndex);

    P_NET_MGR->SendPkg(stMsg);
}

void ShopProxy::sendRestrictShopQueryRqst(){
	unsigned int uCmd = vmsg::CS_CMD_PANIC_BUYING_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void ShopProxy::sendRestrictShopBuyRqst(const int iItemIndex){
	unsigned int uCmd = vmsg::CS_CMD_PANIC_BUYING;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSPanicBuyingStoreRequest *pstRqst = stMsg.mutable_panicbuyingrequst();
	pstRqst->mutable_buying_request()->set_idx(iItemIndex);

	P_NET_MGR->SendPkg(stMsg);
}


