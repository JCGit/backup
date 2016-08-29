#include "BagProxy.h"

#include "BagControl.h"
#include "CSBagMsg.pb.h"
#include "QNetMgr.h"
#include "CSPlayerMsg.pb.h"

BagProxy::BagProxy()
{
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BAG_QUERY, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BAG_ENLARGE, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BAG_WEAR_EQUIP, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BAG_TAKEOFF_EQUIP, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BAG_SELL, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BAG_STAT_NOTIFY, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_CHANGE_ARMOR, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_OPEN_PACKAGE, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_PUSH_BAG_INFO, this);

	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BAG_BUY_ITEM, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_BAG_CONSUME_ITEM, this);
}

BagProxy::~BagProxy()
{
}

void BagProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg) {
	P_BAG_CTRL->onRecvMsg(stMsg);
}


void BagProxy::sendQueryBagInfoRqst() {
	unsigned int uCmd = vmsg::CS_CMD_BAG_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	stMsg.mutable_bagrqst()->mutable_querybaginfo_rqst();

	P_NET_MGR->SendPkg(stMsg);
}

void BagProxy::sendEnlargeBagRqst(int nEnlargeCnt) {
    unsigned int uCmd = vmsg::CS_CMD_BAG_ENLARGE;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSBagRequest *pstBagRqst = stMsg.mutable_bagrqst();
	vmsg::CSEnlargeRqst *pstBagEnlargeRqst = pstBagRqst->mutable_enlarge_rqst();
    pstBagEnlargeRqst->set_enlargecnt(nEnlargeCnt);

    P_NET_MGR->SendPkg(stMsg);
}

void BagProxy::sendTakeOnEquipRqst(int nCellIdx) {
	unsigned int uCmd = vmsg::CS_CMD_BAG_WEAR_EQUIP;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSBagRequest *pstBagRqst = stMsg.mutable_bagrqst();
	vmsg::CSWearEquipRqst *pstWearEquipRqst = pstBagRqst->mutable_wearequip_rqst();
	pstWearEquipRqst->set_cellidx(nCellIdx);

	P_NET_MGR->SendPkg(stMsg);
}

void BagProxy::sendTakeOffEquipRqst(UINT64_t uEquipGUID) {
	unsigned int uCmd = vmsg::CS_CMD_BAG_TAKEOFF_EQUIP;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSBagRequest *pstBagRqst = stMsg.mutable_bagrqst();
	vmsg::CSTakeOffEquipRqst *pstTakeOffEquipRqst = pstBagRqst->mutable_takeoffequip_rqst();
	pstTakeOffEquipRqst->set_equipguid(uEquipGUID);

	P_NET_MGR->SendPkg(stMsg);
}

void BagProxy::sendBagSellRqst(int nBagIdx) {
	unsigned int uCmd = vmsg::CS_CMD_BAG_SELL;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSBagRequest *pstBagRqst = stMsg.mutable_bagrqst();
	vmsg::CSBagSellRqst *pstBagSellRqst = pstBagRqst->mutable_bagsell_rqst();
	pstBagSellRqst->set_bagidx(nBagIdx);

	P_NET_MGR->SendPkg(stMsg);
}

void BagProxy::sendChangeArmorRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_CHANGE_ARMOR;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}


void BagProxy::sendOpenPackageRqst(const int iPkgIdx, const int iPkgCfgID) {
    unsigned int uCmd = vmsg::CS_CMD_OPEN_PACKAGE;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    vmsg::CSItemRequest *pstBagRqst = stMsg.mutable_itemrequest();
    vmsg::CSOpenPackageRqst *pstRqst = pstBagRqst->mutable_openpackage_rqst();
    pstRqst->set_packagecfgid(iPkgCfgID);
    pstRqst->set_packageidx(iPkgIdx);

    P_NET_MGR->SendPkg(stMsg);
}

void BagProxy::sendBuyItemRqst(unsigned int uItemType, unsigned int uItemCfgID, unsigned int uCnt){
	unsigned int uCmd = vmsg::CS_CMD_BAG_BUY_ITEM;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSBagRequest *pstBagRqst = stMsg.mutable_bagrqst();
	vmsg::CSBuyItemRqst *pstRqst = pstBagRqst->mutable_buyitem_rqst();
	pstRqst->set_itemtype(uItemType);
	pstRqst->set_itemcfgid(uItemCfgID);
	pstRqst->set_cnt(uCnt);

	P_NET_MGR->SendPkg(stMsg);
}

void BagProxy::sendConsumeItemRqst(unsigned int uItemType, unsigned int uItemID, unsigned int uBagIdx, unsigned int uCnt /* = 1 */){
	unsigned int uCmd = vmsg::CS_CMD_BAG_CONSUME_ITEM;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSBagRequest *pstBagRqst = stMsg.mutable_bagrqst();
	vmsg::CSConsumeItemRqst *pstRqst = pstBagRqst->mutable_consumeitem_rqst();
	pstRqst->set_type(uItemType);
	pstRqst->set_cfgid(uItemID);
	pstRqst->set_bagidx(uBagIdx);
	pstRqst->set_cnt(uCnt);

	P_NET_MGR->SendPkg(stMsg);
}


