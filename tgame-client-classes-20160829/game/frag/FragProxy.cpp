#include "FragProxy.h"

#include "FragControl.h"
#include "QNetMgr.h"

FragProxy::FragProxy()
{
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DEBRIS_QUERYBAG, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DEBRIS_COMPOSE, this);
}

FragProxy::~FragProxy()
{
}

void FragProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
	const int iCmd = stRsp.msg_head().msg_id();
    switch (iCmd)
    {
    case vmsg::CS_CMD_DEBRIS_QUERYBAG:
        P_FRAG_CTRL->setDebrisBagInfo(stRsp.debrisrespond().debrisbagquery_rsp());
        break;
    case vmsg::CS_CMD_DEBRIS_COMPOSE:
		P_FRAG_CTRL->setDebrisBagInfo(stRsp.debrisrespond().debriscompose_rsp().debrisbaginfo(), true);
        break;
    default:
        break;
    }
}

void FragProxy::fault(const vmsg::CSCommonErrMsg &err) {
    if(P_FRAG_CTRL->getFragPanel()->isOpen()) {
        P_FRAG_CTRL->getFragPanel()->closeLoading();
    }
}


void FragProxy::sendQueryDebrisBagRqst() {
    unsigned int uCmd = vmsg::CS_CMD_DEBRIS_QUERYBAG;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
    
    P_NET_MGR->SendPkg(stMsg);
}
void FragProxy::sendDebrisComposeRqst(const int iCfgID) {
    unsigned int uCmd = vmsg::CS_CMD_DEBRIS_COMPOSE;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    vmsg::CSDebrisRequest *pstDebrisRqst = stMsg.mutable_debrisrequest();
    pstDebrisRqst->mutable_debriscompose_rqst()->set_debriscfgid(iCfgID);

    P_NET_MGR->SendPkg(stMsg);
}



