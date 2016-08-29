/**
* Log类
* @author thinkry
*
* - 支持Windows/Linux
* - 支持单进程使用，也支持多进程互斥Log
* - 支持Named/Daily/Month
* - 支持不同日志类别Filter
* - 支持不同日志格式Format
* - 可动态改变Filter和Format
*
* @note 每次Log最多只能输出8K的记录，多出的会被截断
*/

#ifndef __CVLIB_LOGT_H__
#define __CVLIB_LOGT_H__

#include "CVBaseDefine.h"
#include "CVTime.h"
#include "CVStringUtils.h"
#include "cocos2d.h"
namespace CVLib
{

/** 日志类型 */
typedef enum ELogTypeTag
{
	ENamed = 1,	///< 指定文件名的日志，写满后会增加新文件 test.log, test1.log
	EDaily,	    ///< 以天为单位的日志, 20060307.log, 20060307-00.log
	EMonth,	    ///< 以月为单位的日志, 200603.log, 200603-01.log
    EHour,      ///< 以小时为单位的日志, 20090319_00.log, 20090319_00-01.log
} ELogType;

/** 日志级别 */
typedef enum ELogFilterTag
{
	EFatal 		= 0x1,
	EError 		= 0x2,
	EWarning 	= 0x4,
	EInfo 		= 0x8,
	EDebug 		= 0x10,
	ETrace 		= 0x20,
} ELogFilter;

/** 日志格式 */
typedef enum ELogFormatTag
{
	ETime 		= 0x1,		///< 在行首输出时间
	EProcessId 	= 0x2,		///< 在行首输出进程Id
    EThreadId 	= 0x4,		///< 在行首输出线程Id
	ENewLine 	= 0x8,		///< 在行末输出换行
	EType 		= 0x10,		///< 在行首输出日志级别
	EStdOut 	= 0x20,		///< 额外的在标准输出上输出
    EDebugOut   = 0x40,     ///< 在Windows平台下用DebugOutput输出
	EFileLine	= 0x80,		///< 输出所在m_pFile和m_uiLine
	ENoFile		= 0x100,	///< 不把日志写入文件
} ELogFormat;

#define DEFAULT_SUFFIX	".log"
#define DEFAULT_LOGNAME	"my.log"

/**
* Mutex 的RTTI类
*/
template<typename MUTEX> class CVMutexGuard
{
public:
	CVMutexGuard(MUTEX& st) : m_st(st)
	{
		m_st.Lock();
	}

	~CVMutexGuard()
	{
		m_st.Unlock();
	}

private:
    CVMutexGuard& operator= (const CVMutexGuard& obj) // 没有这个会在VC2005下有4512告警
    {
        return *this;
    }

private:
	MUTEX&	m_st;
};

/**
* 空互斥类，用于单线程单进程的场景
*/
class CVNullMutex
{
public:
    CVNullMutex& operator= (const CVNullMutex& CV_UNUSED(obj)) { return *this; }   // 没有这个会在VC2005下有4512告警

	int Lock() { return 0; }
	int Unlock() { return 0; }
};

/**
* 日志类.
* 模板参数MUTEX是采用什么互斥机制，默认是不互斥\n
* MUTEX对象CTLogT.m_stLock需要在外部初始化
*/
template<typename MUTEX = CVNullMutex> class CVLogT
{
private:
    enum
    {
        CV_LINE_MAX = 8192,     ///< 一次最多能输出的日志长度，多出部分会截断
        CV_DEFAULT_LOGSIZE = 16 * 1024 * 1024,  ///< 默认日志文件是16M
        CV_DEFAULT_LOGNUM = 5,  ///< 默认日志文件是5个
    };

    ELogType 		m_eLogType;
    bool            m_bIsPrintCocosLog; //neilwu added
    unsigned int 	m_uiFilter;
    unsigned int 	m_uiFormat;

    int 	        m_iMaxLogSize;
    int	            m_iMaxLogNum;

    char           m_szLogPath[CV_PATH_MAX];
    char           m_szLogSuffix[20];
    char           m_szLine[CV_LINE_MAX];

public:
    MUTEX           m_stLock;
    unsigned int    m_uiLine;
    const char*		m_pFile;

public:
    CVLogT()
    {
        m_eLogType = ENamed;
        m_uiFilter = EFatal|EError|EWarning|EInfo|EDebug|ETrace;
        m_uiFormat = ETime|ENewLine|EType;
        m_iMaxLogSize = CV_DEFAULT_LOGSIZE;
        m_iMaxLogNum = CV_DEFAULT_LOGNUM;
        ct_snprintf(CV_STRSIZE(m_szLogPath), "%s", DEFAULT_LOGNAME);
        m_szLogSuffix[0] = 0;
        m_szLine[0] = 0;
        m_uiLine = 0;
        m_pFile = 0;
    }

    virtual ~CVLogT()
    {
    }

public:
    void Init(ELogType eLogType = ENamed, 
        const char* szLogPath = NULL, 
        const char* szLogName = NULL, 
        int iMaxLogSize = CV_DEFAULT_LOGSIZE, 
        int iMaxLogNum = CV_DEFAULT_LOGNUM,
        bool bIsPrintCocosLog = true)
    {
        m_eLogType = eLogType;
        m_bIsPrintCocosLog = bIsPrintCocosLog;
        string str = "";
        if (szLogPath)
        {
            str = szLogPath;
            CVStringUtils::TrimLeft(str, " \t\r\n");
            CVStringUtils::TrimRight(str, " \t\r\n");

            // 确保路径最后是\或/
            if (str.size() > 0 && str[str.size() - 1] != '/' && str[str.size() - 1] != '\\')
            {
                size_t idx = str.find_first_of("/\\");
                if (idx != string::npos)
                {
                    str += str[idx];
                }
            }
        }

        if (m_eLogType == ENamed)
        {
            m_iMaxLogSize = iMaxLogSize;
            m_iMaxLogNum  = iMaxLogNum;
            ct_snprintf(CV_STRSIZE(m_szLogPath), "%s%s", 
                str.c_str(), (szLogName == NULL ? DEFAULT_LOGNAME : szLogName));
        }
        else
        {
            // m_szLogSuffix 是日志文件名的附加后缀
            // 例如每日的帐单文件可能有多个，用20060307-0.log, 20060307-1.log的方式区分
            // 这里的 -0, -1 这样的后缀就是 m_szLogSuffix
            ct_snprintf(CV_STRSIZE(m_szLogPath), "%s", str.c_str());
            ct_snprintf(CV_STRSIZE(m_szLogSuffix), "%s", (szLogName == NULL ? DEFAULT_SUFFIX : szLogName));
        }
    }

#define DO_LOG(filter, format) \
    if (!(m_uiFilter & filter)) \
    { \
        return 0; \
    } \
    va_list ap; \
    va_start(ap, format); \
    int iRet = VLog(filter, format, ap); \
    va_end(ap); \
    return iRet

    int Log(ELogFilter filter, const char * format, ...) { DO_LOG(filter, format); }
    int Log(const char* format, ...) { DO_LOG(EInfo, format); }

    /**
    * 不同级别的Log
    */
    int LogFatal(const char * format, ...) { DO_LOG(EFatal, format); }
    int LogError(const char * format, ...) { DO_LOG(EError, format); }
    int LogWarning(const char * format, ...) { DO_LOG(EWarning, format); }
    int LogInfo(const char * format, ...) { DO_LOG(EInfo, format); }
    int LogDebug(const char * format, ...) { DO_LOG(EDebug, format); }
    int LogTrace(const char * format, ...) { DO_LOG(ETrace, format); }
    int LogTraceBinary(const char * buf, int len)
    {
        if (!(ETrace & m_uiFilter))
        {
            return 0;
        }
        int iRet = LogBinary(ETrace, buf, len);
        return iRet;
    }

    int LogWarnBinary(const char * buf, int len)
    {
        if (!(ETrace & m_uiFilter))
        {
            return 0;
        }
        int iRet = LogBinary(EWarning, buf, len);
        return iRet;
    }
    /**
    * Log二进制
    */
    int LogBinary(ELogFilter filter, const char * buf, int len)
    {
        if (!buf || !len || (!(m_uiFilter & filter)))
        {
            return 0;
        }

        unsigned int uiOldFormat = GetFormat();
        RemoveFormat(ETime);
        RemoveFormat(EProcessId);
        RemoveFormat(EThreadId);
        RemoveFormat(ENewLine);
        RemoveFormat(EType);
        RemoveFormat(EFileLine);
        for (int i = 0; i < len; ++i)
        {
            if (!(i % 16))
            {
                if (i != 0)
                {
                    Log(filter, "\n");
                }
                Log(filter, "%04d>    ", i / 16 + 1);
            }
            Log(filter, "%02X ", (unsigned char)buf[i]);
        }

        Log(filter, "\n");
        SetFormat(uiOldFormat);

        return 0;
    }

    /**
    * 真正执行log的函数
    */
    int VLog(ELogFilter filter, const char *szFormat, va_list ap)
    {
        if ((!(m_uiFilter & filter)) || !szFormat)
        {
            return 0;
        }

        // 多线程互斥
        CVMutexGuard<MUTEX> stGuard(m_stLock);

        CVTime stNow(time(0));

        // 计算Log的文件名
        char szTimeBuf[32] = {0};
        char szFilePath[CV_PATH_MAX];
        if (m_eLogType == ENamed)
        {
            ct_snprintf(CV_STRSIZE(szFilePath), "%s", m_szLogPath);
        }
        else
        {
            switch (m_eLogType)
            {
            case EDaily:
                stNow.Format(CV_STRSIZE(szTimeBuf), "%Y%m%d");
                break;
            case EMonth:
                stNow.Format(CV_STRSIZE(szTimeBuf), "%Y%m");
                break;
            case EHour:
                stNow.Format(CV_STRSIZE(szTimeBuf), "%Y%m%d_%H");
                break;
            default:
                stNow.Format(CV_STRSIZE(szTimeBuf), "%Y%m%d");
                break;
            }
            
            if (m_szLogPath[0] != 0)
            {
                ct_snprintf(CV_STRSIZE(szFilePath), "%s%s%s", m_szLogPath, szTimeBuf, m_szLogSuffix);
            }
            else
            {
                ct_snprintf(CV_STRSIZE(szFilePath), "%s%s", szTimeBuf, m_szLogSuffix);
            }
        }

        // 准备好要输出的内容
        char* pLine = m_szLine;
        int iSize = sizeof(m_szLine) - 1;
        int iRet = 0;
        m_szLine[0] = 0;

        if (m_uiFormat & ETime)
        {
            stNow.Format(CV_STRSIZE(szTimeBuf), "%Y-%m-%d %H:%M:%S");
            iRet = ct_snprintf(pLine, iSize, "[%s]", szTimeBuf);
            pLine += iRet;
            iSize -= iRet;
        }

        if (m_uiFormat & EProcessId)
        {
            iRet = ct_snprintf(pLine, iSize, "[%d]", static_cast<int>(GetNowProcessId()));
            pLine += iRet;
            iSize -= iRet;
        }

        if (m_uiFormat & EThreadId)
        {
            //iRet = ct_snprintf(pLine, iSize, "[%d]", static_cast<int>(GetCurrentThreadId())); //note: 为了避免在静态编译时包含pthread库，此处先注销，改用ProcessID
            iRet = ct_snprintf(pLine, iSize, "[%d]", static_cast<int>(GetNowProcessId()));
            pLine += iRet;
            iSize -= iRet;
        }

        if (m_uiFormat & EType)
        {
            iRet = ct_snprintf(pLine, iSize, "[%s]", GetFilterDesc(filter));
            pLine += iRet;
            iSize -= iRet;
        }

        iRet = ct_vsnprintf(pLine, iSize, szFormat, ap);
        pLine += iRet;
        iSize -= iRet;

        if (m_uiFormat & EFileLine)
        {
            iRet = ct_snprintf(pLine, iSize, " (%s:%d)", m_pFile, m_uiLine);
            pLine += iRet;
            iSize -= iRet;
        }

        if (m_bIsPrintCocosLog)
        {
            cocos2d::CCLog("%s", m_szLine); //2014-3-15,neilwu,mod for cocos2dx
        }
        
        
        if (m_uiFormat & ENewLine)
        {
            ct_snprintf(pLine, iSize, "\n");
            pLine += iRet;
            iSize += iRet;
        }
        /*
        //2014-4-24,neilwu comment,log file in android assets may invalid
        // 向目标输出
        if (!(m_uiFormat & ENoFile))
        {
#ifndef WIN32
            mode_t iMode = umask(0);
            FILE* pstFile = ct_fopen(szFilePath, "a+");
            umask(iMode);
#else
            FILE* pstFile = ct_fopen(szFilePath, "a+");
#endif
            if (pstFile)
            {
                fprintf(pstFile, "%s", m_szLine);
                fflush(pstFile);
                fclose(pstFile);

                if (m_eLogType == ENamed)
                {
                    ShiftFiles(szFilePath, m_iMaxLogSize, m_iMaxLogNum);
                }
                else
                {
                    ShiftFiles(szFilePath, 0x7FFFFFFF, 0x7FFFFFFF);
                }
            }
        }
        */
        return 0;
    }

	/**
	* 回滚日志文件
	* @param pszFilePath 要回滚的日志文件
	* @param iMaxLogSize 日志文件的最大大小，超过这个值就要回滚日志文件
	* @param iMaxLogNum 日志文件的最大个数，超过这个值的日志文件就丢弃
	* @return 返回操作的结果，0表示成功
	*/
    int ShiftFiles(const char* pszFilePath, int iMaxLogSize, int iMaxLogNum)
    {
#ifdef WIN32
        struct _stat32 stStat;
#else
        struct stat stStat;
#endif
        if (ct_stat(pszFilePath, &stStat) < 0)
        {
            return -3;	// 获取日志文件信息失败
        }

        if (stStat.st_size < iMaxLogSize)
        {
            return 0;
        }

        char szTempBuf[CV_PATH_MAX];
        char szNewFile[CV_PATH_MAX];

        ct_snprintf(CV_STRSIZE(szTempBuf), "%s.%d", pszFilePath, iMaxLogNum-1);
        if (ct_access(szTempBuf, 0) == 0)
        {
            if (ct_remove(szTempBuf) < 0)
            {
                return -4;	// 删除最后一个日志文件失败
            }
        }

        for (int i = iMaxLogNum - 2; i >= 0; --i)
        {
            if (i == 0)
            {
                ct_snprintf(CV_STRSIZE(szTempBuf), "%s", pszFilePath);
            }
            else
            {
                ct_snprintf(CV_STRSIZE(szTempBuf), "%s.%d", pszFilePath, i);
            }

            if (ct_access(szTempBuf, 0) == 0)
            {
                ct_snprintf(CV_STRSIZE(szNewFile), "%s.%d", pszFilePath, i+1);
                if (ct_rename(szTempBuf, szNewFile) < 0)
                {
                    return -5;	// 重命名日志文件失败
                }
            }
        }

        return 0;
    }

    static const char* GetFilterDesc(unsigned int iFilter)
    {
        // 两种方式获取, 这里选择第一种
#if 1
        switch (iFilter)
        {
            case ETrace: return "trace";
            case EDebug: return "debug";
            case EWarning: return "warn";
            case EInfo: return "info";
            case EError: return "error";
            case EFatal: return "fatal";
            default: return "";
        }

#else
        static int aiDesc[] =
        {
            0,  1,  2,  0,  3,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,
            5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };

        static const char aszDesc[][8] =
        {
            "",
            "fatal",
            "error",
            "warn",
            "info",
            "debug",
            "trace"
        };

        return aszDesc[aiDesc[iFilter]];
#endif
    }

    /** 消息等级相关函数 */
    void SetFilter(unsigned int uiFilter) { m_uiFilter = uiFilter; }
    unsigned int GetFilter() const { return m_uiFilter; }
    
	void AddFilter(ELogFilter filter) { m_uiFilter |= filter; }
    void RemoveFilter(ELogFilter filter) { m_uiFilter &= ~filter; }

    void SetFilterByStr(const char* szFilter)
    {
        if (!szFilter)
        {
            return;
        }
        m_uiFilter = 0;

        string str = "|" + string(szFilter) + "|";
        CVStringUtils::MakeLower(str);
        CVStringUtils::RemoveNot(str, "abcdefghijklmnopqrstuvwxyz|");

        if (str.find("|fatal|") != string::npos) { AddFilter(EFatal); }
        if (str.find("|error|") != string::npos) { AddFilter(EError); }
        if (str.find("|warning|") != string::npos) { AddFilter(EWarning); }
        if (str.find("|info|") != string::npos) { AddFilter(EInfo); }
        if (str.find("|debug|") != string::npos) { AddFilter(EDebug); }
        if (str.find("|trace|") != string::npos) { AddFilter(ETrace); }
    }

    /** 消息格式相关函数 */
    void SetFormat(unsigned int uiFormat) { m_uiFormat = uiFormat; }
    unsigned int GetFormat() const { return m_uiFormat; }

    void AddFormat(ELogFormat format) { m_uiFormat |= format; }
    void RemoveFormat(ELogFormat format)  { m_uiFormat &= ~format; }

    void SetFormatByStr(const char* szFormat)
    {
        if (!szFormat)
        {
            return;
        }
        m_uiFormat = 0;

        string str = "|" + string(szFormat) + "|";
        CVStringUtils::MakeLower(str);
        CVStringUtils::RemoveNot(str, "abcdefghijklmnopqrstuvwxyz|");

        if (str.find("|time|") != string::npos) { AddFormat(ETime); }
        if (str.find("|pid|") != string::npos) { AddFormat(EProcessId); }
        if (str.find("|tid|") != string::npos) { AddFormat(EThreadId); }
        if (str.find("|newline|") != string::npos) { AddFormat(ENewLine); }
        if (str.find("|type|") != string::npos) { AddFormat(EType); }
        if (str.find("|stdout|") != string::npos) { AddFormat(EStdOut); }
        if (str.find("|debugout|") != string::npos) { AddFormat(EDebugOut); }
        if (str.find("|fileline|") != string::npos) { AddFormat(EFileLine); }
        if (str.find("|nofile|") != string::npos) { AddFormat(ENoFile); }
    }
};

typedef CVLogT<CVNullMutex> CVLog;

} //namespace CVLib

// 全局日志
#define CV_NLOG		(CVLib::CVSingleton<CVLib::CVLog>::Instance())
#define CV_ELOG		(CVLib::CVSingleton<CVLib::CVLog, 1>::Instance())

#define CV_LOG(x) do { \
    if (CV_NLOG->GetFormat() & CVLib::EFileLine) { \
    CV_NLOG->m_pFile = __FILE__; \
    CV_NLOG->m_uiLine = __LINE__; } \
    CV_NLOG->Log x; } while(0)

#define CV_WARNING(x) do { \
    if (CV_NLOG->GetFormat() & CVLib::EFileLine) { \
    CV_NLOG->m_pFile = __FILE__; \
    CV_NLOG->m_uiLine = __LINE__; } \
    CV_NLOG->LogWarning x; } while(0)

#define CV_INFO(x) do { \
    if (CV_NLOG->GetFormat() & CVLib::EFileLine) { \
    CV_NLOG->m_pFile = __FILE__; \
    CV_NLOG->m_uiLine = __LINE__; } \
    CV_NLOG->LogInfo x; } while(0)

// 建议使用CV_TRACE, 不用CV_DEBUG
#define CV_DEBUG(x) do { \
    if (CV_NLOG->GetFormat() & CVLib::EFileLine) { \
    CV_NLOG->m_pFile = __FILE__; \
    CV_NLOG->m_uiLine = __LINE__; } \
    CV_NLOG->LogDebug x; } while(0)

#define CV_TRACE(x) do { \
    if (CV_NLOG->GetFormat() & CVLib::EFileLine) { \
    CV_NLOG->m_pFile = __FILE__; \
    CV_NLOG->m_uiLine = __LINE__; } \
    CV_NLOG->LogTrace x; } while(0)

#define CV_TRACEBINARY(x) do { \
    if (CV_NLOG->GetFormat() & CVLib::EFileLine) { \
    CV_NLOG->m_pFile = __FILE__; \
    CV_NLOG->m_uiLine = __LINE__; } \
    CV_NLOG->LogTraceBinary x; } while(0)

#define CV_WARNBINARY(x) do { \
    if (CV_NLOG->GetFormat() & CVLib::EFileLine) { \
    CV_NLOG->m_pFile = __FILE__; \
    CV_NLOG->m_uiLine = __LINE__; } \
    CV_NLOG->LogWarnBinary x; } while(0)

#define CV_FATAL(x) do { \
    if (CV_ELOG->GetFormat() & CVLib::EFileLine) { \
    CV_ELOG->m_pFile = __FILE__; \
    CV_ELOG->m_uiLine = __LINE__; } \
    CV_ELOG->LogFatal x; \
    if (CV_NLOG->GetFormat() & CVLib::EFileLine) { \
    CV_NLOG->m_pFile = __FILE__; \
    CV_NLOG->m_uiLine = __LINE__; } \
    CV_NLOG->LogFatal x; } while(0)

#define CV_ERROR(x) do { \
    if (CV_ELOG->GetFormat() & CVLib::EFileLine) { \
    CV_ELOG->m_pFile = __FILE__; \
    CV_ELOG->m_uiLine = __LINE__; } \
    CV_ELOG->LogError x; \
    if (CV_NLOG->GetFormat() & CVLib::EFileLine) { \
    CV_NLOG->m_pFile = __FILE__; \
    CV_NLOG->m_uiLine = __LINE__; } \
    CV_NLOG->LogError x; } while(0)


#endif //__CVLIB_LOGT_H__
