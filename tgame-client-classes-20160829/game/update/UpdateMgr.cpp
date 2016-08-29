#include "UpdateMgr.h"

#include <sys/stat.h>
#include <ctime>
#ifdef WIN32
//do nothing
#else  
#include <utime.h> 
#include <sys/time.h> 
#include <dirent.h>  
#endif


#include "curl/curl.h"
#include "CocoStudio/Json/rapidjson/document.h"
#include "CocoStudio/Json/rapidjson/filestream.h"
#include "JsonHelper.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "evtlog/EvtLog.h"
#include "UpdateHelper.h"


USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

const char * VERSION_FILE_NAME = "version.json";

namespace {
    int progress_callback(void *clientp, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded) {
        UpdateMgr * updatemgr = (UpdateMgr *) clientp;
        //CCLOG("[trace]progress_callback, %f, %f, %f, %f", totalToDownload, nowDownloaded);
        return updatemgr->OnProgress(totalToDownload, nowDownloaded);
    } //end namespace

    class UpdateException {
    public:
        UpdateException(const char* filename, int what):
        m_filename(filename), m_what(what){
        }
      
    public:
        std::string m_filename;
        int m_what;
    };

}


UpdateMgr::UpdateMgr() {
    m_status.ret = 0;
    m_status.stage = CHECK_VERSION;
    m_status.totalStep = 0;
    m_status.nowStep = 0;
    m_status.nowProgress = 0;
    m_status.needRestartGame = false;
    
}

void UpdateMgr::DownloadFile(TempFile & file, const char * url, const char * filename) {
    int ret = 0;
    do {
        CURL * curl = curl_easy_init();
        if (curl == NULL) {
            CCLOG("[trace]Fail to init curl");
            ThrowError(filename, ERROR_CURL_INIT_FAIL);
            break;
        }
        if (!file.Open("wb")) {
            CCLOG("[trace]Fail to open local file to write %s", file.GetPath().c_str());
            ThrowError(filename, ERROR_VER_FILE_FAIL);
            break;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, UpdateHelper::saveData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);
        ret = curl_easy_perform(curl);
        if (ret != CURLM_OK) {
            CCLOG("[trace]Fail to fetch data from %s", url);
            ThrowError(filename, ERROR_CURL_DO_FAIL);
            break;
        }
        curl_easy_cleanup(curl);
        CCLOG("[trace]File %s downloaded", file.GetPath().c_str());
    } while(0);
}

int UpdateMgr::OnProgress(float total, float now) {
    if(total <= 0.1) {
        return 0;
    }
    m_status.nowProgress = int(now * 100 / total);

    //CCLOG("[trace]on progress %d", m_status.nowProgress);
    return 0;
}

void UpdateMgr::DownloadVersionFile(const char * path, 
                                    const char * url, 
                                    const char * filename) 
{
    ADD_EVT_LOG(EVT_LOG_CHECK_UPDATE); //游戏事件日志
    struct stat filestat;
    time_t modifyTime = 0;
    time_t serverFileTime = 0;
    long code = stat(path, &filestat);
    if (code == 0) {
        modifyTime = filestat.st_mtime;
    }
    int ret = 0;
    CURL * curl = curl_easy_init();
    std::string tmpfilename(path);
    remove(path); //neilwu mod,先把老的删除掉

    //tmpfilename += ".bak" ;

    FILE * file = fopen(tmpfilename.c_str(), "wb");
    do {
        if (curl == NULL) {
            CCLOG("Fail to init curl");
            ThrowError(filename, ERROR_CURL_INIT_FAIL);
            break;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, UpdateHelper::saveDataToFD);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        curl_easy_setopt(curl, CURLOPT_FILETIME, &serverFileTime);
        /*
        if (modifyTime != 0) {
            CCLOG("%ld modify time", modifyTime);
            //curl_easy_setopt(curl, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE); //neilwu, comment
            //curl_easy_setopt(curl, CURLOPT_TIMEVALUE, modifyTime); //neilwu, comment
        }*/
        ret = curl_easy_perform(curl);
        if (ret != CURLM_OK) {
            CCLOG("[warn]UpdateMgr::DownloadVersionFile,Fail to fetch data from %s,curl ret %d", url, ret);
            ThrowError(filename, ERROR_CURL_DO_FAIL);
            break;
        }
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
        curl_easy_getinfo(curl, CURLINFO_FILETIME, &serverFileTime);
        CCLOG("[trace]curl get code ret %ld", code);
        CCLOG("[trace]File %s downloaded", path);
    } while(0);
    fclose(file);
    curl_easy_cleanup(curl);
    if (ret == 0 && code == 200) { 
        //200 - 确定。客户端请求已成功。
        //remove(path); //neilwu, comment
        //rename(tmpfilename.c_str(), path); //去掉.bak后缀
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)  
        //linux
        struct utimbuf buf;
        if (serverFileTime < 0) {
            serverFileTime = 0;
        }
        buf.actime = serverFileTime;
        buf.modtime = serverFileTime;
        CCLOG("%ld server modify time", serverFileTime);
        utime(path, &buf);
#else
        //win32
        //SetFileTime(file, );
#endif
        ADD_EVT_LOG(EVT_LOG_CHECK_UPDATE_RET); //游戏事件日志
    } else {
        //remove(tmpfilename.c_str()); //如果出错就删除该.bak文件
    }
    
    if (ret != 0) {
        ThrowError(filename, ERROR_CURL_DO_FAIL2);
    }
}


int UpdateMgr::startUpdate() {
    pthread_t thread;
    int ret = pthread_mutex_init(&m_mutex, NULL);
    if (ret != 0) {
        CCLOG("Fail to init mutex");
        return ret;
    }
    ret = pthread_create(&thread, NULL, &UpdateMgr::startUpdate, (void*)this); 
    if (ret == 0) {
        pthread_detach(thread);
    }
    return ret;
}

void UpdateMgr::doUpdate() {
    {
        MutexWrapper locker(&m_mutex);
        locker.Lock();
        m_status.stage = CHECK_VERSION;
    }
    try {
        UpdateHelper::MakeDirs(this->GetExtractPath().c_str()); //先创建文件夹
        std::string path = this->GetExtractPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
        //win下m_writableFolder最后带着 反斜杠
        path += "\\Resources\\";
#else  
        path += "/Resources/"; 
#endif
        if(IsApkNewest()) {
            delResDataDir();
        }
        UpdateHelper::MakeDirs(path.c_str()); //先创建文件夹

        std::string versionFilePath = GetTempDownloadPath(VERSION_FILE_NAME);

        DownloadVersionFile(versionFilePath.c_str(), 
            GetPkgUrl(VERSION_FILE_NAME).c_str(), VERSION_FILE_NAME);

        ADD_EVT_LOG(EVT_LOG_CHECK_UPDATE_RET_CHECK); //游戏事件日志
        GetNeedPackageList(versionFilePath.c_str(), GetCurrentVersion());

        if(m_needPkg.size() <= 0) {
            CCLOG("[trace]UpdateMgr::doUpdate,no pkg need to be updated.");
        } else {
            for (size_t i = 0; i < m_needPkg.size(); ++i) {
                UpdateSinglePkg(m_needPkg[i]);
            }
        }
        
        m_status.stage = DONE;
    } catch (const UpdateException & e) {
        {
            MutexWrapper locker(&m_mutex);
            locker.Lock();
            m_status.stage = DONE;
            m_status.ret = e.m_what;
            m_status.filename = e.m_filename;
        }
    }
}

void UpdateMgr::GetNeedPackageList(const char * versionFile, int version) {
    FILE * fd = fopen(versionFile, "rb");
    rapidjson::Document document;     
    rapidjson::FileStream fs(fd);
    document.ParseStream<0,rapidjson::FileStream>(fs);
    if (document.HasParseError() || !document.IsArray()) {
        CCLOG("parse error %s", document.GetParseError());
        ThrowError(VERSION_FILE_NAME, ERROR_JSON_PARSE_FAIL);
    } else {
        try {
            for (size_t i = 0; i < document.Size(); ++i) {
                rapidjson::Value & val = document[i];
                if (!val.IsObject()) {
                    return ThrowError(VERSION_FILE_NAME, ERROR_JSON_FORMAT_ERROR);
                }
                int thisversion = GetJsonValue<int>(val, "ver");
                if (thisversion > version) {
                    PushNeedPackage(GetJsonValue<const char*>(val, "pkg"),
                        GetJsonValue<const char*>(val, "sign"), 
                        thisversion);
                }
				CCLOG("[trace]catch GetNeedPackageList, ver:%d,cur:%d", thisversion, version);
            }
        } catch (JsonException & err) {
            CCLOG("[error]catch JsonException, err %s,file %s", err.mErrMsg.c_str(), VERSION_FILE_NAME);
            ThrowError(VERSION_FILE_NAME, ERROR_JSON_EXCEPTION);
        }
    }
    MutexWrapper locker(&m_mutex);
    locker.Lock();
    m_status.stage = DOWNLOAD_RESOURCE;
    m_status.totalStep = (int)m_needPkg.size();
    m_status.nowStep = 0;
    m_status.nowProgress = 0;
}

void * UpdateMgr::startUpdate(void * data) {
    UpdateMgr * mgr = (UpdateMgr*)data;
    mgr->doUpdate();
    return NULL;
}

UpdateMgr::~UpdateMgr() {
    //pthread_mutex_destroy(&m_mutex);
}

void UpdateMgr::PushNeedPackage(const char * pkg, const char * sign, int version) {
    CCLOG("[trace]need pkg=%s,sign=%s,version=%d", pkg, sign, version);
    m_needPkg.push_back(DownloadPackage(pkg, sign, version));
}


void UpdateMgr::UpdateSinglePkg(const DownloadPackage & pkgInfo) {
    {
        MutexWrapper locker(&m_mutex);
        locker.Lock();
        m_status.nowStep++;
        m_status.nowProgress = 0;
        m_status.filename = pkgInfo.packageName;
    }
    std::string url = GetPkgUrl(pkgInfo.packageName.c_str());
    TempFile tmpFile(GetTempDownloadPath(pkgInfo.packageName.c_str()));

    ADD_EVT_LOG(EVT_LOG_START_DOWNLOAD); //游戏事件日志
    DownloadFile(tmpFile, url.c_str(), pkgInfo.packageName.c_str());
    ADD_EVT_LOG(EVT_LOG_END_DOWNLOAD); //游戏事件日志

    if (tmpFile.GetSign() != pkgInfo.sign) {
        CCLOG("[error]pkg %s's sign not equal,tmp file sign=%s,need sign=%s",
            pkgInfo.packageName.c_str(), tmpFile.GetSign().c_str(), pkgInfo.sign.c_str());
        ThrowError(pkgInfo.packageName.c_str(), ERROR_SIGN_WRONG);
    }

    ADD_EVT_LOG(EVT_LOG_START_UNZIP); //游戏事件日志
    ExtractFile(tmpFile, pkgInfo.packageName.c_str());
    ADD_EVT_LOG(EVT_LOG_END_UNZIP); //游戏事件日志

    UpdateVersion(pkgInfo.version);
    {
        MutexWrapper locker(&m_mutex);
        locker.Lock();
        m_status.nowProgress = 100;
    }
}

UpdateMgr::Status UpdateMgr::GetStatus() {
    MutexWrapper locker(&m_mutex);
    locker.Lock();
    return m_status;
}


void UpdateMgr::ExtractFile(TempFile & tmpFile, const char * pkgname) {
    tmpFile.Close();
    //解压文件
    ZipFile zipfile(tmpFile.GetPath().c_str(), &(m_status.nowProgress));
    m_status.stage = EXTRACT_RESOURCE;
    if (!zipfile.IsOpen()) {
        CCLOG("[error]UpdateMgr::ExtractFile,Fail to open zip file %s", pkgname);
        ThrowError(pkgname, ERROR_ZIP_OPEN_FAIL);
    }
    if (zipfile.ExtractTo(GetExtractPath().c_str()) != 0) {
        CCLOG("[error]UpdateMgr::ExtractFile,Fail to extract zip file %s", pkgname);
        ThrowError(pkgname, ERROR_ZIP_EXTRACT_FAIL);
    }
    if(zipfile.getIsHaveSo()) {
        m_status.needRestartGame = true;
    }
    
}


void UpdateMgr::ThrowError(const char * filename, int what) {
    throw UpdateException(filename, what);
}



void UpdateMgr::delResDataDir() {
    //不需要更新时，删除下载目录
    CCLOG("[trace]UpdateMgr::delResDataDir");
    std::string path = this->GetExtractPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
    //win下m_writableFolder最后带着 反斜杠
    path += "\\Resources\\";
#else  
    path += "/Resources/"; 
#endif
    UpdateHelper::RemoveDir(path.c_str());


}

