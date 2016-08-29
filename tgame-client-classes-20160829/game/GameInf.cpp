#include "GameInf.h"
#include "login/LoginObj.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "SceneManager.h"
#include "task/TaskControl.h"
#include "login/LoginScene.h"
#include "login/LoginControl.h"
#include "QNetMgr.h"
#include "Global.h"
#include "guide/GuideControl.h"
#include "NounHelper.h"
#include "ChannelInf.h"
#include "QEnvSet.h"
#include "ChannelInfoCallback.h"
#include "FaceBookSDK.h"

#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID) 
#include "jni/JniHelper.h"
#endif

int GameInf::GetZoneID(){
    return P_LOGIN_OBJ->getCurSvrInfo().zoneid;
}

void GameInf::DoConnectionClose(){
    LoginScene *pLoginScene = dynamic_cast<LoginScene*>(CCDirector::sharedDirector()->getRunningScene());
    if(pLoginScene) {
        //�رս�ɫѡ��ȸ���½��ص����
        P_LOGIN_CTRL->forceCloseLoginPanels();
        CCLOG("[warn]GameInf::DoConnectionClose,but now in login scene! just do nothing");
        return;
    }

    CCLOG("[warn]GameInf::DoConnectionClose");
    P_GUIDE_CTRL->endGuide(); //ͣ����������

    //test
    P_GAME_INF->showReconnectTip();
}

void GameInf::onClickClose() {
    CCLOG("[warn]GameInf::onClickClose");
    //�رս�ɫѡ��ȸ���½��ص����
    P_LOGIN_CTRL->forceCloseLoginPanels();
    //�������ǰ��¼��ɫ������
    P_GLOBAL->resetDataOnReconnect();

    //�л�����¼����
    SceneManager::sharedSceneManager()->enterScene(1);
}


void GameInf::showHttpFail() {
    COMMON_DIALOG->show(TextStr(TEXT_NET_CLOSE));
}



void GameInf::showReconnectTip() {
    COMMON_DIALOG->show(TextStr(TEXT_NET_CLOSE), this, TextStr(TEXT_NET_RECONNECT), "", dialogbtneventselector(GameInf::onClickToReconnect), true);
}

void GameInf::onClickToReconnect() {
    COMMON_DIALOG->showLoading(); //loading
	COMMON_DIALOG->reset();

    P_NET_MGR->InitNetConnection(P_LOGIN_OBJ->getCurSvrInfo().ip, 9000);

    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameInf::doAfterTimeout), this, 10.0f, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameInf::doAfterConnect), this, 2.0f, false);
}


void GameInf::doAfterConnect(float dt)  {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameInf::doAfterConnect), this);

    CCLOG("[trace]GameInf::doAfterConnect");
    P_LOGIN_CTRL->getProxy()->sendQuickLoginRqst();
}


void GameInf::doQuickLoginFail() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameInf::doAfterTimeout), this);

    CCLOG("[trace]GameInf::doQuickLoginFail");
    COMMON_DIALOG->closeLoading();

    onClickClose();
}
void GameInf::doQuickLoginSuccess() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GameInf::doAfterTimeout), this);

    CCLOG("[trace]GameInf::doQuickLoginSuccess");
    COMMON_DIALOG->closeLoading();


    COMMON_DIALOG->show(TextStr(TEXT_NET_RECONNECT_DONE));
}


void GameInf::doAfterTimeout(float dt) {
    doQuickLoginFail();
}


void GameInf::sendSysNotify(const std::string& strTitle, const std::string& strTxt, const int iDelaySeconds) {
    
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  

    //·������Ҫ����class�����֣������ܴ���.java�ĺ�׺
    JniMethodInfo methodInfo; //���ڻ�ȡ������  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "doSysNotify", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");  

    if (isHave) {  
        CCLOG("[trace]GameInf::sendSysNotify,get jni method success.");  
        jstring jstr;   
        //NounStr(TXT_TGAME_NAME).c_str()
        jstring jstrGameName = methodInfo.env->NewStringUTF(NounStr(TXT_TGAME_NAME).c_str()); //can send as param
        jstring jstrTitle = methodInfo.env->NewStringUTF(strTitle.c_str());
        jstring jstrTxt = methodInfo.env->NewStringUTF(strTxt.c_str());

        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, jstrGameName,jstrTitle, jstrTxt, iDelaySeconds);  

    } else  {  
        CCLOG("[error]GameInf::sendSysNotify,get jni method fail!!!");  
    } 
#endif  
}


void GameInf::doSDKLogout() {
    CCLOG("[trace]GameInf::doSDKLogout");
	P_NET_MGR->closeConn();
#ifndef TGAME_IN_APP_SDK
    //��ʹ��sdkʱ��ֱ����
    onClickClose();
#endif

#ifdef TGAME_USE_CYOU
    //use chameleon
    ChannelInf::Logout();
    return;
#endif

#ifdef TGAME_IN_ITOOLS
    //use chameleon
    ChannelInf::Logout();
    return;
#endif

	//����SDK�˳���½
	FaceBookSDK::onLogOut();
	return;
}


std::string GameInf::getUmengDeviceToken() {
    CCLOG("[trace]GameInf::getUmengDeviceToken");
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  

    //umeng 
    std::string strRetToken;
    //·������Ҫ����class�����֣������ܴ���.java�ĺ�׺
    JniMethodInfo methodInfo; //���ڻ�ȡ������  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "getUmengDeviceToken", "()Ljava/lang/String;");  

    if (isHave) {  
        jstring jstrToken = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID);  
        strRetToken = JniHelper::jstring2string(jstrToken); 
        CCLOG("[trace]GameInf::getUmengDeviceToken,retToken=[%s]", strRetToken.c_str() );  
    } else  {  
        CCLOG("[error]GameInf::getUmengDeviceToken,get jni method fail!!!");  
    } 
    return strRetToken;
#endif  
    return "";
}

void GameInf::doSDKExit(int iRetCode) {
    CCLOG("[trace]GameInf::doSDKExit,ret %d", iRetCode);
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
    //sdk exit game 
    std::string strRetToken;
    //·������Ҫ����class�����֣������ܴ���.java�ĺ�׺
    JniMethodInfo methodInfo; //���ڻ�ȡ������  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "doSDKExit", "(I)V");  
    if (isHave) {  
        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID, iRetCode);   
    } else  {  
        CCLOG("[error]GameInf::doSDKExit,get jni method fail!!!");  
    } 
#endif  
}

void GameInf::doRestartGame() {
    CCLOG("[trace]GameInf::doRestartGame");
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
    JniMethodInfo methodInfo; //���ڻ�ȡ������  
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "restartGame", "()V");  
    if (isHave) {  
        methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID);   
    } else  {  
        CCLOG("[error]GameInf::doRestartGame,get jni method fail!!!");  
    }
#endif 
}

const char* GameInf::getDirURL() {
    static std::string strUrl;
    strUrl.clear();



#ifdef TGAME_IN_APP_SDK //����sdk������

#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
    //��׿�´���Դ���ȡdir
    const char* pszChannelName = ChannelInf::GetChannelName().c_str();

	if(std::strcmp(pszChannelName, "") == 0) {
		CCLOG("[trace]GameInf::getDirURL get from sdk failed, use local");
		pszChannelName = ChannelInfoCallback::getChannelName().c_str();
	}
	//������ʱ�������ֵ�����ڲ��ӱ�ɫ��������ȡ���ˣ��ӱ��ػ������Ϣ��������
	pszChannelName = ChannelInfoCallback::getChannelName().c_str();
    const char* pszDirCfgJson = "dir_url.json";
    rapidjson::Document json;
    unsigned long size;
    unsigned char *pBytes = CCFileUtils::sharedFileUtils()->getFileData(pszDirCfgJson , "r", &size);
    std::string load_str = std::string((const char*)pBytes, size);
    json.Parse<0>(load_str.c_str());
    CC_SAFE_DELETE_ARRAY(pBytes);

    if (json.HasParseError()) {
        CCLOG("[error]GameInf::getDirURL,json file %s,parse error %s", pszDirCfgJson, json.GetParseError());
        return "";
    }
    if(json.HasMember(pszChannelName) && json[pszChannelName].IsString()) {
        strUrl = json[pszChannelName].GetString();
    }else if (json.HasMember("default") && json["default"].IsString()){
		strUrl = json["default"].GetString();
	}

	

    CCLOG("[trace]GameInf::getDirURL,in android,channel=[%s],dir=[%s]",pszChannelName, strUrl.c_str() );
    return strUrl.c_str();
#endif

#ifdef TGAME_IN_ITOOLS
	strUrl = "itoolsdir.cutecoon.com:8080"; //itoolsdir
	return strUrl.c_str();
#endif

#endif
    //win32����׿�����������
    //strUrl = "192.168.2.32:8080"; //����

	//strUrl = "itoolsdir.cutecoon.com:8080";		//itools dir

	//strUrl = "118.192.89.18:8080"; //����openid��¼	//����

	//strUrl = "10.10.2.59:8080";	//���Է�

	//strUrl = "119.28.7.235:8080"; //���������¼		
    
	strUrl =  "10.10.2.59:8080"; //��k������
	//strUrl = "203.195.191.213:8080";//��������
	
    CCLOG("[trace]GameInf::getDirURL,[%s]", strUrl.c_str() );
    return strUrl.c_str();
}


