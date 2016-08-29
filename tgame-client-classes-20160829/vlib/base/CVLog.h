/**
* Log��
* @author thinkry
*
* - ֧��Windows/Linux
* - ֧�ֵ�����ʹ�ã�Ҳ֧�ֶ���̻���Log
* - ֧��Named/Daily/Month
* - ֧�ֲ�ͬ��־���Filter
* - ֧�ֲ�ͬ��־��ʽFormat
* - �ɶ�̬�ı�Filter��Format
*
* @note ÿ��Log���ֻ�����8K�ļ�¼������Ļᱻ�ض�
*/

#ifndef __CVLIB_LOGT_H__
#define __CVLIB_LOGT_H__

#include "CVBaseDefine.h"
#include "CVTime.h"
#include "CVStringUtils.h"
#include "cocos2d.h"
namespace CVLib
{

/** ��־���� */
typedef enum ELogTypeTag
{
	ENamed = 1,	///< ָ���ļ�������־��д������������ļ� test.log, test1.log
	EDaily,	    ///< ����Ϊ��λ����־, 20060307.log, 20060307-00.log
	EMonth,	    ///< ����Ϊ��λ����־, 200603.log, 200603-01.log
    EHour,      ///< ��СʱΪ��λ����־, 20090319_00.log, 20090319_00-01.log
} ELogType;

/** ��־���� */
typedef enum ELogFilterTag
{
	EFatal 		= 0x1,
	EError 		= 0x2,
	EWarning 	= 0x4,
	EInfo 		= 0x8,
	EDebug 		= 0x10,
	ETrace 		= 0x20,
} ELogFilter;

/** ��־��ʽ */
typedef enum ELogFormatTag
{
	ETime 		= 0x1,		///< ���������ʱ��
	EProcessId 	= 0x2,		///< �������������Id
    EThreadId 	= 0x4,		///< ����������߳�Id
	ENewLine 	= 0x8,		///< ����ĩ�������
	EType 		= 0x10,		///< �����������־����
	EStdOut 	= 0x20,		///< ������ڱ�׼��������
    EDebugOut   = 0x40,     ///< ��Windowsƽ̨����DebugOutput���
	EFileLine	= 0x80,		///< �������m_pFile��m_uiLine
	ENoFile		= 0x100,	///< ������־д���ļ�
} ELogFormat;

#define DEFAULT_SUFFIX	".log"
#define DEFAULT_LOGNAME	"my.log"

/**
* Mutex ��RTTI��
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
    CVMutexGuard& operator= (const CVMutexGuard& obj) // û���������VC2005����4512�澯
    {
        return *this;
    }

private:
	MUTEX&	m_st;
};

/**
* �ջ����࣬���ڵ��̵߳����̵ĳ���
*/
class CVNullMutex
{
public:
    CVNullMutex& operator= (const CVNullMutex& CV_UNUSED(obj)) { return *this; }   // û���������VC2005����4512�澯

	int Lock() { return 0; }
	int Unlock() { return 0; }
};

/**
* ��־��.
* ģ�����MUTEX�ǲ���ʲô������ƣ�Ĭ���ǲ�����\n
* MUTEX����CTLogT.m_stLock��Ҫ���ⲿ��ʼ��
*/
template<typename MUTEX = CVNullMutex> class CVLogT
{
private:
    enum
    {
        CV_LINE_MAX = 8192,     ///< һ��������������־���ȣ�������ֻ�ض�
        CV_DEFAULT_LOGSIZE = 16 * 1024 * 1024,  ///< Ĭ����־�ļ���16M
        CV_DEFAULT_LOGNUM = 5,  ///< Ĭ����־�ļ���5��
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

            // ȷ��·�������\��/
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
            // m_szLogSuffix ����־�ļ����ĸ��Ӻ�׺
            // ����ÿ�յ��ʵ��ļ������ж������20060307-0.log, 20060307-1.log�ķ�ʽ����
            // ����� -0, -1 �����ĺ�׺���� m_szLogSuffix
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
    * ��ͬ�����Log
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
    * Log������
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
    * ����ִ��log�ĺ���
    */
    int VLog(ELogFilter filter, const char *szFormat, va_list ap)
    {
        if ((!(m_uiFilter & filter)) || !szFormat)
        {
            return 0;
        }

        // ���̻߳���
        CVMutexGuard<MUTEX> stGuard(m_stLock);

        CVTime stNow(time(0));

        // ����Log���ļ���
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

        // ׼����Ҫ���������
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
            //iRet = ct_snprintf(pLine, iSize, "[%d]", static_cast<int>(GetCurrentThreadId())); //note: Ϊ�˱����ھ�̬����ʱ����pthread�⣬�˴���ע��������ProcessID
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
        // ��Ŀ�����
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
	* �ع���־�ļ�
	* @param pszFilePath Ҫ�ع�����־�ļ�
	* @param iMaxLogSize ��־�ļ�������С���������ֵ��Ҫ�ع���־�ļ�
	* @param iMaxLogNum ��־�ļ������������������ֵ����־�ļ��Ͷ���
	* @return ���ز����Ľ����0��ʾ�ɹ�
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
            return -3;	// ��ȡ��־�ļ���Ϣʧ��
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
                return -4;	// ɾ�����һ����־�ļ�ʧ��
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
                    return -5;	// ��������־�ļ�ʧ��
                }
            }
        }

        return 0;
    }

    static const char* GetFilterDesc(unsigned int iFilter)
    {
        // ���ַ�ʽ��ȡ, ����ѡ���һ��
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

    /** ��Ϣ�ȼ���غ��� */
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

    /** ��Ϣ��ʽ��غ��� */
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

// ȫ����־
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

// ����ʹ��CV_TRACE, ����CV_DEBUG
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
