#include "NetConnection.h"
#include "cocos-ext.h"

#ifdef WIN32

#else
#include "netdb.h"
#include <fcntl.h>
#include "errno.h"
#endif

using namespace cocos2d::extension;
/*
void NetConnection::getHttpData(const std::string& stSvrUrl, const std::string& stContent)  
{  
    cocos2d::extension::CCHttpClient* httpClient = cocos2d::extension::CCHttpClient::getInstance();  
  
    cocos2d::extension::CCHttpRequest* httpReq =new cocos2d::extension::CCHttpRequest();  
  
    httpReq->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);  
    httpReq->setUrl(stSvrUrl.c_str());  
    httpReq->setResponseCallback(this,callfuncND_selector(NetConnection::httpReqFinished));  
    httpReq->setTag("FirstNet");  
    httpClient->setTimeoutForConnect(30);  
    httpClient->send(httpReq);  
  
    httpReq->release();  
	
}  


void NetConnection::httpReqFinished(CCNode* node,CCObject* obj)
{
	CCHttpResponse *response = (CCHttpResponse*)obj;  
    if (!response)  
    {  
        return;  
    }  
    // 你可以使用: response->request->reqType获取请求类型  
    if (0 != strlen(response->getHttpRequest()->getTag()))  
    {  
        CCLog("%s completed", response->getHttpRequest()->getTag());  
    }  
    //获取状态码  
    int statusCode = response->getResponseCode();  
    char statusString[64] = {};  
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());  
    //m_labelStatusCode->setString(statusString);  
    CCLog("response code: %d", statusCode);  
    if (!response->isSucceed())  
    {  
    //访问失败获取错误信息  
        CCLog("response failed");  
        CCLog("error buffer: %s", response->getErrorBuffer());  
        return;  
    }  
    // 获取返回数据。  
    std::vector<char> *buffer = response->getResponseData();  
    
    printf("Http Test, dump data: ");  
    for (unsigned int i = 0; i < buffer->size(); i++)  
    {  
        printf("%c", (*buffer)[i]);  
    }  
    printf("\n");  
    
	CCLog("return: %s", buffer);
    
}
*/

int NetConnection::Connect(const std::string& stSvrAddr, unsigned short ushPort)
{
	return m_stSocket.CreateByIPStr(stSvrAddr.c_str(), ushPort, 1, 0, true);
}

int NetConnection::Connect(unsigned int uIP, unsigned short ushPort){
	return m_stSocket.Create(uIP, ushPort, 1, 0, true);
}


int NetConnection::Send(const char* pzBuf, unsigned short ushLen, bool bTryConnect)
{
	
	return m_stSocket.SendMsg((void*)pzBuf, ushLen, bTryConnect);
}

int NetConnection::Send(const std::string& stBuf, bool bTryConnect)
{
	return m_stSocket.SendMsg((void*)stBuf.c_str(), stBuf.length()+1, bTryConnect);
}

/*
iLength 传入时为buf的长度，传出时为实际数据的长度
返回0表示成功收取数据，非0表示错误码
*/
int NetConnection::Recv(char* pszBuf, int& iLength)
{
	int iRet = m_stSocket.ReceiveMsg(pszBuf, iLength);
	return iRet;
}

bool NetConnection::IsConnect() {
    return m_stSocket.IsConnect();
}



TCPSocket::TCPSocket()
{ 
    m_sockClient = INVALID_SOCKET; //init
	// 初始化
	memset(m_bufOutput, 0, sizeof(m_bufOutput));
	memset(m_bufInput, 0, sizeof(m_bufInput));
}

void TCPSocket::closeSocket()
{
#ifdef WIN32
	closesocket(m_sockClient);
	WSACleanup();
#else
	close(m_sockClient);
#endif
    m_uiConnectStat = ECONN_CLOSED;
}

int TCPSocket::CreateByHostname(const char* pszSvrHostname, unsigned short nServerPort, int tagid, int nBlockSec /* = BLOCKSECONDS */, bool bKeepAlive /* = false */)
{
	unsigned int uiSvrIP;
#ifdef WIN32

#else

#endif
	hostent *h = gethostbyname(pszSvrHostname);
	uiSvrIP = *(reinterpret_cast<unsigned int *> (h->h_addr_list[0]));
	return Create(uiSvrIP, nServerPort, tagid, nBlockSec, bKeepAlive);
}

int TCPSocket::CreateByIPStr(const char* pszServerIP, unsigned short nServerPort, int tagid, int nBlockSec /* = BLOCKSECONDS */, bool bKeepAlive /* = false */)
{
    unsigned int uiIPAddr = inet_addr(pszServerIP);
    int iRet = Create(uiIPAddr, nServerPort, tagid, nBlockSec, bKeepAlive);
    CCLog("[trace]TCPSocket::CreateByIPStr,ret %d,ip=%s,port=%d,tagid=%d,blocksec=%d", iRet, pszServerIP, nServerPort, tagid, nBlockSec);
	return iRet;
}

int TCPSocket::Create(unsigned int uiIPAddr, unsigned short nServerPort, int tagid, int nBlockSec /* = BLOCKSECONDS */, bool bKeepAlive /* = false */)
{
	m_nInbufLen        = 0;
	m_nInbufStart    = 0;
	m_nOutbufLen    = 0;

	
	m_uiIpAddr = uiIPAddr;
	m_ushPort = nServerPort;
    int iRet = Connect(uiIPAddr, nServerPort, tagid, nBlockSec, bKeepAlive);
    CCLog("[trace]TCPSocket::Create,Connect ret %d,ip=%u,port=%d,tagid=%d,blocksec=%d", iRet, uiIPAddr, nServerPort, tagid, nBlockSec);
	if (0 == iRet)
	{		
        m_uiConnectStat = ECONN_CONNECTED;
		Flush();
		return 0;
	}
	else
    {
        return -1;
    }
		//return iRet;
  
}

//返回0表示成功,负数表示失败
int TCPSocket::Connect(unsigned int uiIPAddr, unsigned short nServerPort, int tagid, int nBlockSec /* = BLOCKSECONDS */, bool bKeepAlive /* = false */)
{
#ifdef WIN32
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if (ret != 0) {
		return -1;
	}
#endif

	// 创建主套接字
	m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sockClient == INVALID_SOCKET) {
		closeSocket();
		return -2;
	}

	// 设置SOCKET为KEEPALIVE
	if(bKeepAlive)
	{
		int        optval=1;
		if(setsockopt(m_sockClient, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval)))
		{
			closeSocket();
			return -3;
		}
	}

	if (nBlockSec == 0)
	{
#ifdef WIN32
		DWORD nMode = 1; //非阻塞方式
		int nRes = ioctlsocket(m_sockClient, FIONBIO, &nMode);
		if (nRes == SOCKET_ERROR) {
			closeSocket();
			return -4;
		}
#else
		// 设置为非阻塞方式
		fcntl(m_sockClient, F_SETFL, O_NONBLOCK);
#endif
	}


	unsigned long serveraddr = uiIPAddr;
	if(serveraddr == INADDR_NONE)    // 检查IP地址格式错误
	{
		closeSocket();
		return -5;
	}

	sockaddr_in    addr_in;
	memset((void *)&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(nServerPort);
	addr_in.sin_addr.s_addr = serveraddr;

    //If no error occurs, connect returns zero.Otherwise, it returns SOCKET_ERROR,
	if(connect(m_sockClient, (sockaddr *)&addr_in, sizeof(addr_in)) == SOCKET_ERROR) {
		if (hasError()) {
			closeSocket();
			return -6;
		}
		else    // WSAWOLDBLOCK
		{
            
			timeval timeout;
			timeout.tv_sec    =  10; //nBlockSec;
			timeout.tv_usec    = 0;
            //fd_set writeset, exceptset;
            fd_set m_stReadSet; //可读
            fd_set m_stWriteSet; //可写
            fd_set m_stExceptSet; //出错
            FD_ZERO(&m_stReadSet);
			FD_ZERO(&m_stWriteSet);
			FD_ZERO(&m_stExceptSet);
			FD_SET(m_sockClient, &m_stWriteSet);
			FD_SET(m_sockClient, &m_stExceptSet);

			int ret = select(FD_SETSIZE, NULL, &m_stWriteSet, &m_stExceptSet, &timeout);
			if (ret == 0 || ret < 0) {
                //time out
                CCLog("[error]TCPSocket::Connect,select ret %d", ret);
                hasError();
				closeSocket();
				return -7;
			} else {
				ret = FD_ISSET(m_sockClient, &m_stExceptSet);
				if(ret)        // or (!FD_ISSET(m_sockClient, &writeset)
                {
                    //exceptset,error
                    CCLog("TCPSocket::Connect,FD_ISSET exceptset ret %d", ret);
                    hasError();
					closeSocket();
					return -8;
				}
			}
            CCLog("[error]TCPSocket::Connect,select done ret %d,connect success", ret);
		}
	}

	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	//setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));
	m_tag = tagid;
	m_uiConnectStat = ECONN_CONNECTED;

	return 0;

}

//返回0表示成功
int TCPSocket::SendMsg(void* pBuf, int nSize, bool bTryConnect)
{
	if(pBuf == 0 || nSize <= 0) {
		return ESOCRET_INVALID_FUNC_PARAM;
	}

	if (m_sockClient == INVALID_SOCKET) {
		return ESOCRET_INVALID_SOCKET;
	}
    if(m_uiConnectStat == ECONN_CLOSED)
    {
        if (bTryConnect)
        {
            Reconnect();
        }
        else 
        {
            return ESOCRET_DISCONNECT;
        }
    }

	// 检查通讯消息包长度
	int packsize = 0;
	packsize = nSize;

	// 检测BUF溢出
	if(m_nOutbufLen + nSize + 2 > OUTBUFSIZE) {
		// 立即发送OUTBUF中的数据，以清空OUTBUF。
		Flush();
		if(m_nOutbufLen + nSize + 2> OUTBUFSIZE) {
			// 出错了
			Destroy();
			return ESOCRET_OUT_BUF_NOT_ENOUGH;
		}
	}
	// 数据添加到BUF尾
	m_bufOutput[m_nOutbufLen++] = ((nSize + 2) >> 8);
	m_bufOutput[m_nOutbufLen++] = ((nSize + 2) & 0xFF);
	memcpy(m_bufOutput + m_nOutbufLen, pBuf, nSize);
	m_nOutbufLen += nSize;
	return Flush();	
}

//返回0表示成功
int TCPSocket::Reconnect()
{
	if (m_uiIpAddr == 0 && m_stSvrAddr.length() == 0)
	{
		return -1;
	}

	if (m_stSvrAddr.length())
	{
		return CreateByHostname(m_stSvrAddr.c_str(), m_ushPort, m_tag, 0, true);
	}
    CCLog("[trace]TCPSocket::Reconnect,reconnect,ip %d",m_uiIpAddr);
	return Create(m_uiIpAddr, m_ushPort, m_tag, 0, true);
}

//返回0表示成功收取数据，非0表示错误码
int TCPSocket::ReceiveMsg(void* pBuf, int& nSize)
{
	//检查参数
	if(pBuf == NULL || nSize <= 0) {
		return ESOCRET_INVALID_FUNC_PARAM;
	}

	if (m_sockClient == INVALID_SOCKET) {
		return ESOCRET_INVALID_SOCKET;
	}
    int iRet = ESOCRET_OK;
	// 检查是否有一个消息(小于2则无法获取到消息长度)
	if(m_nInbufLen < 2) {
		//  如果没有请求成功  或者   如果没有数据则直接返回
        iRet = recvFromSock();
		if(ESOCRET_OK != iRet ) {
			return iRet;
		}
	}

	// 计算要拷贝的消息的大小（一个消息，大小为整个消息的第一个16字节），因为环形缓冲区，所以要分开计算
	int packsize = (unsigned char)m_bufInput[(m_nInbufStart +1) % INBUFSIZE] +
		(unsigned char)m_bufInput[(m_nInbufStart + 0)] * 256 - 2; // 注意字节序，高位+低位
	m_nInbufStart += 2;
	m_nInbufLen -= 2;
	// 检测消息包尺寸错误 暂定最大16k
	if (packsize <= 0 || packsize > _MAX_MSGSIZE) {
		m_nInbufLen = 0;        // 直接清空INBUF
		m_nInbufStart = 0;
		return -1;
	}

	// 检查消息是否完整(如果将要拷贝的消息大于此时缓冲区数据长度，需要再次请求接收剩余数据)
	if (packsize > m_nInbufLen) {
		// 如果没有请求成功   或者    依然无法获取到完整的数据包  则返回，直到取得完整包
		/*if (!recvFromSock() ) {    // 这个m_nInbufLen已更新
			return false; 
		}*/
		// 上次接收不完整，恢复原状
		recvFromSock();
		m_nInbufStart -= 2;
		m_nInbufLen += 2;
        return ESOCRET_SOCKET_PKG_NOT_READY;
	} 

	if (packsize > m_nInbufLen) {
		nSize = 0;
		return ESOCRET_SOCKET_PKG_NOT_READY;
	}

	// 复制出一个消息
	if(m_nInbufStart + packsize > INBUFSIZE) {
		// 如果一个消息有回卷（被拆成两份在环形缓冲区的头尾）
		// 先拷贝环形缓冲区末尾的数据
		int copylen = INBUFSIZE - m_nInbufStart;
		memcpy(pBuf, m_bufInput + m_nInbufStart, copylen);

		// 再拷贝环形缓冲区头部的剩余部分
		memcpy((unsigned char *)pBuf + copylen, m_bufInput, packsize - copylen);
		nSize = packsize;
	} else {
		// 消息没有回卷，可以一次拷贝出去
		memcpy(pBuf, m_bufInput + m_nInbufStart, packsize);
		nSize = packsize;
	}

	// 重新计算环形缓冲区头部位置
	m_nInbufStart = (m_nInbufStart + packsize) % INBUFSIZE;
	m_nInbufLen -= packsize;
	
	return ESOCRET_OK;
}


// 从网络中读取尽可能多的数据，实际向服务器请求数据的地方
// true表示读到消息；false表示出错或无消息
int TCPSocket::recvFromSock(void)
{
	if (m_nInbufLen >= INBUFSIZE || m_sockClient == INVALID_SOCKET) {
		return ESOCRET_INVALID_SOCKET;
	}
    //////////////////select/////////////////
    timeval timeout;
    timeout.tv_sec    =  0; //nBlockSec;
    timeout.tv_usec    = 50; //微秒
    //fd_set writeset, exceptset;
    fd_set m_stReadSet; //可读
    fd_set m_stWriteSet; //可写
    fd_set m_stExceptSet; //出错
    FD_ZERO(&m_stReadSet);
    FD_ZERO(&m_stWriteSet);
    FD_ZERO(&m_stExceptSet);
    FD_SET(m_sockClient, &m_stReadSet);
    FD_SET(m_sockClient, &m_stExceptSet);

    int ret = select(m_sockClient+1, &m_stReadSet, NULL, &m_stExceptSet, &timeout);
    if (ret <= 0) {
        //<0: select错误
        //0： 等待超时，没有可读写或错误的文件
        //time out
        //CCLog("TCPSocket::recvFromSock,select time out,ret %d", ret);
        return ESOCRET_SEL_TIMEOUT;
    } else {
        //某些文件可读写或出错 0
        //CCLog("TCPSocket::recvFromSock,ret %d", ret);
        if(FD_ISSET(m_sockClient, &m_stExceptSet)) {
            //exceptset,error
            //CCLog("TCPSocket::recvFromSock,FD_ISSET exceptset");
            closeSocket();
            return ESOCRET_SEL_EXCEPTION;
        } else if(FD_ISSET(m_sockClient, &m_stReadSet)) {
            //CCLog("TCPSocket::recvFromSock,FD_ISSET readset,");
        } else {
            //CCLog("[error]TCPSocket::recvFromSock,select done ret %d", ret);
            return ESOCRET_NO_DATA;
        }
    }

    /////////////////////////////////////////
	// 接收第一段数据
	int    savelen, savepos;            // 数据要保存的长度和位置
	if(m_nInbufStart + m_nInbufLen < INBUFSIZE)    {    // INBUF中的剩余空间有回绕
		savelen = INBUFSIZE - (m_nInbufStart + m_nInbufLen);        // 后部空间长度，最大接收数据的长度
	} else {
		savelen = INBUFSIZE - m_nInbufLen;
	}

	// 缓冲区数据的末尾
	savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
	//CHECKF(savepos + savelen <= INBUFSIZE);
    int inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
    //CCLog("[trace]TCPSocket::recvFromSock,recv len %d", inlen);
	if(inlen > 0) {
		// 有接收到数据
		m_nInbufLen += inlen;

		if (m_nInbufLen > INBUFSIZE) {
			return ESOCRET_IN_BUF_NOT_ENOUGH;
		}

		// 接收第二段数据(一次接收没有完成，接收第二段数据)
		if(inlen == savelen && m_nInbufLen < INBUFSIZE) {
			int savelen = INBUFSIZE - m_nInbufLen;
			int savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
			//CHECKF(savepos + savelen <= INBUFSIZE);
            inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
            //CCLog("[trace]TCPSocket::recvFromSock,recv len2 %d", inlen);
			if(inlen > 0) {
				m_nInbufLen += inlen;
				if (m_nInbufLen > INBUFSIZE) {
					return ESOCRET_IN_BUF_NOT_ENOUGH;
				}    
			} else if(inlen == 0) {
				//Destroy();
				return ESOCRET_DISCONNECT;
			} else {
				// 连接已断开或者错误（包括阻塞）
				if (hasError()) {
					Destroy();
					return ESOCRET_SOCK_ERR;
				}
			}
		}
	} else if(inlen == 0) {
		Destroy();
        //=0 连接关闭
        //hasError();
		return ESOCRET_DISCONNECT;
	} else {
		// 连接已断开或者错误（包括阻塞）
		if (hasError()) {
			Destroy();
			return ESOCRET_SOCK_ERR;
		}
	}

	return ESOCRET_OK;
}

//返回0表示成功
//? 如果 OUTBUF > SENDBUF 则需要多次SEND（）
int TCPSocket::Flush(void)        
{
	if (m_sockClient == INVALID_SOCKET) {
		return ESOCRET_INVALID_SOCKET;
	}

	if(m_nOutbufLen <= 0) {
		return ESOCRET_OK;
	}

	// 发送一段数据
	int    outsize;
	outsize = send(m_sockClient, m_bufOutput, m_nOutbufLen, 0);
	if(outsize > 0) {
		// 删除已发送的部分
		if(m_nOutbufLen - outsize > 0) {
			memcpy(m_bufOutput, m_bufOutput + outsize, m_nOutbufLen - outsize);
		}

		m_nOutbufLen -= outsize;

		if (m_nOutbufLen < 0) {
			return ESOCRET_OUT_BUF_NOT_ENOUGH;
		}
	} else {
		CCLog("[error]TCPSocket::Flush,fail to send msg, errno %d: %s", errno, strerror(errno));
		if (hasError()) {
			Destroy();
			return ESOCRET_SOCK_ERR;
		}
	}
    //CCLog("[trace]TCPSocket::Flush,sendlen=%d", outsize);

	return ESOCRET_OK;
}

bool TCPSocket::Check(void)
{
	// 检查状态
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	char buf[1];
	int ret = recv(m_sockClient, buf, 1, MSG_PEEK);
	if(ret == 0) {
		Destroy();
		return false;
	} else if(ret < 0) {
		if (hasError()) {
			Destroy();
			return false;
		} else {    // 阻塞
			return true;
		}
	} else {    // 有数据
		return true;
	}

	return true;
}

void TCPSocket::Destroy(void)
{
	// 关闭
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	int ret = setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	closeSocket();

	m_sockClient = INVALID_SOCKET;
	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;

	memset(m_bufOutput, 0, sizeof(m_bufOutput));
	memset(m_bufInput, 0, sizeof(m_bufInput));
}


bool TCPSocket::hasError()
{
#ifdef WIN32
    int err = WSAGetLastError();
    CCLog("wsaerror=%d", err); //122指传递给系统调用的缓冲区太小。还没找到原因
    if(err != 0 && err != 2 && err != 122 && err != WSAEWOULDBLOCK) {
#else
    int err = errno;
    if(err != EINPROGRESS && err != EAGAIN) {
#endif
        CCLog("TCPSocket::hasError,Error!WSAGetLastError=%d", err);
        return true;
    }

    return false;
}