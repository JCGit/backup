#ifndef  _NET_MGR_
#define  _NET_MGR_
#include <map>
#include "vlib/NetConnection.h"
#include "ClientSvrMsg.pb.h"
#include "PlayerLoginMsg.pb.h"
#include "cocos2d.h"

#include "QBaseProxy.h"

//网络连接管理
class QNetMgr : public cocos2d::CCObject
{
public:
	QNetMgr();
	~QNetMgr();  
    enum {
        BUF_SIZE = 65535,
    };
	int InitNetConnection(const std::string &strIP, unsigned short uPort);
    int DoRecv();
    
    void GenRqstMsgHead(vmsg::MsgHead& stHead, const int iCmd,  const int uUin, bool bIsRestSeq = false, bool bIsUpdateSeq = true);
    void GenRqstMsgHead(vmsg::MsgHead& stHead, const int iCmd);

    //注册消息处理函数,如果已经注册过并且isUpdateFunc是true,则更新该函数指针
	int RegCmdListener(const int iCmd, QBaseProxy *pstProxy, bool isUpdateFunc = false);

    void SendPkg(const vmsg::VCli2LogicSvrMsg& stMsg, bool bIsCheckRecv = false);

	void startSecSchedule();

	void endSecSchedule();

	void secSchedule(float dt); // 每秒回调

	bool checkRecv(unsigned int uCmd);

    void delCmdListener(const int iCmd);

    bool IsConnect();

    void closeConn(); //仅用作测试
private:
    void DoOneCmd(vmsg::VLogicSvr2CliMsg& stRsp);
    void PrintMsgHead(const char* prefix,  const vmsg::MsgHead& stHead, bool isSend);
private:
	std::map<int, QBaseProxy*> m_stCmdProxyMap; // <CmdID, CmdTarget>
	std::map<int, vmsg::VCli2LogicSvrMsg> m_stMsgCacheMap; // <CmdID, MsgTarget>
	unsigned int m_uCheckCmd;
    NetConnection stNet;

	unsigned int m_uSeq;

    char m_acCodeBuff[BUF_SIZE];
};

#define P_NET_MGR  CVLib::CVSingleton<QNetMgr>::Instance()

#endif // _NET_MGR_

