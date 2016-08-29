/**
* 单件类
* @author thinkry
*/

#ifndef __CVLIB_SINGLETON_H__
#define __CVLIB_SINGLETON_H__

#include <stdlib.h>
//#include <malloc.h>
namespace CVLib
{

/**
* 一个非常简单的智能指针
*/
template<typename T>
class CVAutoPtr
{
public:
	CVAutoPtr() : m_auto_ptr(0) {}
    CVAutoPtr(T* ptr) : m_auto_ptr(ptr) {}
    CVAutoPtr(const CVAutoPtr& obj) : m_auto_ptr(obj.get()) {}
	~CVAutoPtr() { reset (0); }

    CVAutoPtr& operator= (const CVAutoPtr& obj)
    {
        this->m_auto_ptr = obj.get();
        return *this;
    }

	T* get() const { return m_auto_ptr; }
    T* operator() () { return get(); }

	void reset(T* ptr)
    {
        if (m_auto_ptr == ptr)
        {
            return;
        }

        if (m_auto_ptr)
        {
            delete m_auto_ptr;
        }

        m_auto_ptr = ptr;
    }

private:
	T* m_auto_ptr;
};

/**
* 一个非常简单的用于数组的智能指针
*/
template<typename T>
class CVAutoArray
{
public:
    CVAutoArray() : m_auto_ptr(0) {}
    CVAutoArray(T* ptr) : m_auto_ptr(ptr) {}
    CVAutoArray(const CVAutoArray<T>& obj) : m_auto_ptr(obj.m_auto_ptr) {}
    ~CVAutoArray() { reset (0); }

    CVAutoArray& operator= (const CVAutoArray<T>& obj)
    {
        m_auto_ptr = obj.get();
        return *this;
    }

    T* get() const { return m_auto_ptr; }
    T* operator()() const { return get(); }

    void reset(T* ptr)
    {
        if (m_auto_ptr == ptr)
        {
            return;
        }

        if (m_auto_ptr)
        {
            delete [] m_auto_ptr;
        }

        m_auto_ptr = ptr;
    }

private:
    T* m_auto_ptr;
};

/**
* 一个非常简单的智能指针(free)
*/
class CVAutoFree
{
public:
    CVAutoFree() : m_auto_ptr(0) {}
    CVAutoFree(void* ptr) : m_auto_ptr(ptr) {}
    CVAutoFree(const CVAutoFree& obj) : m_auto_ptr(obj.get()) {}
    ~CVAutoFree() { reset (0); }

    CVAutoFree& operator= (const CVAutoFree& obj)
    {
        this->m_auto_ptr = obj.get();
        return *this;
    }

    void* get() const { return m_auto_ptr; }
    void* operator() () { return get(); }

    void reset(void* ptr)
    {
        if (m_auto_ptr == ptr)
        {
            return;
        }

        if (m_auto_ptr)
        {
            free(m_auto_ptr);
        }

        m_auto_ptr = ptr;
    }

private:
    void* m_auto_ptr;
};

/**
* 字符串经常需要自动释放，用这两个智能指针能解决
*/
typedef CVAutoArray<char>         CVAutoCharA;
typedef CVAutoArray<wchar_t>      CVAutoCharW;

#ifdef _UNICODE
    typedef CVAutoCharW CVAutoChar;
#else
    typedef CVAutoCharA CVAutoChar;
#endif

class CVNonCopyable
{
public:
    CVNonCopyable()
    {

    }
private:
    CVNonCopyable(const CVNonCopyable&);
    const CVNonCopyable & operator=(const CVNonCopyable&);
};

template<typename T>
class CVScopePtr : public CVNonCopyable
{
public:
    CVScopePtr(T * pstPtr = NULL):
    m_pstPtr(pstPtr)
    {

    }

    ~CVScopePtr()
    {
        if (m_pstPtr != NULL)
        {
            delete m_pstPtr;
        }
    }

    T * GetPtr() const
    {
        return m_pstPtr;
    }

    T * GiveUp()
    {
        T * pstTmp = m_pstPtr;
        m_pstPtr = NULL;     
        return pstTmp;
    }

    void Swap(CVScopePtr & stOther)
    {
        if (this == &stOther)
        {
            return;
        }
        T * pstTmp = this->m_pstPtr;
        this->SetPtr(stOther.GetPtr());
        stOther.SetPtr(pstTmp);
    }
private:
    void SetPtr(T * pstPtr)
    {
        m_pstPtr = pstPtr;
    }
private:
    T * m_pstPtr;
};

/**
* 单件类, X可以让单件类创建多个
*/
template<typename T, int X = 0> class CVSingleton 
{
private:
	CVSingleton() {}
	CVSingleton(const CVSingleton&) {}
	~CVSingleton() {}
	CVSingleton& operator= (const CVSingleton &) { return *this; } 

public:
	static T* Instance()
	{
		static CVAutoPtr<T> autoptr;
 		if(autoptr.get() == 0)
		{
 			autoptr.reset(new T);
		}
 		return autoptr.get();
	}
};

} // namespace CVLib

#endif //__CVLIB_SINGLETON_H__
