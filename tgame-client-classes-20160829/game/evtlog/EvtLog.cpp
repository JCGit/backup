#include "EvtLog.h"
#include "cocos2d.h"
#include "defines.h"
#include "QEnvSet.h"
#include "curl/curl.h"
#include "UserObj.h"

#include <pthread.h>
#include <vector>
pthread_mutex_t gCurlMutex;
bool gIsNeedInitMutex = true;
vector<string> gSendEvtBuffLst;

using namespace std;

#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID) 
#include "jni/JniHelper.h"
#endif

USING_NS_CC;

void EvtLogControl::log(const int iEvtID, const std::string& strExt) {
    if(m_strSysInfo.size() <= 0) {
        initSysInfo();
    }
    CCLOG("[trace]EvtLogControl::log,evt id [%d][%s]", iEvtID, m_strSysInfo.c_str());

#ifndef TGAME_UPLOAD_EVT
    return;
#endif

#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID) 

    string info = "{\"key\":\"" + m_strSysInfo + "[EVT=" + intToString(iEvtID) 
        + "][UIN=" + intToString((unsigned int)(P_USER_OBJ->getUserInfo()->uin)) + "]";
    if(strExt.size() > 0) {
        info += "[EXT=" + strExt + "]";
    }
    info += "[APK=20150106]"; //apk版本标识
    info += "\"}";
    sendEvtID(info); //tgame evt log


    doCyouMBILogEvt(intToString(iEvtID));
#endif
}

void EvtLogControl::logByLv(const int iEvtID, const std::string strExt) {
    if(P_USER_OBJ->getUserInfo()->level <= 3) {
        //限制3级上报
        log(iEvtID, strExt);
    }
}
void EvtLogControl::log(const int iEvtID, const int iExtID) {
    if(iExtID > 0) {
        log(iEvtID, intToString(iExtID) );
    } else {
        log(iEvtID, "");
    }
}


void EvtLogControl::initSysInfo() {
    if(m_strSysInfo.size() > 0 ) {
        return;
    }

#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  

    //路径必须要给出class的名字，但不能带有.java的后缀
    JniMethodInfo methodInfo; //用于获取函数体  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "doAction", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");  

    if (isHave)  
    {  
        CCLOG("[trace]EvtLogControl::initSysInfo,get jni method success.");  
        jstring jstr;   
        jstring key_arg = methodInfo.env->NewStringUTF("tgame_key"); //can send as param
        jstring msg_arg = methodInfo.env->NewStringUTF( "tgame_val");

        jstring retstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, key_arg, msg_arg);  

        string cstr = JniHelper::jstring2string(retstr); 
        CCLOG("[trace]EvtLogControl::initSysInfo,sysinfo=%s", cstr.c_str());
        m_strSysInfo = cstr;
    }  
    else 
    {  
        CCLOG("[error]EvtLogControl::initSysInfo,get jni method fail!!!");  
        m_strSysInfo = "[fail]";
    } 
#endif  

}


void EvtLogControl::sendEvtID(const std::string &strInfo) {

    CCLOG("[trace]EvtLogControl::sendEvtID,%s", strInfo.c_str());
    //用线程来吧
    int ret = 0;
    if(gIsNeedInitMutex) {
        ret = pthread_mutex_init(&gCurlMutex, NULL);
        if (ret != 0) {
            CCLOG("[warn]EvtLogControl::sendEvtID,Fail to init mutex");
            return;
        }
        curl_global_init(CURL_GLOBAL_ALL);   //global init just once,neilwu comment
        gIsNeedInitMutex = false;
    }
    pthread_mutex_lock(&gCurlMutex);
    gSendEvtBuffLst.push_back(strInfo); //curl在线程发送时不能使用临时变量
    pthread_mutex_unlock(&gCurlMutex);

    pthread_t thread;
    ret = pthread_create(&thread, NULL, &EvtLogControl::pthreadSendEvtID, NULL); 
    if (ret == 0) {
        pthread_detach(thread);
    }
 
}

void* EvtLogControl::pthreadSendEvtID(void *pStr) {
    CCLOG("[trace]EvtLogControl::pthreadSendEvtID");
    if(gSendEvtBuffLst.size() <= 0) {
        return NULL;
    }

    pthread_mutex_lock(&gCurlMutex);
    vector<string> stTmpSendEvtBuffLst = gSendEvtBuffLst;
    gSendEvtBuffLst.clear();
    pthread_mutex_unlock(&gCurlMutex);

    try   
    {  
        CURL *pCurl = NULL;  
        CURLcode res;  

        // get a curl handle  
        pCurl = curl_easy_init();  
        if (NULL != pCurl)   
        {  
            // 设置超时时间为10秒  
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 10);  
            curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1); //多线程时设置
            curl_easy_setopt(pCurl, CURLOPT_CUSTOMREQUEST, "POST");

            // First set the URL that is about to receive our POST.   
            // This URL can just as well be a   
            // https:// URL if that is what should receive the data.  
            curl_easy_setopt(pCurl, CURLOPT_URL, "http://118.192.76.46:8090/clickstream");  

            // 设置http发送的内容类型为JSON  
            curl_slist *plist = curl_slist_append(NULL,   
                "Content-Type:application/json;charset=UTF-8");  
            curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, plist);  


            while(stTmpSendEvtBuffLst.size() > 0) {
                // 设置要POST的JSON数据  
                curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS,  stTmpSendEvtBuffLst.rbegin()->c_str() );  

                // Perform the request, res will get the return code   
                res = curl_easy_perform(pCurl);  
                // Check for errors  
                if (res != CURLE_OK)   
                {  
                    CCLOG("[warn]EvtLogControl::pthreadSendEvtID,curl_easy_perform() failed:%s\n", curl_easy_strerror(res)); 
                    CCLOG("[warn]EvtLogControl::pthreadSendEvtID,fail to send data=[%s]", stTmpSendEvtBuffLst.rbegin()->c_str() );   
                }
                stTmpSendEvtBuffLst.erase(stTmpSendEvtBuffLst.begin() + stTmpSendEvtBuffLst.size() - 1);
            }

            // always cleanup  
            curl_easy_cleanup(pCurl);  
        }  

    }  
    catch (std::exception &ex)  
    {  
        CCLOG("[warn]EvtLogControl::pthreadSendEvtID,curl exception %s.\n", ex.what());  
    } 

    return NULL;
}


void EvtLogControl::doCyouLogin() {
#ifndef TGAME_USE_CYOU
    return;
#endif
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  

    //路径必须要给出class的名字，但不能带有.java的后缀
    JniMethodInfo methodInfo; //用于获取函数体  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "doCyouLogin", "()V");  

    if (isHave) {  
        CCLOG("[trace]EvtLogControl::doCyouLogin,get jni method success.");  

        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID);  
    } else  {  
        CCLOG("[error]EvtLogControl::doCyouLogin,get jni method fail!!!");  

    } 
#endif  
}

void EvtLogControl::doCyouMBILogLogin() {
#ifndef TGAME_USE_CYOU
    return;
#endif
    //选区登陆后上报
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
    //路径必须要给出class的名字，但不能带有.java的后缀
    JniMethodInfo methodInfo; //用于获取函数体  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "doCyouMBILogLogin", "(Ljava/lang/String;Ljava/lang/String;)V");  

    if (isHave) {  
        CCLOG("[trace]EvtLogControl::doCyouMBILogLogin,get jni method success.");  
        std::string strSDKUid = P_USER_OBJ->getUserInfo()->loginInfo; //ChannelInfoCallback::onCyouSDKLogin里设置
        std::string strZoneID = intToString(P_USER_OBJ->getUserInfo()->zoneid);
        jstring jstrUid = methodInfo.env->NewStringUTF(strSDKUid.c_str()); 
        jstring jstrZone = methodInfo.env->NewStringUTF(strZoneID.c_str()); 

        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, jstrUid, jstrZone);  
    } else  {  
        CCLOG("[error]EvtLogControl::doCyouMBILogLogin,get jni method fail!!!");  
    } 
#endif  
}


void EvtLogControl::doCyouMBILogEvt(const std::string &strEvtID) {
#ifndef TGAME_USE_CYOU
    return;
#endif
    //上报事件日志
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
    //路径必须要给出class的名字，但不能带有.java的后缀
    JniMethodInfo methodInfo; //用于获取函数体  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "doCyouMBILogEvt", "(Ljava/lang/String;)V");  

    if (isHave) {  
        CCLOG("[trace]EvtLogControl::doCyouMBILogEvt,get jni method success.");  
        jstring jstrEvtID = methodInfo.env->NewStringUTF(strEvtID.c_str()); 

        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, jstrEvtID );  
    } else  {  
        CCLOG("[error]EvtLogControl::doCyouMBILogEvt,get jni method fail!!!");  
    } 
#endif  
}

void EvtLogControl::doCyouOpenBugReport() {
#ifndef TGAME_USE_CYOU
    return;
#endif
    //cyou客服接口
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
    JniMethodInfo methodInfo; //用于获取函数体  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "doCyouBugReport", "(Ljava/lang/String;)V");  

    if (isHave) {  
        CCLOG("[trace]EvtLogControl::doCyouOpenBugReport,get jni method success.");  

        jstring jstrCyouUid = methodInfo.env->NewStringUTF(P_USER_OBJ->getUserInfo()->openID.c_str()); 

        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, jstrCyouUid );  
    } else  {  
        CCLOG("[error]EvtLogControl::doCyouOpenBugReport,get jni method fail!!!");  
    } 
#endif 
}


void EvtLogControl::logTDAppReg(const std::string &strUserID) {
#ifndef TGAME_UPLOAD_EVT
    return;
#endif
    //注册成功
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
    JniMethodInfo methodInfo; //用于获取函数体  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "logTDAppReg", "(Ljava/lang/String;)V");  
    if (isHave) {  
        jstring key_arg = methodInfo.env->NewStringUTF(strUserID.c_str()); //can send as param
        CCLOG("[trace]EvtLogControl::logTDAppReg,get jni method success. userid %s", strUserID.c_str() );  
        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, key_arg);  
    } else {  
        CCLOG("[error]EvtLogControl::logTDAppReg,get jni method fail!!!");  
    } 
#endif  
}

void EvtLogControl::logTDAppLogin(const std::string &strUserID) {
#ifndef TGAME_UPLOAD_EVT
    return;
#endif
    //登陆成功
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
    JniMethodInfo methodInfo; //用于获取函数体  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "logTDAppLogin", "(Ljava/lang/String;)V");  
    if (isHave) {  
        jstring key_arg = methodInfo.env->NewStringUTF(strUserID.c_str()); //can send as param
        CCLOG("[trace]EvtLogControl::logTDAppLogin,get jni method success. userid %s", strUserID.c_str() );  
        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, key_arg);  
    } else {  
        CCLOG("[error]EvtLogControl::logTDAppLogin,get jni method fail!!!");  
    } 
#endif  
}

void EvtLogControl::logTDGamePay(const string& strUserID, 
                                 const string& strOrderID, 
                                 int iAmount, 
                                 const string& strCurrency, 
                                 const string& strPayType)
{
#ifndef TGAME_UPLOAD_EVT
    return;
#endif
    //支付
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
    JniMethodInfo methodInfo; //用于获取函数体  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "logTDGamePay", 
        "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V");  
    if (isHave) {  
        CCLOG("[trace]EvtLogControl::logTDGamePay,get jni method success.");  
        jstring jstrUserID = methodInfo.env->NewStringUTF(strUserID.c_str()); //can send as param
        jstring jstrOrderID = methodInfo.env->NewStringUTF(strOrderID.c_str()); //can send as param
        jstring jstrCurrency = methodInfo.env->NewStringUTF(strCurrency.c_str()); //can send as param
        jstring jstrPayType = methodInfo.env->NewStringUTF(strPayType.c_str()); //can send as param
        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, jstrUserID, jstrOrderID, iAmount, jstrCurrency, jstrPayType);  
    } else {  
        CCLOG("[error]EvtLogControl::logTDGamePay,get jni method fail!!!");  
    } 
#endif 
}

void EvtLogControl::logTDGameAddRole(const std::string &strRoleName) {
#ifndef TGAME_UPLOAD_EVT
    return;
#endif
    //创建角色
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
    JniMethodInfo methodInfo; //用于获取函数体  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "logTDGameAddRole", "(Ljava/lang/String;)V");  
    if (isHave) {  
        CCLOG("[trace]EvtLogControl::logTDGameAddRole,get jni method success.");  
        jstring jstrRoleName = methodInfo.env->NewStringUTF(strRoleName.c_str()); //can send as param
        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, jstrRoleName);  
    } else {  
        CCLOG("[error]EvtLogControl::logTDGameAddRole,get jni method fail!!!");  
    } 
#endif 
}
