/**
* ��������Windows��LinuxһЩ���������Ĳ���
* @author thinkry
*/

#ifndef __CVLIB_PLATFORM_H__
#define __CVLIB_PLATFORM_H__

#include "CVBaseDefine.h"
#include "CVSingleton.h"

namespace CVLib
{

/**
* CVPlatform����������Windows��LinuxһЩ���������Ĳ���
*/
class CVPlatform
{
public:
#ifdef WIN32
    /**
    * ʵ��linux��sleep
    * @param[in] iSecond sleepʱ������λ��
    */
    static void _ct_sleep(int iSecond)
    {
        Sleep(iSecond * 1000);
    }

    /**
    * 
    * ʵ��linux��usleep
    * @param[in] usec usleepʱ������λ΢��
    */
    static void _ct_usleep(int usec)
    {
        Sleep(usec / 1000);
    }
#endif

    /**
    * Linux��Windows��vsnprintf������iCount�ͷ���ֵ������Щ��һ��������ͳһ
    * @return ͬwindows vsnprintf����, ����������ʱ����-1
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
    * Linux��Windows��vsnwprintf������iCount�ͷ���ֵ������Щ��һ��������ͳһ
    * @return ͬwindows vsnwprintf����, ����������ʱ����-1
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
    * Linux��Windows��snprintf������iCount�ͷ���ֵ������Щ��һ��������ͳһ
    * @return ͬwindows vsnprintf����, ����������ʱ����-1
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
    * Linux��Windows��snwprintf������iCount�ͷ���ֵ������Щ��һ��������ͳһ
    * @return ͬwindows vsnprintf����, ����������ʱ����-1
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
    * ���ļ�.
    * ��Windows����a/a+ģʽ���ļ��ᱨERROR_ALREADY_EXISTS�Ĵ�����������һ��
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
    * ��wcs��תΪmbs������������ָ�뱣֤�Զ��ͷ�
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
