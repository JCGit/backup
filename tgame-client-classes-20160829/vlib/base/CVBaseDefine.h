/**
* ��������.
*
* ���ļ�����Windows��Linux���������Ĳ���\n
* ����utf8��֧�ֹ��ʻ�i18n, ���ػ�l10n�������ڲ����ÿ���unicode�ķ�ʽ
*
* @author thinkry
*/

#ifndef __CVLIB_BASEDEFINE_H__
#define __CVLIB_BASEDEFINE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>

// ��Ȼ���鲻Ҫ��ͷ�ļ���ʹ��using namespace����std̫���õ��ˣ�����Ͱ���using��
using namespace std;

#ifdef WIN32
    #pragma warning(disable: 4503)  // ���ι��ڳ����������ĳ��ȣ����Ʊ��ضϵ�WARNING
    #pragma warning(disable: 4786)  // ���ι��ں���������256��WARNING
    #pragma warning(disable: 4127)  // ���ι����������ʽ�ǳ�����WARNING
    #pragma warning(disable: 4996)  // ���ι��ڱ�����Ϊ�����_CRT_SECURE_NO_DEPRECATE��WARNING
    #pragma warning(disable: 4100)  // ���ι���δ���õ��βε�WARNING

    #ifndef _WINSOCKAPI_
        #define _WINSOCKAPI_
    #endif
    #include <Windows.h>
    #include <WinSock2.h>
    #include <io.h>
#else
    #include <fcntl.h>
    #include <ctype.h>
    #include <unistd.h>
    #include <sys/time.h>
#endif

#if __WORDSIZE == 64
#define CV_BIT64
#else
#define CV_BIT32
#endif

typedef void* CV_POINT;

#define CV_STRSIZE(x)	x,sizeof(x)					///< ����snprintf�еļ򻯺�
#define CV_COUNTOF(x)	(sizeof(x)/sizeof(x[0]))
#define CV_NOTNULL(x)   (x == NULL ? "" : x)

#define CV_MAX(a, b)	((a) > (b) ? (a) : (b))
#define CV_MIN(a, b)	((a) < (b) ? (a) : (b))
#define CV_CEIL(a, b) ((a) == 0 ? 0 : ((a) - 1) / (b) + 1)

#define CV_UNUSED(x)    // �����Щ�����ں�����û�õ�����������Ա��⾯��
#define CV_NAME(x)  #x

// ��windef.h�����ĺ�
// 2��unsigned char��unsignd short��ת��
#define CV_USHORT_MAKE(high, low)   ((unsigned short)(((unsigned char)((low) & 0xFF)) | ((unsigned short)((unsigned char)((high) & 0xFF))) << 8))
#define CV_USHORT_LOW(i)            ((unsigned char)((unsigned short)(i) & 0xFF))
#define CV_USHORT_HIGH(i)           ((unsigned char)((unsigned short)(i) >> 8))

// 2��unsigned short��unsigned int��ת��
#define CV_UINT_MAKE(high, low)   ((unsigned int)(((unsigned short)((low) & 0xFFFF)) | ((unsigned int)((unsigned short)((high) & 0xFFFF))) << 16))
#define CV_UINT_LOW(i)            ((unsigned short)((unsigned int)(i) & 0xFFFF))
#define CV_UINT_HIGH(i)           ((unsigned short)((unsigned int)(i) >> 16))

// 2��unsigned int��uint64��ת��
#define CV_UINT64_MAKE(high, low)   ((uint64)(((unsigned int)((low) & 0xFFFFFFFF)) | ((uint64)((unsigned int)((high) & 0xFFFFFFFF))) << 32))
#define CV_UINT64_LOW(i)            ((unsigned int)((uint64)(i) & 0xFFFFFFFF))
#define CV_UINT64_HIGH(i)           ((unsigned int)((uint64)(i) >> 32))

// 32λ��λ����
// pos��ȡֵ��Χ��0-31
#define CV_BIT_SET(i, pos)      (i |= ((unsigned int)1 << pos))        // �ѵ�posλ��1
#define CV_BIT_CLR(i, pos)      (i &= ~((unsigned int)1 << pos))       // �ѵ�posλ��0
#define CV_BIT_TEST(i, pos)     ((i & ((unsigned int)1 << pos)) != 0)  // �жϵ�pos�ǲ���1

namespace CVLib
{

/**
* ��������
*/
enum
{
    CV_PATH_MAX = 1024,       ///< ·������󳤶�
    CV_INT_MAX = 0x7FFFFFFF, ///< int���͵��������
    CV_CONF_TXT_MAX = 1024,  ///< ����ı����ó���
};

} // namespace CVLib


// ��i18n�йصĺ궨��
// Ҫ������������Ե��ַ���������_("")���������Ա�gettext������ֱ�ӴӴ����г�ȡ����
#ifndef _
    #ifdef CV_GETTEXT
        #define _(x) gettext(x)
    #else
        #define _(x) x
    #endif
#endif

#ifdef WIN32
    typedef unsigned __int64 uint64;
    typedef __int64 int64;
    typedef int socklen_t;
    //typedef unsigned int pid_t;

    // Windows socket�Ĵ�����ͺ����linux��һ����ͨ����ͳһ
    // ���Ƿ���WSAEWOULDBLOCK
    #define CV_ERRNO			GetLastError()
    #define CV_WSA_ERRNO		WSAGetLastError()
    #define CV_EWOULDBLOCK      WSAEWOULDBLOCK	// 10035
    #define CV_EINPROGRESS      WSAEINPROGRESS	// 10036
    #define CV_ETIME            WSAETIMEDOUT	// 10060
    #define CV_EINTR            WSAEINTR		// 10000
    #define	CV_ECONNRESET       WSAECONNRESET	// 10054, ������ʾԶ�̹ر�������
    #define GetNowProcessId GetCurrentProcessId 
#else   //not WIN32
    #define closesocket			close
    #define GetNowProcessId	getpid
    #define GetCurrentThreadId	pthread_self
#ifdef CV_BIT64
    //typedef long int64;
    //typedef unsigned long uint64;
#else
    typedef long long int64;
    typedef unsigned long long uint64;
#endif

    typedef int SOCKET;

#ifndef INVALID_SOCKET
    #define INVALID_SOCKET		(SOCKET)(~0)
#endif

    #define CV_ERRNO            errno
    #define CV_WSA_ERRNO        errno
    #define CV_EWOULDBLOCK      EWOULDBLOCK
    #define CV_EINPROGRESS      EINPROGRESS
    #define CV_ETIME            ETIME
    #define CV_EINTR            EINTR
    #define	CV_ECONNRESET       ECONNRESET
#endif //end WIN32

// ����Windows��Linux����Щ��������
#ifdef WIN32
    #define strcasecmp          _stricmp
    #define strncasecmp		    _strnicmp
    #define wcscasecmp          _wcsicmp
    #define wcsncasecmp		    _wcsnicmp
    #define strtoll             _strtoi64
    #define strtoull            _strtoui64
    #define wcstoll             _wcstoi64
    #define wcstoull            _wcstoui64

    #define sleep               CVLib::CVPlatform::_ct_sleep
    #define usleep              CVLib::CVPlatform::_ct_usleep

    #define ct_fopen            CVLib::CVPlatform::_ct_fopen
    #define ct_stat             _stat32
    #define ct_access           _access
    #define ct_rename           rename
    #define ct_unlink           _unlink
    #define ct_remove           remove
#else
    #define ct_fopen            fopen
    #define ct_stat             stat
    #define ct_access           access
    #define ct_rename           rename
    #define ct_unlink           unlink
    #define ct_remove           remove
#endif

// windows��linux��snprintf�ķ���ֵ��һ����ͨ�����ַ�ʽ����
#define ct_snprintf         CVLib::CVPlatform::_ct_snprintf
#define ct_vsnprintf		CVLib::CVPlatform::_ct_vsnprintf
#define ct_snwprintf        CVLib::CVPlatform::_ct_snwprintf
#define ct_vsnwprintf		CVLib::CVPlatform::_ct_vsnwprintf
//typedef uint64  UID_t;
#define EMAX_MULTICAST_UIDS 512

/*
enum enumConnectFlags
{//connection flags!
    
    ESVR_CLOSE_CONNECTION_FLAG = 0x1000, //close connection
    ECLIENT_CLOSE_CONNECTION_FLAG = 0x2000,
    ENORMAL_MSG               = 0x0,               ///< ������ͨѶ
    ECONNECTION_ERR			= 0x1,		///< (enp ->) ����δ֪����    
    ECONNECTION_IDLECLOSE		= 0x4,		///< (enp ->) ��Ϊ��������enp�ر�

    ECONNECTION_ASKCLOSE		= 0x100,	///< (-> enp) Handle�ڷ�����ɺ�ر�
    ECONNECTION_BUSY   		= 0x200,	///< (-> enp) �ϲ㷱æ
    
    ECONNECTION_MSG_TOO_BIG    = 0x400,    ///< ��Ϣ̫��
};
*/
enum
{
    MYSQL_OP_SELECT = 1,
    MYSQL_OP_INSERT = 2,
    MYSQL_OP_REPLACE = 3,
    MYSQL_OP_UPDATE = 4,
    MYSQL_OP_DELETE = 5,
};

#endif //__CVLIB_BASEDEFINE_H__
