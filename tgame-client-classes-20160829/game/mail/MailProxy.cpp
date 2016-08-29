#include "MailProxy.h"

#include "ClientSvrMsg.pb.h"
#include "QNetMgr.h"
#include "MailControl.h"

using namespace vmsg;

MailProxy::MailProxy()
{
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_MAIL_QUERY, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_MAIL_READ, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_MAIL_DELETE, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_MAIL_GET_ATTACH, this);	
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_MAIL_PUSH_NOTIFY, this);			//邮件通知推送
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_MAIL_NEW, this);					//新的邮件
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_MAIL_DELALL, this);
}

MailProxy::~MailProxy()
{
}

void MailProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg) 
{
	if(stMsg.has_mailrespond()) {
        const CSMailRespond& stRsp = stMsg.mailrespond();
        if(stRsp.has_querymail_rsp()) {
            P_MAIL_CTRL->getMailObj()->updateMailData(stRsp.querymail_rsp());
            P_MAIL_CTRL->doQueryMailRsp();
        } else if(stRsp.has_readmail_rsp()) {
            P_MAIL_CTRL->getMailObj()->updateMailContent(stRsp.readmail_rsp());
            P_MAIL_CTRL->doQueryMailRsp();
        } else if(stRsp.has_deletemail_rsp()) {
            P_MAIL_CTRL->getMailObj()->delMailID(stRsp.deletemail_rsp().mailid());
            P_MAIL_CTRL->doQueryMailRsp();
        } else if(stRsp.has_getattach_rsp()) {
            for(int i = 0; i < stRsp.getattach_rsp().mailidlst_size(); ++i) {
                P_MAIL_CTRL->getMailObj()->setAttachGeted(stRsp.getattach_rsp().mailidlst(i));
            }
            P_MAIL_CTRL->doQueryMailRsp();
            P_MAIL_CTRL->showGetAttachTip(m_uLastGetAttachMailID);
            m_uLastGetAttachMailID = 0; //用完重置
        } else if(stRsp.has_deleteall_rsp()) {
            P_MAIL_CTRL->getMailObj()->updateMailData(stRsp.deleteall_rsp() );
            P_MAIL_CTRL->doQueryMailRsp();
        }
    }

	if(stMsg.msg_head().msg_id() == vmsg::CS_CMD_MAIL_NEW){		//新的邮件
		sendQueryMail();		//再次查询下邮件信息
	}
}

void MailProxy::sendQueryMail()
{
    unsigned int uCmd = CS_CMD_MAIL_QUERY;
    VCli2LogicSvrMsg stMsg;
    MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
    P_NET_MGR->SendPkg(stMsg);
}

void MailProxy::sendReadMail(const UNIT64_t uMailID)
{
    unsigned int uCmd = CS_CMD_MAIL_READ;
    VCli2LogicSvrMsg stMsg;
    MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    CSMailRequest *pstRqst = stMsg.mutable_mailrequest();
    pstRqst->mutable_readmail_rqst()->set_mailid(uMailID);
    pstRqst->mutable_readmail_rqst()->set_groupid(P_MAIL_CTRL->getMailObj()->getGrpID(uMailID));

    P_NET_MGR->SendPkg(stMsg);
}


void MailProxy::sendDelMail(const UNIT64_t uMailID)
{
    unsigned int uCmd = CS_CMD_MAIL_DELETE;
    VCli2LogicSvrMsg stMsg;
    MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    CSMailRequest *pstRqst = stMsg.mutable_mailrequest();
    pstRqst->mutable_deletemail_rqst()->set_mailid(uMailID);
    pstRqst->mutable_deletemail_rqst()->set_groupid(P_MAIL_CTRL->getMailObj()->getGrpID(uMailID));

    P_NET_MGR->SendPkg(stMsg);
}

void MailProxy::sendGetAttachOne(const UNIT64_t uMailID)
{
    m_uLastGetAttachMailID = uMailID;
    unsigned int uCmd = CS_CMD_MAIL_GET_ATTACH;
    VCli2LogicSvrMsg stMsg;
    MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    CSMailRequest *pstRqst = stMsg.mutable_mailrequest();
    CSOneMail* pstOneMail = pstRqst->mutable_getattach_rqst()->add_maillst();
    pstOneMail->set_mailid(uMailID);
    pstOneMail->set_groupid(P_MAIL_CTRL->getMailObj()->getGrpID(uMailID));

    P_NET_MGR->SendPkg(stMsg);
}

void MailProxy::sendGetAttachAll(const std::vector<UNIT64_t>& stMailLst)
{
    m_uLastGetAttachMailID = 0; //重置
    unsigned int uCmd = CS_CMD_MAIL_GET_ATTACH;
    VCli2LogicSvrMsg stMsg;
    MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    CSMailRequest *pstRqst = stMsg.mutable_mailrequest();
    UNIT64_t uMailID = 0;
    for(unsigned int i = 0; i < stMailLst.size(); ++i) {
        uMailID = stMailLst[i];
        CSOneMail* pstOneMail = pstRqst->mutable_getattach_rqst()->add_maillst();
        pstOneMail->set_mailid(uMailID);
        pstOneMail->set_groupid(P_MAIL_CTRL->getMailObj()->getGrpID(uMailID));
    }

    P_NET_MGR->SendPkg(stMsg);
}



void MailProxy::sendAutoDelRqst()
{
    unsigned int uCmd = CS_CMD_MAIL_DELALL;
    VCli2LogicSvrMsg stMsg;
    MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
    P_NET_MGR->SendPkg(stMsg);
}




