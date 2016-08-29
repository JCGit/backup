#include "FriendProxy.h"

#include "ClientSvrMsg.pb.h"
#include "QNetMgr.h"
#include "UserObj.h"
#include "UserInfo.h"
#include "FriendControl.h"
#include "fight/LevelControl.h"
#include "CommonDialog.h"
#include "TextHelper.h"

using namespace vmsg;

FriendProxy::FriendProxy()
{
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_ADD_FRIEND, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_DEL_FRIEND, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_ADD_BLACK, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_QUERY, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_GET_PROPOSE, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_ADMIN_ADD, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_ADMIN_REFUSE, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_ADMIN_CLEAR, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_SEND_MANUAL, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_RECV_MANUAL, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_REQ_NOTIFY, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FRIEND_BAT_RECV_MANUAL, this);
}

FriendProxy::~FriendProxy()
{
}

void FriendProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg) {
    if(vmsg::CS_CMD_FRIEND_ADMIN_CLEAR == stMsg.msg_head().msg_id()) {
        P_FRIEND_CTRL->doClearBlacklistRsp();
        return;
    }
	if(stMsg.has_friendrespond()) {
		vmsg::CSFriendRespond& stFriendRsp = *(stMsg.mutable_friendrespond());
        if (stFriendRsp.has_friendquery_rsp() ) {
            P_FRIEND_CTRL->doQueryRsp(stFriendRsp.mutable_friendquery_rsp());

			//更新好友助战
			P_LEVEL_CTL->doQueryFriendLstRsp();
        } else if (stFriendRsp.has_proposefriend_rsp()) {
            P_FRIEND_CTRL->doProposeRsp(stFriendRsp.mutable_proposefriend_rsp());
        } else if(stFriendRsp.has_adminadd_rsp() ) {
            //添加好友
            P_FRIEND_CTRL->doAdminAddRsp(stFriendRsp.mutable_adminadd_rsp());
        } else if(stFriendRsp.has_frienddel_rsp() ) {
            P_FRIEND_CTRL->doDelFriendRsp(stFriendRsp.frienddel_rsp());
        } else if(stFriendRsp.has_addblack_rsp() ) {
            P_FRIEND_CTRL->doAddBlackRsp(stFriendRsp.addblack_rsp());
        } else if(stFriendRsp.has_sendmanual_rsp() ) {
            P_FRIEND_CTRL->doSendManualRsp(stFriendRsp.sendmanual_rsp());
        } else if(stFriendRsp.has_addrequin_rsp() ) {
            //do nothing
            P_FRIEND_CTRL->doSendAddReqRsp(stFriendRsp.addrequin_rsp());
            COMMON_DIALOG->show(TextStr(TEXT_ADD_FRIEND_TIP));
        } else if(stFriendRsp.has_adminrefuseuin() ) {
            P_FRIEND_CTRL->doAdminRefuseUin(stFriendRsp.adminrefuseuin());
        } else if(stFriendRsp.has_batchadd_rsp() ) {
            //do nothing
            P_FRIEND_CTRL->doBatchAddReqRsp(stFriendRsp.batchadd_rsp());
            COMMON_DIALOG->show(TextStr(TEXT_ADD_FRIEND_TIP));
        } else if(stFriendRsp.has_reqnumnotify_rsp()) {
            P_FRIEND_CTRL->doFriendReqNumNotify(stFriendRsp.reqnumnotify_rsp());
        } else if(stFriendRsp.has_recvmanual_rsp()) {
            P_FRIEND_CTRL->doRecvManualRsp(stFriendRsp.recvmanual_rsp());
        }

	}

}

void FriendProxy::fault(const vmsg::CSCommonErrMsg &err) {
    CCLOG("[error]FriendProxy::fault,cmd=%d,error=%d", err.rqstcmdid(), err.errorcode());
}

void FriendProxy::sendQueryFriendRqst()
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_QUERY;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    //CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    P_NET_MGR->SendPkg(stMsg);
}

//发送加好友申请，name格式为：XXX.sX
void FriendProxy::sendAddReqRqst(const std::string& strName)
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_ADD_FRIEND;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    std::string strNick = P_USER_OBJ->genZoneNick(strName);
    pstFriendRqst->set_friendadd_rqst(strNick);

    P_NET_MGR->SendPkg(stMsg);
    CCLOG("%s,send add friend req,zone nick = %s", __FUNCTION__, strNick.c_str());
}

void FriendProxy::sendDelRqst(const UIN_t uUin)
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_DEL_FRIEND;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    pstFriendRqst->set_frienddel_rqst(uUin);

    P_NET_MGR->SendPkg(stMsg);
}


void FriendProxy::sendAddBlackRqst(const UIN_t uUin)
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_ADD_BLACK;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    pstFriendRqst->set_addblack_rqst(uUin);

    P_NET_MGR->SendPkg(stMsg);
}

void FriendProxy::sendAdminAddRqst(const UIN_t uUin)
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_ADMIN_ADD;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    pstFriendRqst->set_adminaddfrienduin(uUin);

    P_NET_MGR->SendPkg(stMsg);
}

void FriendProxy::sendAdminRefuseRqst(const UIN_t uUin)
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_ADMIN_REFUSE;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    pstFriendRqst->set_adminrefuseuin(uUin);

    P_NET_MGR->SendPkg(stMsg);
}

void FriendProxy::sendAdminClearRqst()
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_ADMIN_CLEAR;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    P_NET_MGR->SendPkg(stMsg);
}

void FriendProxy::sendGetProposeRqst()
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_GET_PROPOSE;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    P_NET_MGR->SendPkg(stMsg);
}

void FriendProxy::sendSendManualRqst(const UIN_t uUin)
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_SEND_MANUAL;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    pstFriendRqst->set_sendmanualuin(uUin);

    P_NET_MGR->SendPkg(stMsg);
}

void FriendProxy::sendRecvManualRqst(const UIN_t uUin)
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_RECV_MANUAL;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    pstFriendRqst->set_recvmanualuin(uUin);

    P_NET_MGR->SendPkg(stMsg);
}


void FriendProxy::sendBatchAddRqst(const std::vector<UIN_t>& stUinLst)
{
    if(stUinLst.size() <= 0) {
        return;
    }

    unsigned int uCmd = vmsg::CS_CMD_FRIEND_BATCH_ADD;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    
    CSBatchAddUinLst* pstReq = pstFriendRqst->mutable_batchadd_rqst();
    for (unsigned int i = 0; i < stUinLst.size(); ++i)
    {
        pstReq->add_uinlst(stUinLst[i]);
    }

    P_NET_MGR->SendPkg(stMsg);
}


void FriendProxy::sendBatRecvManualRqst()
{
    unsigned int uCmd = vmsg::CS_CMD_FRIEND_BAT_RECV_MANUAL;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin );

    //CSFriendRequest* pstFriendRqst = stMsg.mutable_friendrequest();
    P_NET_MGR->SendPkg(stMsg);
}


