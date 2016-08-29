#ifndef __TEXT_CFG_MASTER_H__
#define __TEXT_CFG_MASTER_H__

#include "CfgMasterBase.h"
#include "TextCfg.pb.h"

using namespace std;
using namespace com::cfg::vo;

class CTextCfgMaster : public CCfgMasterBase<TextCfgSet>
{
public:

    CTextCfgMaster(){}
    ~CTextCfgMaster(){}

    //std::string GetStr(const int iKey);
    const char* GetCstr(const int iKey); //不需要拼接字符串的话，优先用这个函数，没有字符串拷贝

protected:
    
    virtual int ValidateCfg()const;

    virtual int SetCfg();
    virtual void Clear() {
        m_stMap.clear();
    }
private:
    map<int, string> m_stMap;
};

#define TEXT_CFG_MASTER (CVSingleton<CTextCfgMaster>::Instance())

#endif

