/**
* 用来屏蔽Windows和Linux一些基本函数的差异
* @author thinkry
*/

#ifndef __CVLIB_PLATFORM_H__
#define __CVLIB_PLATFORM_H__

#include "CVBaseDefine.h"
#include "CVSingleton.h"

namespace CVLib
{

/**
* CVPlatform类用来屏蔽Windows和Linux一些基本函数的差异
*/
class CVPlatform
{
public:
#ifdef WIN32
    /**
    * 实现linux的sleep
    * @param[in] iSecond sleep时长，单位秒
    */
    static void _ct_sleep(int iSecond)
    {
        Sleep(iSecond * 1000);
    }

    /**
    * 
    * 实现linux的usleep
    * @param[in] usec usleep时长，单位微秒
    */
    static void _ct_usleep(int usec)
    {
        Sleep(usec / 1000);
    }
#endif

    /**
    * Linux和Windows的vsnprintf函数的iCount和返回值处理有些不一样，这里统一
    * @return 同windows vsnprintf函数, 缓冲区不足时返回-1
    */
    static int _ct_vsnprintf(char* pszBuffer, int iCount, const char* pszFormat, va_list ap)
    {
#ifdef WIN32
        if (pszBuffer == NULL && iCount == 0)
        {
            return ::_vsnprintf(NULL, 0, pszFormat, ap);
        }

        int iRet = ::_vsnprintf(pszBuffer, iCount-1, pszFormat, ap);
#else
        int iRet = ::vsnprintf(pszBuffer, iCount, pszFormat, ap);
        if (pszBuffer == NULL && iCount == 0)
        {
            return iRet;
        }

        if (iRet >= iCount)
        {
            iRet = -1;
        }
#endif
        if (iRet > 0 && iRet < iCount)
        {
            pszBuffer[iRet] = 0;
        }

        va_end(ap);
        return iRet;
    }

    /**
    * Linux和Windows的vsnwprintf函数的iCount和返回值处理有些不一样，这里统一
    * @return 同windows vsnwprintf函数, 缓冲区不足时返回-1
    */
    static int _ct_vsnwprintf(wchar_t* pszBuffer, int iCount, const wchar_t* pszFormat, va_list ap)
    {
#ifdef WIN32
        if (pszBuffer == NULL && iCount == 0)
        {
            return ::_vsnwprintf(NULL, 0, pszFormat, ap);
        }

        int iRet = ::_vsnwprintf(pszBuffer, iCount-1, pszFormat, ap);
#else
        int iRet = ::vswprintf(pszBuffer, iCount, pszFormat, ap);
        if (pszBuffer == NULL && iCount == 0)
        {
            return iRet;
        }

        if (iRet >= iCount)
        {
            iRet = -1;
        }
#endif
        if (iRet > 0 && iRet < iCount)
        {
            pszBuffer[iRet] = 0;
        }

        va_end(ap);
        return iRet;
    }

    /**
    * Linux和Windows的snprintf函数的iCount和返回值处理有些不一样，这里统一
    * @return 同windows vsnprintf函数, 缓冲区不足时返回-1
    */
    static int _ct_snprintf(char* pszBuffer, int iCount, const char* pszFormat, ...)
    {
        va_list ap;
        va_start(ap, pszFormat);
        int iRet = _ct_vsnprintf(pszBuffer, iCount, pszFormat, ap);
        va_end(ap);
        return iRet;
    }

    /**
    * Linux和Windows的snwprintf函数的iCount和返回值处理有些不一样，这里统一
    * @return 同windows vsnprintf函数, 缓冲区不足时返回-1
    */
    static int _ct_snwprintf(wchar_t* pszBuffer, int iCount, const wchar_t* pszFormat, ...)
    {
        va_list ap;
        va_start(ap, pszFormat);
        int iRet = _ct_vsnwprintf(pszBuffer, iCount, pszFormat, ap);
        va_end(ap);
        return iRet;
    }

#ifdef WIN32
    /**
    * 打开文件.
    * 在Windows下用a/a+模式打开文件会报ERROR_ALREADY_EXISTS的错误，这里屏蔽一下
    */
    static FILE* _ct_fopen(const char* pszFileName, const char* pszMode)
    {
        DWORD dwRet = ::GetLastError();
        FILE* pstFile = fopen(pszFileName, pszMode);
        ::SetLastError(dwRet);
        return pstFile;
    }
#endif

    /**
    * 把wcs串转为mbs串，并用智能指针保证自动释放
    */
    static CVAutoCharA _ct_wcstombs(const wchar_t* psz)
    {
        CVAutoCharA autop;

        size_t iLen = wcstombs(NULL, psz, 0) + 1;
        if (iLen == (size_t)(-1))
        {
            return autop;
        }

        char* p = new char[iLen];
        if (p == NULL)
        {
            return autop;
        }

        wcstombs(p, psz, iLen);
        autop.reset(p);
        return autop;
    }
};

} // namespace CVLib

#endif //__CVLIB_PLATFORM_H__
