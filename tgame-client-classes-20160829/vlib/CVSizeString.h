/**
* 定长的字符串封装
* @author thinkry
*/

#ifndef __CVLIB_SIZESTRING_H__
#define __CVLIB_SIZESTRING_H__

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
//#include "CVBaseDefine.h"
//#include "CVObjReloadCheck.h"

namespace CVLib
{

/**
* 字符串、二进制对象用于编解码的接口类
*/
class ICodeRaw
{
public:
	virtual ~ICodeRaw() {}
    virtual char* GetBuffer() const = 0;
    virtual int GetBufferLen() const = 0;

    virtual int GetDataLen() const = 0;
    virtual void SetDataLen(int iLen) = 0;
};

/**
* 字符串接口
*/
class ISizeString: public ICodeRaw
{
public:
    virtual char* Get() const = 0;
    virtual int Size() const = 0;
    virtual int Set(const char* szFormat, ...) = 0;
    virtual int Cat(const char* szFormat, ...) = 0;

    bool operator == (const ISizeString& obj) const
    {
        return strcmp(Get(), obj.Get()) == 0;
    }
};

/**
* 定长的字符串封装
*/
template<int SIZE = 1024>
class CVSizeString: public ISizeString
{
public:
    CVSizeString()
    {
        //OBJ_CHECK_RELOAD();
        m_szBuf[0] = 0;
    }

    virtual ~CVSizeString()
    {
    }

    bool operator == (const char* pstr) const
    {
        return strcmp(Get(), pstr) == 0;
    }

    bool operator == (const ISizeString& obj) const
    {
        //CV_TRACE(("this:%s  obj:%s",Get(), obj.Get()));
        return strcmp(Get(), obj.Get()) == 0;
    }

    ISizeString& operator = (const ISizeString& obj)
    {
        Copy(obj);
        return *this;
    }

    char* operator ()() const
    {
        return Get();
    }

    const char& operator [] (int i) const
    {
        return m_szBuf[i];
    }

    char& operator [](int i)
    {
        return m_szBuf[i];
    }

    virtual char* GetBuffer() const
    {
        return Get();
    }

    virtual int GetBufferLen() const
    {
        return Size();
    }

    virtual int GetDataLen() const
    {
        return (int) strlen(Get());
    }

    virtual void SetDataLen(int iLen)
    {
        m_szBuf[iLen] = '\0';
    }

    virtual char* Get() const
    {
        return (char*)m_szBuf;
    }

    virtual int Size() const
    {
        return sizeof(m_szBuf);
    }

    virtual int Set(const char* szFormat, ...)
    {
        va_list ap;
        va_start(ap, szFormat);
        int iRet = VSet(szFormat, ap);
        va_end(ap);
        return iRet;
    }

    virtual int VSet(const char * szFormat, va_list ap)
    {
        if (!szFormat)
        {
            return -1;
        }

        int iRet = ct_vsnprintf(m_szBuf, Size(), szFormat, ap);
        if (iRet < 0)
        {
            return -1;
        }

        m_szBuf[iRet] = 0;
        return iRet;
    }

    virtual int Cat(const char* szFormat, ...)
    {
        va_list ap;
        va_start(ap, szFormat);
        int iRet = VCat(szFormat, ap);
        va_end(ap);
        return iRet;
    }

    virtual int VCat(const char* szFormat, va_list ap)
    {
        if (!szFormat)
        {
            return -1;
        }

        int iLen = (int)strlen(m_szBuf);
        int iMaxLen = Size() - iLen;
        if (iMaxLen <= 0)
        {
            return -1;
        }

        int iRet = ct_vsnprintf(m_szBuf + iLen, iMaxLen, szFormat, ap);
        if (iRet < 0)
        {
            return -1;
        }

        iRet += iLen;
        m_szBuf[iRet] = 0;
        return iRet;
    }

    int Copy(const ISizeString& stString)
    {
        return Set("%s", stString.Get());
    }

    //OBJ_CHECK_BEGIN(CVSizeString)
    //OBJ_CHECK_END()
private:
    char	m_szBuf[SIZE];
};

} // namespace

#endif // __CVLIB_SIZESTRING_H__
