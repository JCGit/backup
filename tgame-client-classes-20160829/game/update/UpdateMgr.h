#ifndef __UpdateMgr_H_
#define __UpdateMgr_H_

//#include <thread>
#include <memory>
#include <string>
#include <vector>
#include <pthread.h>

class TempFile;

struct DownloadPackage {
    DownloadPackage(){
    }
    DownloadPackage(const char * _pkg, const char * _sign, int _version):
        packageName(_pkg), sign(_sign), version(_version){

    }
    std::string packageName;
    std::string sign;
    int version;
};

struct MutexWrapper {
    MutexWrapper(pthread_mutex_t * _mutex) {
        mutex = _mutex;
    }
    ~MutexWrapper() {
        pthread_mutex_unlock(mutex);
    }
    int Lock() {
        return pthread_mutex_lock(mutex);
    }
    pthread_mutex_t * mutex;
};


class UpdateMgr {
public:
    enum ExceptionDef
    {
        //出错定义
        ERROR_CURL_INIT_FAIL    = 1001, //curl初始化失败
        ERROR_CURL_DO_FAIL      = 1002, //curl_easy_perform失败
        ERROR_CURL_DO_FAIL2     = 1003, //

        ERROR_JSON_PARSE_FAIL   = 1004, //JSON parse fail
        ERROR_JSON_FORMAT_ERROR = 1005, //JSON 格式不对
        ERROR_JSON_EXCEPTION    = 1006, //JsonException

        ERROR_SIGN_WRONG        = 1007, //单个包md5校验失败

        ERROR_ZIP_OPEN_FAIL     = 1008, //ZipFile open失败
        ERROR_ZIP_EXTRACT_FAIL  = 1009, //解压失败
        ERROR_VER_FILE_FAIL     = 1010, //version文件打开失败
    };
    enum Stage {
        CHECK_VERSION = 0,
        DOWNLOAD_RESOURCE,
        EXTRACT_RESOURCE, //解压文件
        DONE
    };
    struct Status {
        Stage stage;
        int totalStep;
        int nowStep;
        int nowProgress;
        std::string filename;
        int ret;
        bool needRestartGame; //是否需要重启游戏
    };
    
    UpdateMgr();
    virtual ~UpdateMgr();
    int startUpdate();
    Status GetStatus();
    int OnProgress(float total, float now);
private:
    static void * startUpdate(void*);
    void PushNeedPackage(const char * pkg, const char * sign, int version);
    void GetNeedPackageList(const char * versionFile, int version);
    void UpdateSinglePkg(const DownloadPackage & pkgInfo);
    void DownloadFile(TempFile & file, const char * url, const char * filename);
    void DownloadVersionFile(const char * path, const char * url, const char * filename);
    void ExtractFile(TempFile & tmpFile, const char * pkgname);
protected:
    virtual std::string GetPkgUrl(const char * pkgName) = 0;
    virtual std::string GetTempDownloadPath(const char * pkgName) = 0;
    virtual std::string GetExtractPath() = 0;
    virtual int GetCurrentVersion() = 0;
    virtual void UpdateVersion(int version) = 0;
    virtual bool IsApkNewest() = 0; //apk assets版本是否比本地版本高
    virtual void delResDataDir();

    void ThrowError(const char * filename, int what);
    void doUpdate();

private:
    
    pthread_mutex_t m_mutex;
    std::vector<DownloadPackage> m_needPkg;
    Status m_status; 
};

#endif //__UpdateMgr_H_

