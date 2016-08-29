#ifndef __UpdateMgrImp_H_
#define __UpdateMgrImp_H_

#include "UpdateMgr.h"


class UpdateMgrImp : public UpdateMgr {
public:
    UpdateMgrImp();

    void updateSearchPath();
    void setResUrl(const char* pszUrl);
protected:
    virtual std::string GetPkgUrl(const char * pkgName);
    virtual std::string GetTempDownloadPath(const char * pkgName);
    virtual std::string GetExtractPath();
    virtual int GetCurrentVersion();
    virtual void UpdateVersion(int version);
    virtual bool IsApkNewest();
    virtual void delResDataDir();

    int getJsonVer(const char* strFilePath);
private:
    std::string m_writableFolder;
    bool m_bCanUpdateSearchPath; //�Ƿ���Ը�������·��(Ĭ��true,���apk�汾���ߣ���Ϊfalse)

    std::string m_strResUrl;
};

#endif //__UpdateMgrImp_H_
