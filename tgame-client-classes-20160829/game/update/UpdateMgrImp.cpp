#include "UpdateMgrImp.h"
#include "cocos2d.h"
#include "platform/CCFileUtils.h"
#include "cocos-ext.h"
#include "QEnvSet.h"

using namespace std;
USING_NS_CC_EXT;
using namespace cocos2d;

static const string LOCAL_VERSION_FILE = "local_version.json";

UpdateMgrImp::UpdateMgrImp() {
    m_writableFolder = CCFileUtils::sharedFileUtils()->getWritablePath();
    m_bCanUpdateSearchPath = true;
}

void UpdateMgrImp::setResUrl(const char* pszUrl) {
    if(pszUrl) {
        CCLOG("[trace]UpdateMgrImp::setResUrl,set res url [%s]", pszUrl);
        m_strResUrl = pszUrl;
    }
}

std::string UpdateMgrImp::GetPkgUrl(const char * pkgName) {
    return (m_strResUrl) + std::string(pkgName); 
}

std::string UpdateMgrImp::GetTempDownloadPath(const char * pkgName) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
    return m_writableFolder + pkgName; 
#else  
    return m_writableFolder+"/"+pkgName; 
#endif
}

std::string UpdateMgrImp::GetExtractPath() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
    //win下m_writableFolder最后带着 反斜杠
    return m_writableFolder + "data";
#else  
    return m_writableFolder + "/data"; 
#endif
    return "";
}

int UpdateMgrImp::GetCurrentVersion() {
    //返回当前所使用版本的版本号
    //1. 判断LOCAL_VERSION_FILE文件是否存在
    std::string strLocalVer = m_writableFolder + LOCAL_VERSION_FILE;
    //如果存在，则返回此中信息
    int iVer = 0;
    FILE * fs = fopen(strLocalVer.c_str(), "r");
    if (fs) {
        fclose(fs);
        iVer = getJsonVer(strLocalVer.c_str());
    } else {
        //如果不存在，返回apk里的版本信息
        std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("res_version.json");
        iVer = getJsonVer(fullPath.c_str());
    }
    CCLOG("[trace]current version = %d", iVer);

    return iVer;
}

void UpdateMgrImp::UpdateVersion(int version) {
    //每次更新完后都会调用这个值
    //写如次次更新完后的版本号
    std::string strLocalVer = m_writableFolder + LOCAL_VERSION_FILE;
    //如果存在，则返回此中信息
    int iVer = 0;
    FILE * fs = fopen(strLocalVer.c_str(), "wb");
    if(NULL == fs) {
        CCLOG("[error]UpdateMgrImp::UpdateVersion,fail to open file %s for write.", strLocalVer.c_str());
        return;
    }
    char strBuf[100] = {0};
    snprintf(strBuf, 100, "{\"ver\":%d}", version);
    fputs(strBuf, fs);  
    fclose(fs); 
    CCLOG("[trace]UpdateMgrImp::UpdateVersion,update local version to %d", version);

    //检查，如果apk中资源版本比这个版本高，则不需要更新搜索路径
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("res_version.json");
    const int iApkResVer = getJsonVer(fullPath.c_str());
    if(iApkResVer <= 0 || iApkResVer < version) {
        m_bCanUpdateSearchPath = true;
    } else {
        m_bCanUpdateSearchPath = false;
    }
    if(iApkResVer >= version) {
        //如果apk中资源版本比这个版本高
        //删除下载目录
        delResDataDir();
    }
}


int UpdateMgrImp::getJsonVer(const char* strFilePath) {
    rapidjson::Document json;
    unsigned long size;
    unsigned char *pBytes = CCFileUtils::sharedFileUtils()->getFileData(strFilePath , "r", &size);
    std::string load_str = std::string((const char*)pBytes, size);
    json.Parse<0>(load_str.c_str());
    int ver = 0;
    if (json.HasParseError()) {
        CCLOG("[error]json file %s,parse error %s", strFilePath, json.GetParseError());
    } else {
        ver =  DICTOOL->getIntValue_json(json, "ver"); //默认值为0
        CCLOG("[trace]load json file %s,version = %d", strFilePath, ver);
    }
    CC_SAFE_DELETE_ARRAY(pBytes);

    return ver;
}


void UpdateMgrImp::updateSearchPath() {
    std::string path = GetExtractPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
    //win下m_writableFolder最后带着 反斜杠
    path += "\\Resources\\";
#else  
    path += "/Resources/"; 
#endif
    if(m_bCanUpdateSearchPath) {
        CCLOG("[trace]UpdateMgrImp::updateSearchPath,add search path:%s",path.c_str());
        CCFileUtils::sharedFileUtils()->addSearchPath(path.c_str());
    } else {
        CCLOG("[trace]UpdateMgrImp::updateSearchPath,del search path:%s",path.c_str());
        CCFileUtils::sharedFileUtils()->removeSearchPath(path.c_str());
    }
    

}

bool UpdateMgrImp::IsApkNewest() {

    //1. 判断LOCAL_VERSION_FILE文件是否存在
    std::string strLocalVer = m_writableFolder + LOCAL_VERSION_FILE;
    //如果存在，则同assets里的比较
    
    FILE * fs = fopen(strLocalVer.c_str(), "r");
    if (!fs) {
        return false;
    }
    fclose(fs);
    int iLocalVer = getJsonVer(strLocalVer.c_str());

    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("res_version.json");
    const int iApkResVer = getJsonVer(fullPath.c_str());
    if(iApkResVer > 0 && iApkResVer > iLocalVer) {
        CCLOG("[trace]UpdateMgrImp::IsApkNewest, apk ver [%d] is newest,local ver [%d]", iApkResVer, iLocalVer);
        return true;
    }

    return false;
}

void UpdateMgrImp::delResDataDir() {
    CCLOG("[trace]UpdateMgrImp::delResDataDir");
    UpdateMgr::delResDataDir();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 

#else  
    std::string strLocalVer = m_writableFolder + LOCAL_VERSION_FILE;
    remove(strLocalVer.c_str());
#endif
}


