#include "DailyInstanceProxy.h"
#include "QNetMgr.h"
#include "vlib/base/CVSingleton.h"
#include "dailyinstance/DailyInstanceControl.h"

DailyInstanceProxy::DailyInstanceProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DAILY_INSTANCE_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DAILY_INSTANCE_PASS, this);
}

DailyInstanceProxy::~DailyInstanceProxy()
{
}

void DailyInstanceProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stRsp) {
    const unsigned int uCmd  = stRsp.msg_head().msg_id();

    switch(uCmd) {
    case vmsg::CS_CMD_DAILY_INSTANCE_QUERY:
        DAILY_INSTANCE_CTRL->updateQueryRsp(stRsp.warmaprespond().querydailyinstance_rsp());
        break;
    case vmsg::CS_CMD_DAILY_INSTANCE_PASS:
        DAILY_INSTANCE_CTRL->updatePassRsp(stRsp.warmaprespond().passdailyinstance_rsp());
        break;
    }
}


void DailyInstanceProxy::sendQueryRqst() {
    unsigned int uCmd = vmsg::CS_CMD_DAILY_INSTANCE_QUERY;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
    
    P_NET_MGR->SendPkg(stMsg);
}
void DailyInstanceProxy::sendPassRqst(const int iCfgKey, const int iPassLvIdx) {
    //注意iPassLvIdx是下标
    unsigned int uCmd = vmsg::CS_CMD_DAILY_INSTANCE_PASS;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
    stMsg.mutable_warmaprequest()->mutable_passdailyinstance_rqst()->set_key(iCfgKey);
    stMsg.mutable_warmaprequest()->mutable_passdailyinstance_rqst()->set_passlvl(iPassLvIdx);
        
    P_NET_MGR->SendPkg(stMsg);
    CCLOG("[trace]DailyInstanceProxy::sendPassRqst,cfgkey=%d,passLvIdx=%d", iCfgKey, iPassLvIdx);
}




