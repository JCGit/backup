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
    // �����ʹ��: response->request->reqType��ȡ��������  
    if (0 != strlen(response->getHttpRequest()->getTag()))  
    {  
        CCLog("%s completed", response->getHttpRequest()->getTag());  
    }  
    //��ȡ״̬��  
    int statusCode = response->getResponseCode();  
    char statusString[64] = {};  
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());  
    //m_labelStatusCode->setString(statusString);  
    CCLog("response code: %d", statusCode);  
    if (!response->isSucceed())  
    {  
    //����ʧ�ܻ�ȡ������Ϣ  
        CCLog("response failed");  
        CCLog("error buffer: %s", response->getErrorBuffer());  
        return;  
    }  
    // ��ȡ�������ݡ�  
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
iLength ����ʱΪbuf�ĳ��ȣ�����ʱΪʵ�����ݵĳ���
����0��ʾ�ɹ���ȡ���ݣ���0��ʾ������
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
	// ��ʼ��
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

//����0��ʾ�ɹ�,������ʾʧ��
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

	// �������׽���
	m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sockClient == INVALID_SOCKET) {
		closeSocket();
		return -2;
	}

	// ����SOCKETΪKEEPALIVE
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
		DWORD nMode = 1; //��������ʽ
		int nRes = ioctlsocket(m_sockClient, FIONBIO, &nMode);
		if (nRes == SOCKET_ERROR) {
			closeSocket();
			return -4;
		}
#else
		// ����Ϊ��������ʽ
		fcntl(m_sockClient, F_SETFL, O_NONBLOCK);
#endif
	}


	unsigned long serveraddr = uiIPAddr;
	if(serveraddr == INADDR_NONE)    // ���IP��ַ��ʽ����
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
            fd_set m_stReadSet; //�ɶ�
            fd_set m_stWriteSet; //��д
            fd_set m_stExceptSet; //����
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

//����0��ʾ�ɹ�
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

	// ���ͨѶ��Ϣ������
	int packsize = 0;
	packsize = nSize;

	// ���BUF���
	if(m_nOutbufLen + nSize + 2 > OUTBUFSIZE) {
		// ��������OUTBUF�е����ݣ������OUTBUF��
		Flush();
		if(m_nOutbufLen + nSize + 2> OUTBUFSIZE) {
			// ������
			Destroy();
			return ESOCRET_OUT_BUF_NOT_ENOUGH;
		}
	}
	// ������ӵ�BUFβ
	m_bufOutput[m_nOutbufLen++] = ((nSize + 2) >> 8);
	m_bufOutput[m_nOutbufLen++] = ((nSize + 2) & 0xFF);
	memcpy(m_bufOutput + m_nOutbufLen, pBuf, nSize);
	m_nOutbufLen += nSize;
	return Flush();	
}

//����0��ʾ�ɹ�
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

//����0��ʾ�ɹ���ȡ���ݣ���0��ʾ������
int TCPSocket::ReceiveMsg(void* pBuf, int& nSize)
{
	//������
	if(pBuf == NULL || nSize <= 0) {
		return ESOCRET_INVALID_FUNC_PARAM;
	}

	if (m_sockClient == INVALID_SOCKET) {
		return ESOCRET_INVALID_SOCKET;
	}
    int iRet = ESOCRET_OK;
	// ����Ƿ���һ����Ϣ(С��2���޷���ȡ����Ϣ����)
	if(m_nInbufLen < 2) {
		//  ���û������ɹ�  ����   ���û��������ֱ�ӷ���
        iRet = recvFromSock();
		if(ESOCRET_OK != iRet ) {
			return iRet;
		}
	}

	// ����Ҫ��������Ϣ�Ĵ�С��һ����Ϣ����СΪ������Ϣ�ĵ�һ��16�ֽڣ�����Ϊ���λ�����������Ҫ�ֿ�����
	int packsize = (unsigned char)m_bufInput[(m_nInbufStart +1) % INBUFSIZE] +
		(unsigned char)m_bufInput[(m_nInbufStart + 0)] * 256 - 2; // ע���ֽ��򣬸�λ+��λ
	m_nInbufStart += 2;
	m_nInbufLen -= 2;
	// �����Ϣ���ߴ���� �ݶ����16k
	if (packsize <= 0 || packsize > _MAX_MSGSIZE) {
		m_nInbufLen = 0;        // ֱ�����INBUF
		m_nInbufStart = 0;
		return -1;
	}

	// �����Ϣ�Ƿ�����(�����Ҫ��������Ϣ���ڴ�ʱ���������ݳ��ȣ���Ҫ�ٴ��������ʣ������)
	if (packsize > m_nInbufLen) {
		// ���û������ɹ�   ����    ��Ȼ�޷���ȡ�����������ݰ�  �򷵻أ�ֱ��ȡ��������
		/*if (!recvFromSock() ) {    // ���m_nInbufLen�Ѹ���
			return false; 
		}*/
		// �ϴν��ղ��������ָ�ԭ״
		recvFromSock();
		m_nInbufStart -= 2;
		m_nInbufLen += 2;
        return ESOCRET_SOCKET_PKG_NOT_READY;
	} 

	if (packsize > m_nInbufLen) {
		nSize = 0;
		return ESOCRET_SOCKET_PKG_NOT_READY;
	}

	// ���Ƴ�һ����Ϣ
	if(m_nInbufStart + packsize > INBUFSIZE) {
		// ���һ����Ϣ�лؾ�����������ڻ��λ�������ͷβ��
		// �ȿ������λ�����ĩβ������
		int copylen = INBUFSIZE - m_nInbufStart;
		memcpy(pBuf, m_bufInput + m_nInbufStart, copylen);

		// �ٿ������λ�����ͷ����ʣ�ಿ��
		memcpy((unsigned char *)pBuf + copylen, m_bufInput, packsize - copylen);
		nSize = packsize;
	} else {
		// ��Ϣû�лؾ�����һ�ο�����ȥ
		memcpy(pBuf, m_bufInput + m_nInbufStart, packsize);
		nSize = packsize;
	}

	// ���¼��㻷�λ�����ͷ��λ��
	m_nInbufStart = (m_nInbufStart + packsize) % INBUFSIZE;
	m_nInbufLen -= packsize;
	
	return ESOCRET_OK;
}


// �������ж�ȡ�����ܶ�����ݣ�ʵ����������������ݵĵط�
// true��ʾ������Ϣ��false��ʾ���������Ϣ
int TCPSocket::recvFromSock(void)
{
	if (m_nInbufLen >= INBUFSIZE || m_sockClient == INVALID_SOCKET) {
		return ESOCRET_INVALID_SOCKET;
	}
    //////////////////select/////////////////
    timeval timeout;
    timeout.tv_sec    =  0; //nBlockSec;
    timeout.tv_usec    = 50; //΢��
    //fd_set writeset, exceptset;
    fd_set m_stReadSet; //�ɶ�
    fd_set m_stWriteSet; //��д
    fd_set m_stExceptSet; //����
    FD_ZERO(&m_stReadSet);
    FD_ZERO(&m_stWriteSet);
    FD_ZERO(&m_stExceptSet);
    FD_SET(m_sockClient, &m_stReadSet);
    FD_SET(m_sockClient, &m_stExceptSet);

    int ret = select(m_sockClient+1, &m_stReadSet, NULL, &m_stExceptSet, &timeout);
    if (ret <= 0) {
        //<0: select����
        //0�� �ȴ���ʱ��û�пɶ�д�������ļ�
        //time out
        //CCLog("TCPSocket::recvFromSock,select time out,ret %d", ret);
        return ESOCRET_SEL_TIMEOUT;
    } else {
        //ĳЩ�ļ��ɶ�д����� 0
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
	// ���յ�һ������
	int    savelen, savepos;            // ����Ҫ����ĳ��Ⱥ�λ��
	if(m_nInbufStart + m_nInbufLen < INBUFSIZE)    {    // INBUF�е�ʣ��ռ��л���
		savelen = INBUFSIZE - (m_nInbufStart + m_nInbufLen);        // �󲿿ռ䳤�ȣ����������ݵĳ���
	} else {
		savelen = INBUFSIZE - m_nInbufLen;
	}

	// ���������ݵ�ĩβ
	savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
	//CHECKF(savepos + savelen <= INBUFSIZE);
    int inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
    //CCLog("[trace]TCPSocket::recvFromSock,recv len %d", inlen);
	if(inlen > 0) {
		// �н��յ�����
		m_nInbufLen += inlen;

		if (m_nInbufLen > INBUFSIZE) {
			return ESOCRET_IN_BUF_NOT_ENOUGH;
		}

		// ���յڶ�������(һ�ν���û����ɣ����յڶ�������)
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
				// �����ѶϿ����ߴ��󣨰���������
				if (hasError()) {
					Destroy();
					return ESOCRET_SOCK_ERR;
				}
			}
		}
	} else if(inlen == 0) {
		Destroy();
        //=0 ���ӹر�
        //hasError();
		return ESOCRET_DISCONNECT;
	} else {
		// �����ѶϿ����ߴ��󣨰���������
		if (hasError()) {
			Destroy();
			return ESOCRET_SOCK_ERR;
		}
	}

	return ESOCRET_OK;
}

//����0��ʾ�ɹ�
//? ��� OUTBUF > SENDBUF ����Ҫ���SEND����
int TCPSocket::Flush(void)        
{
	if (m_sockClient == INVALID_SOCKET) {
		return ESOCRET_INVALID_SOCKET;
	}

	if(m_nOutbufLen <= 0) {
		return ESOCRET_OK;
	}

	// ����һ������
	int    outsize;
	outsize = send(m_sockClient, m_bufOutput, m_nOutbufLen, 0);
	if(outsize > 0) {
		// ɾ���ѷ��͵Ĳ���
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
	// ���״̬
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
		} else {    // ����
			return true;
		}
	} else {    // ������
		return true;
	}

	return true;
}

void TCPSocket::Destroy(void)
{
	// �ر�
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
    CCLog("wsaerror=%d", err); //122ָ���ݸ�ϵͳ���õĻ�����̫С����û�ҵ�ԭ��
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