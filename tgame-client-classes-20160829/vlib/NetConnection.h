#ifndef __NET_CONNECTION_H__
#define __NET_CONNECTION_H__

#include "cocos-ext.h"  

#include <cocos2d.h>  
#include <string>
//#include "ByteBuffer.h"
//#include "base/CVSingleton.h"

#ifdef WIN32

#include <windows.h>
#include <WinSock.h>
#pragma comment( lib, "ws2_32.lib" )

#else

#include <sys/types.h>          
#include <sys/socket.h>
typedef int SOCKET;
#include "unistd.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#define INVALID_SOCKET -1
//#define SOCKET_ERROR -1

#endif


#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif // !INVALID_SOCKET
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif // !SOCKET_ERROR



USING_NS_CC;  
#ifndef CHECKF
#define CHECKF(x) \
	do \
{ \
	if (!(x)) { \
	log_msg("CHECKF", #x, __FILE__, __LINE__); \
	return 0; \
	} \
} while (0)
#endif

#define _MAX_MSGSIZE 16 * 1024        // 暂定一个消息最大为16k
#define BLOCKSECONDS    30            // INIT函数阻塞时间
#define INBUFSIZE    (16*1024*3)        //?    具体尺寸根据剖面报告调整  接收数据的缓存
#define OUTBUFSIZE    (8*1024)        //? 具体尺寸根据剖面报告调整。 发送数据的缓存，当不超过8K时，FLUSH只需要SEND一次

//socket 的返回值定义
enum TGameSocketRetVal
{
    ESOCRET_OK = 0, //正确返回
    ESOCRET_NO_DATA = 101, //没有接收数据需要处理
    ESOCRET_SEL_TIMEOUT = 102, //接收select超时（或许暂时没有数据需要处理）

    ESOCRET_INVALID_FUNC_PARAM = 1, //函数的传入参数错误
    ESOCRET_INVALID_SOCKET = 2, //socket未建立？
    ESOCRET_INVALID_PKG_SIZE = 3, //无法获取到完整的数据包
    ESOCRET_SEL_EXCEPTION = 4, //select检查到文件描述符出错（socket或许断开？）
    ESOCRET_IN_BUF_NOT_ENOUGH = 5, //接收buff比pkg小
    ESOCRET_DISCONNECT = 6, //连接断开
    ESOCRET_SOCK_ERR = 7, //socket error
    ESOCRET_OUT_BUF_NOT_ENOUGH = 8, //发送buff比pkg小
    ESOCRET_SOCKET_PKG_NOT_READY = 9, //recv不是完整包，暂不处理
};

class TCPSocket 
{
public:
	TCPSocket(void);
	~TCPSocket(){}
	int    CreateByHostname(const char* pszServerIP, unsigned short nServerPort, int tagid, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);
	int    CreateByIPStr(const char* pszSvrHostname, unsigned short nServerPort, int tagid, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);
	int    Create(unsigned int uiIPAddr, unsigned short nServerPort, int tagid, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);
	int     Connect(unsigned int uiIPAddr, unsigned short nServerPort, int tagid, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);
	int    SendMsg(void* pBuf, int nSize, bool bTryConnect);
	int    ReceiveMsg(void* pBuf, int& nSize);
	int    Flush(void);
	bool    Check(void);
	void    Destroy(void);
	SOCKET    GetSocket(void) const { return m_sockClient; }

	int        getTagID(){ return m_tag; }
	void    closeSocket();
	const std::string& GetSvrIP() const{return m_stSvrAddr;}
	unsigned short GetSvrPort() const {return m_ushPort;};
	enum ECONNSTAT
	{
		ECONN_CLOSED = 0,
		ECONN_CONNECTING = 1,
		ECONN_CONNECTED = 2,
	};
    bool IsConnect(){return m_uiConnectStat == ECONN_CONNECTED;};
private:
	int Reconnect();
	

private:
	int    recvFromSock(void);        // 从网络中读取尽可能多的数据
	bool    hasError();            // 是否发生错误，注意，异步模式未完成非错误


	SOCKET    m_sockClient;

	// 发送数据缓冲
	char    m_bufOutput[OUTBUFSIZE];    //? 可优化为指针数组
	int        m_nOutbufLen;

	// 环形缓冲区
	char    m_bufInput[INBUFSIZE];
	int        m_nInbufLen;
	int        m_nInbufStart;                // INBUF使用循环式队列，该变量为队列起点，0 - (SIZE-1)
	int        m_tag;
	std::string  m_stSvrAddr;
	unsigned short m_ushPort;
	unsigned int m_uiIpAddr;
	unsigned int m_uiConnectStat;           //连接状态


};
  
class NetConnection: public CCObject  
{  
private:
	TCPSocket m_stSocket;
public:  
    CREATE_FUNC(NetConnection);  
  
	virtual bool init(){return m_stSocket.IsConnect();};  
  
    //void getHttpData(const std::string& stSvrUrl, const std::string& stBuf);  
  
    //void httpReqFinished(CCNode* node,CCObject* obj);  
	int Connect(const std::string& stSvrAddr, unsigned short ushPort);
	int Connect(unsigned int uIP, unsigned short ushPort);
	int Send(const std::string& stBuf, bool bTryConnect = true);
	int Send(const char* pzBuf, unsigned short ushLen, bool bTryConnect = true);
	int Recv(char* pszBuf, int&length);
	int ConnectSend(std::string& stBuf);
	int RecvClose(std::string& stBuf);
    int Close(){

		m_stSocket.closeSocket();

		return 0;
	}

	unsigned int GetSocket() const{
		return m_stSocket.GetSocket();
	}

	const std::string& GetSvrAddr() const{
		return m_stSocket.GetSvrIP();
	}

	unsigned short GetSvrPort() const{
		return m_stSocket.GetSvrPort();
	}

    bool IsConnect();

	~NetConnection(){}
};  

//#define P_CONN  CVLib::CVSingleton<NetConnection>::Instance()


#endif