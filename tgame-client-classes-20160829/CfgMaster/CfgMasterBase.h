#ifndef __CFG_MASTER_BASE_H__
#define __CFG_MASTER_BASE_H__

#include "vlib/base/CVLog.h"
#include "vlib/base/CVSingleton.h"
#include <fstream>
#include <string>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace CVLib;
USING_NS_CC;
USING_NS_CC_EXT;

#define __PRETTY_FUNCTION__ __FUNCTION__
#define ERET_OK 0
#define ERET_SYS_ERR -1
using namespace std;

class CmpByKey
{
public:
	bool operator() (int order1, int order2) const
	{
		return order1 < order2;
	}
};


template<class T>
class CCfgMasterBase
{
public:
    CCfgMasterBase(){}
    virtual ~CCfgMasterBase(){}
    
    int Init(const char* pstrConfigPath)
    {
        m_stCfg.Clear();
        Clear();
        int iRet = 0;

        /*
        std::string strFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(pstrConfigPath);
        fstream stInputFile(strFile.c_str(), ios::in | ios::binary);

        CCLog("=============%s!", strFile.c_str());
        if(!stInputFile.is_open())
        {
        	CCLog("%s, read config %s fail 1 !", __FUNCTION__, pstrConfigPath);
        	return -1;
        }
        */
        unsigned long iLen = 0;
        unsigned char* pFile = CCFileUtils::sharedFileUtils()->getFileData(pstrConfigPath, "rb", &iLen);
        //pFile[iLen] = '\0';    // json文件加此，cfg文件不需加 
        //iLen++;
        CCLog("=============cfg=%s,len=%lu", pstrConfigPath, iLen);


        /*
        ::google::protobuf::io::IstreamInputStream zero_copy_input(&stInputFile);
        if (! m_stCfg.ParseFromZeroCopyStream(&zero_copy_input) && stInputFile.eof())
        {
            CCLog("%s, read config %s fail 2!", __FUNCTION__, pstrConfigPath);
            return -1;
        }
        */
        //::google::protobuf::io::IstreamInputStream zero_copy_input(&stInputFile);
        if (! m_stCfg.ParseFromArray(pFile, iLen))
        {
            CCLog("%s, read config %s fail 2!", __FUNCTION__, pstrConfigPath);
            if(pFile) {
                delete [] pFile;
                pFile = NULL;
            }
            return -1;
        }
        if(pFile) {
            delete [] pFile;
            pFile = NULL;
        }

        /*
        if (!m_stCfg.ParseFromIstream(&stInputFile))
        {
        	CCLog("%s, read config %s fail!", __FUNCTION__, pstrConfigPath);
            return -1;
        }
        */
        iRet = ValidateCfg();
        if(0 != iRet)
        {
            CCLog("%s,  %s is invalidate cfg", __FUNCTION__, pstrConfigPath);
            return iRet;
        }

        iRet = SetCfg();
        if(0 != iRet)
        {
            CCLog("%s, %s is set config fail!", __FUNCTION__, pstrConfigPath);
            return iRet;
        }
        
        return iRet;
    }
    
    const T & GetCfg() const
    {
        return m_stCfg;
    }
protected:
    virtual int ValidateCfg()const {return 0;}
    virtual int SetCfg() {return 0;}
    // 如果子类有自己的数据，在这里实现清理自己数据的功能
    virtual void Clear() {}

protected:
    T m_stCfg;
};


#endif

