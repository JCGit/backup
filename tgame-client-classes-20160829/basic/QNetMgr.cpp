#include "QNetMgr.h"
#include "ClientSvrMsg.pb.h"
#include "PlayerLoginMsg.pb.h"
#include "QNetMgr.h"
#include "AppID.pb.h"
#include "SpecAppID.pb.h"
#include "QEnvSet.h"
#include "defines.h"
#include "NounHelper.h"
#include "UserObj.h"
#include "CommonDialog.h"
#include "GameInf.h"
#include "ErrorCodeCfgMaster.h"


using namespace std;


QNetMgr::QNetMgr() {
	m_uSeq = 0;
}

QNetMgr::~QNetMgr() 
{
}

void QNetMgr::PrintMsgHead(const char* prefix, const vmsg::MsgHead& stHead, bool isSend)
{
    if(isSend) {
        CCLOG("[trace]send cmd %d", stHead.msg_id());
    } else {
        CCLOG("[trace]recv cmd %d", stHead.msg_id());
    }
    return;

    const int iBufSize = 100;
    int iStrLen = 0;
    char str[iBufSize] = {0};
    snprintf(str, iBufSize, "--%s-->%s={", (isSend ? "RQST" : "RSP"), prefix);
    iStrLen = strlen(str);
    if (stHead.has_msg_id())
    {
        snprintf(str + iStrLen, iBufSize - iStrLen, " MsgID=%d", stHead.msg_id());
        iStrLen = strlen(str);
    }
    if (stHead.has_uid())
    {
        //CCLOG("Uid=%d", stHead.uid());
        snprintf(str + iStrLen, iBufSize - iStrLen, " Uid=%llu", stHead.uid());
        iStrLen = strlen(str);
    }
    if (stHead.has_seq())
    {
        //CCLOG("seq=%d", stHead.seq());
        snprintf(str + iStrLen, iBufSize - iStrLen, " Seq=%d", stHead.seq());
        iStrLen = strlen(str);
    }
    if (stHead.has_act())
    {
        //CCLOG("act=%d", stHead.act());
        snprintf(str + iStrLen, iBufSize - iStrLen, " Act=%llu", stHead.act());
        iStrLen = strlen(str);
    }
    if (stHead.has_msg_type())
    {
        //CCLOG("msg_type=%d", stHead.msg_type());
        snprintf(str + iStrLen, iBufSize - iStrLen, " Msg_type=%d", stHead.msg_type());
        iStrLen = strlen(str);
    }
    if (stHead.has_app_id())
    {
        //CCLOG("app_id=%d", stHead.app_id());
        snprintf(str + iStrLen, iBufSize - iStrLen, " App_id=%d", stHead.app_id());
        iStrLen = strlen(str);
    }
    if (stHead.has_routeid())
    {
        //CCLOG("routeid=%d", stHead.routeid());
        snprintf(str + iStrLen, iBufSize - iStrLen, " Routeid=%d", stHead.routeid());
        iStrLen = strlen(str);
    }
    CCLOG("%s }", str);
}

void QNetMgr::SendPkg(const vmsg::VCli2LogicSvrMsg& stRqst, bool bIsCheckRecv)
{
	if(bIsCheckRecv) {
		m_uCheckCmd = stRqst.msg_head().msg_id();
		std::map<int, vmsg::VCli2LogicSvrMsg>::const_iterator targetIter = m_stMsgCacheMap.find(m_uCheckCmd);
		
		if(targetIter == m_stMsgCacheMap.end()) {
			m_stMsgCacheMap[m_uCheckCmd] = stRqst;
			startSecSchedule();
		}
	}

    int iRet = 0;
    memset(m_acCodeBuff, 0, BUF_SIZE);
    bool bCodeRslt = stRqst.SerializeToArray(m_acCodeBuff, sizeof(m_acCodeBuff));
    if (!bCodeRslt)
    {
        CCLOG("[error]fail to encode message %d!", stRqst.msg_head().msg_id());
        return;
    }

	//加密算法  长度 stRqst.ByteSize() + 4
	uint64 iCheckSum = 0;
	for (int iIdx = 0; iIdx < stRqst.ByteSize() - 4; iIdx++){
		unsigned int iCodeSeg = ntohl(*(unsigned int*)(m_acCodeBuff + iIdx));
		iCheckSum += iCodeSeg;
	}

	//CCLOG("the long check sum ====== %llu", iCheckSum);

	unsigned int finalCheckSum = (((iCheckSum & 0xFFFFFFFF) * stRqst.msg_head().seq()) & 0xFFFFFFFF);
	//CCLOG("msg head cmd  === %u, seq :: == %u =============",stRqst.msg_head().msg_id(), stRqst.msg_head().seq());

	//CCLOG("before htonl :: == %u", finalCheckSum);
	unsigned int be32 = htonl(finalCheckSum);
	//CCLOG("after htonl :: == %u", be32);
	*(unsigned int*)(&m_acCodeBuff[stRqst.ByteSize()]) = be32;

	//CCLOG("the msg length :: == %d", stRqst.ByteSize() + sizeof(be32));

    iRet = stNet.Send(m_acCodeBuff, stRqst.ByteSize() + sizeof(be32));
    if(0 != iRet) {
        CCLOG("[error]QNetMgr::SendPkg,fail to send cmd %d,ret %d", stRqst.msg_head().msg_id(), iRet);
        return;
    }
    PrintMsgHead(stRqst.GetTypeName().c_str(), stRqst.msg_head(), true);
}

int QNetMgr::RegCmdListener(const int iCmd, QBaseProxy *pstProxy, bool isUpdateFunc)
{
	if(NULL == pstProxy) {
		CCLOG("[error]QNetMgr::RegCmdListener, Fail to reg cmd target %d, pstProxy is null", iCmd);
        return -1;
	}


	map<int, QBaseProxy*>::const_iterator targetIter = m_stCmdProxyMap.find(iCmd);
	if (targetIter == m_stCmdProxyMap.end())
    {
        //reg
        m_stCmdProxyMap[iCmd] = pstProxy;
        CCLOG("[trace]reg cmd %d -> %s success", iCmd, typeid(*pstProxy).name());
		return 0;
    }

    if (targetIter != m_stCmdProxyMap.end() && pstProxy != targetIter->second)
    {
        CCLOG("[warning]cmd %d's function now is %s,tobe reg is %s", iCmd, typeid(*((targetIter->second))).name(), typeid(*pstProxy).name());
        if (isUpdateFunc)
        {
            m_stCmdProxyMap[iCmd] = pstProxy;
            CCLOG("[trace]update cmd %d -> %s", iCmd, typeid(*pstProxy).name());
        }
        
    }
    return 0;
}


int QNetMgr::InitNetConnection(const std::string &strIP, unsigned short uPort) {
    CCLOG("[trace]QNetMgr::InitNetConnection,ip=%s,port=%d", strIP.c_str(), uPort);
	stNet.Connect(strIP, uPort);
	//stNet.Connect("123.57.140.186", uPort);
	return 0;
}

int QNetMgr::DoRecv()
{
    if (!stNet.init())
    {
        // not connect
        return -1;
    }

    int iRet = 0;

    //recv
    int iLen = BUF_SIZE;
    memset(m_acCodeBuff, 0, BUF_SIZE);
    iRet = stNet.Recv(m_acCodeBuff, iLen);


    while (ESOCRET_OK == iRet)
    {
        //CCLOG("[trace]recv len %d", iLen);

        vmsg::VLogicSvr2CliMsg stRsp; //TODO:两种不同的格式
        stRsp.Clear();
        string strByte(m_acCodeBuff);
        int iVal = stRsp.ParseFromArray(m_acCodeBuff, iLen);
        
		if (0 == iVal)
		{
			CCLOG("[Error]%s,parse VLogicSvr2CliMsg fail! cmd=%d", __FUNCTION__, stRsp.msg_head().msg_id());
            
            string str = "parse fail! cmd = " + intToString(stRsp.msg_head().msg_id());
            COMMON_DIALOG->show(str, NULL, "OK");

			
		} 
		else
		{
			DoOneCmd(stRsp);

			unsigned int uCmd = stRsp.msg_head().msg_id();
			std::map<int, vmsg::VCli2LogicSvrMsg>::iterator targetIter = m_stMsgCacheMap.find(uCmd);
			
			if(targetIter != m_stMsgCacheMap.end()) {
				m_stMsgCacheMap.erase(targetIter);
			}
		}

        //recv
        memset(m_acCodeBuff, 0, BUF_SIZE);
        iLen = BUF_SIZE;
        iRet = stNet.Recv(m_acCodeBuff, iLen);
    }

    //check ret val
    if(iRet > 0 && ESOCRET_SEL_TIMEOUT != iRet && ESOCRET_SOCKET_PKG_NOT_READY != iRet) {
        CCLOG("[warn]QNetMgr::DoRecv, connection recv error,ret %d!!!", iRet);
        switch (iRet)
        {
        case ESOCRET_SOCK_ERR:
        case ESOCRET_DISCONNECT:
        case ESOCRET_INVALID_SOCKET:
            GameInf::DoConnectionClose();
            break;
        default:
            break;
        }
    }

    return 0;
}

void QNetMgr::startSecSchedule() {
	cocos2d::CCScheduler *pScheduler = cocos2d::CCDirector::sharedDirector()->getScheduler();
	pScheduler->scheduleSelector(schedule_selector(QNetMgr::secSchedule), this, 1.0f, false);
}

void QNetMgr::endSecSchedule() {
	cocos2d::CCScheduler *pScheduler = cocos2d::CCDirector::sharedDirector()->getScheduler();
	pScheduler->unscheduleSelector(schedule_selector(QNetMgr::secSchedule), this);
}

void QNetMgr::secSchedule(float dt) {
	if(!checkRecv(m_uCheckCmd)) { // 没有收到
        /*
		vmsg::VCli2LogicSvrMsg &stMsg = m_stMsgCacheMap[m_uCheckCmd];

		vmsg::MsgHead *pstMsgHead = stMsg.mutable_msg_head();
		pstMsgHead->set_uid(P_USER_OBJ->getUserInfo()->uin);
		pstMsgHead->set_act(1);
		pstMsgHead->set_msg_id(m_uCheckCmd);
		pstMsgHead->set_seq(1);
		pstMsgHead->set_msg_type(vmsg::MsgHead::RQST_MSG);
		pstMsgHead->set_routeid(P_LOGIN_OBJ->getCurSvrInfo().zoneid);
		pstMsgHead->set_app_id(APPID_VCONND);

		vmsg::CSPlayerLoginRqst *pstPlayerLoginRqst = stMsg.mutable_msg_head()->mutable_loginrqst();
		//pstPlayerLoginRqst->set_uid(stMsg.msg_head().msg_id());
		pstPlayerLoginRqst->set_uid(P_USER_OBJ->getUserInfo()->uin);
		pstPlayerLoginRqst->set_acctype(1);
		pstPlayerLoginRqst->set_platform(1);
		pstPlayerLoginRqst->set_zoneid(P_LOGIN_OBJ->getCurSvrInfo().zoneid);


		SendPkg(stMsg, true);
        */
	} else {
		endSecSchedule();
		m_uCheckCmd = 0;
	}
}

bool QNetMgr::checkRecv(unsigned int uCmd) {
	std::map<int, vmsg::VCli2LogicSvrMsg>::const_iterator targetIter = m_stMsgCacheMap.find(uCmd);

	return targetIter == m_stMsgCacheMap.end();
}

void QNetMgr::DoOneCmd(vmsg::VLogicSvr2CliMsg& stRsp)
{
	if(stRsp.msg_head().msg_id() != vmsg::CS_CMD_OSD_SYNC_PLAYER_IN_SCENE) {
		PrintMsgHead(stRsp.GetTypeName().c_str(),*stRsp.mutable_msg_head(), false);
	}
    
    const int iCmd = stRsp.msg_head().msg_id();
    if (vmsg::CS_CMD_ERR == iCmd)
    {
        CCLOG("-->Cmd=%d,ErrorCode=%d", stRsp.errmsg().rqstcmdid(), stRsp.errmsg().errorcode() );
        
        unsigned int uRqstCmdID = stRsp.errmsg().rqstcmdid();
		map<int, QBaseProxy*>::iterator rqstTargetIter = m_stCmdProxyMap.find(uRqstCmdID);
		if (rqstTargetIter != m_stCmdProxyMap.end())
		{
            QBaseProxy *pListener = rqstTargetIter->second;
            if (NULL != pListener)
            {
                pListener->fault(stRsp.errmsg()); //处理错误

                if(stRsp.errmsg().rqstcmdid() == vmsg::CS_CMD_PLAYER_LOGIN && stRsp.errmsg().errorcode() == 100005) {
                    return;
                } else {
                    string str;
                    const char* strErrDesc = ERROR_CODE_CFG_MASTER->getErrDesc(stRsp.errmsg().errorcode());
                    if(strErrDesc) {
                        str += strErrDesc;
                        CCLOG("[warn]cmd %d,errorcode %d,%s,%s", stRsp.errmsg().rqstcmdid(), stRsp.errmsg().errorcode(),
                                strErrDesc, typeid(*pListener).name() );
                    } else {
                        str += "msg=" + intToString(stRsp.errmsg().rqstcmdid());
                        str += "\nErrorCode == " + intToString(stRsp.errmsg().errorcode());
                        str += "\n" + string(typeid(*pListener).name());
                    }
                    
                    COMMON_DIALOG->show(str, NULL, NounStr(OK));
                }
                				
            }
		}
        return;
    }
	map<int, QBaseProxy*>::iterator targetIter = m_stCmdProxyMap.find(iCmd);
    if (targetIter == m_stCmdProxyMap.end())
    {
		if(vmsg::CS_CMD_OSD_SYNC_PLAYER_IN_SCENE != iCmd) {
			CCLOG("[warning]QNetMgr::DoOneCmd,not find cmd %d's function", iCmd);
		}
    }
    else
    {
		QBaseProxy *pListener = targetIter->second;
        if (NULL != pListener)
        {
            pListener->DoRsp(stRsp);
			//CCLOG("recv cmd=%d", iCmd);
        }
    }
    
}


void QNetMgr::GenRqstMsgHead(vmsg::MsgHead& stHead, const int iCmd, const int uUin, bool bIsRestSeq/* = false*/,  bool bIsUpdateSeq/* = true*/)
{
	if(bIsRestSeq){
		m_uSeq = 0;
	}

    stHead.set_uid(uUin); //2097152);
    stHead.set_act(1);
    stHead.set_msg_id(iCmd);
    stHead.set_seq(m_uSeq);
    stHead.set_msg_type(vmsg::MsgHead::RQST_MSG);
    stHead.set_routeid(GameInf::GetZoneID());
    stHead.set_app_id(vmsg::APPID_MAINSVR);

	if(bIsUpdateSeq){
		m_uSeq++;
	}
}

void QNetMgr::GenRqstMsgHead(vmsg::MsgHead& stHead, const int iCmd)
{
    GenRqstMsgHead(stHead, iCmd, P_USER_OBJ->getUserInfo()->uin, false);
}

void QNetMgr::delCmdListener(const int iCmd){
    if(m_stCmdProxyMap.count(iCmd)) {
        m_stCmdProxyMap.erase(iCmd);
    }
}


bool QNetMgr::IsConnect() {
    return stNet.IsConnect();
}

void QNetMgr::closeConn() {
#ifdef TGAME_DEV_TEST
    stNet.Close();
#endif
}
