/**
* 异常类定义.
* @author thinkry
*/

#ifndef __CVLIB_EXCEPTION_H__
#define __CVLIB_EXCEPTION_H__

#include "base/CVBaseDefine.h"
#include "base/CVStringUtils.h"
#ifndef WIN32
    #include <execinfo.h>
#endif

#ifdef CV_NO_EXCEPTION
    #define CV_THROW(x)
#else
    #define CV_THROW(x) throw CVLib::CVException x
#endif

/**
*  CVLib命名空间
*/
namespace CVLib
{

class CVException
{
public:
    /**
    * 构造/析构
    */
    CVException() : m_iErrorCode(-1)
    {
        DumpBackTrace();
    }

    CVException(int iErrorCode) : m_iErrorCode(iErrorCode)
    {
        DumpBackTrace();
    }

    CVException(const char* pszFormat, ...) : m_iErrorCode(-1)
    {
        va_list ap;
        va_start(ap, pszFormat);
        CVStringUtils::FormatV(m_strErrorMsg, pszFormat, ap);
        va_end(ap);

        DumpBackTrace();
    }

	CVException(int iErrorCode, const char* pszFormat, ...)
	{
		m_iErrorCode = iErrorCode;

		va_list ap;
		va_start(ap, pszFormat);
		CVStringUtils::FormatV(m_strErrorMsg, pszFormat, ap);
		va_end(ap);

        DumpBackTrace();
	}

    virtual ~CVException()
    {
    }

    /**
    * Get系列
    */
	int GetErrorCode() const
	{
		return m_iErrorCode;
	}

	const char* GetErrorMsg() const
	{
		return m_strErrorMsg.c_str();
	}

	const string GetErrorString() const
	{
		return m_strErrorMsg;
	}

    /** 和std::exception一致 */
    virtual const char* what() const
    {
        return GetErrorMsg();
    }

    /**
    * Set系列
    * 感觉没必要Set, 注释掉 thinkry 2008.05.26
    */
    /*void Set(int iErrorCode, const char* pszFormat, ...)
    {
        m_iErrorCode = iErrorCode;

        va_list ap;
        va_start(ap, pszFormat);
        CVStringUtils::FormatV(m_strErrorMsg, pszFormat, ap);
        va_end(ap);
    }

    void SetErrorCode(int iErrorCode)
    {
        m_iErrorCode = iErrorCode;
    }

    void SetErrorMsg(const char* pszErrorMsg)
    {
        m_strErrorMsg = pszErrorMsg;
    }

    void SetErrorMsg(int iErrorCode, const char* pszFormat, ...)
    {
        m_iErrorCode = iErrorCode;

        va_list ap;
        va_start(ap, pszFormat);
        CVStringUtils::FormatV(m_strErrorMsg, pszFormat, ap);
        va_end(ap);
    }*/

protected:
    void DumpBackTrace()
    {
        // 编译时如果不加-rdynamic参数, backtrace_symbols只会返回地址信息
        // 可以通过addr2line工具查到地址对应的文件行号
#ifndef WIN32
        void* array[20];
        int iCount = backtrace(array, CV_COUNTOF(array));
        char** symbols = backtrace_symbols(array, iCount);
        CVAutoFree autop(symbols);
        string sBackTrace = "StackTrace:\n";
        for (int i = 1; i < iCount; ++i)  // 第1个是DumpBackTrace, 忽略
        {
            sBackTrace += "    ";
            sBackTrace += symbols[i];
            sBackTrace += "\n";
        }

        CV_ERROR(("Exception: ErrorCode=%d Msg=%s", m_iErrorCode, m_strErrorMsg.c_str()));
        CV_ERROR(("%s", sBackTrace.c_str()));
#endif
    }

protected:
	int			m_iErrorCode;
	string	    m_strErrorMsg;
};

// 异常类由于有堆栈跟踪, 不适用作为错误值类
// 注释掉 thinkry 2008.05.26
//typedef CVException CVError;

} // namespace CVLib

#endif //__CVLIB_EXCEPTION_H__
