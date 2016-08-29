#include "FaceBookSDK.h"
#include "cocos2d.h"
#include "UserObj.h"
#include "defines.h"
#include "GameInf.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "shop/ShopControl.h"
#include "login/LoginControl.h"
#include "game/task/TaskControl.h"

#include "CocoStudio/Json/rapidjson/document.h"
#include "CocoStudio/Json/rapidjson/filestream.h"
#include "CocoStudio/Json/rapidjson/writer.h"
#include "CocoStudio/Json/rapidjson/stringbuffer.h"


#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID) 
#include "jni/JniHelper.h"
#endif

USING_NS_CC;
using namespace std;

string FaceBookSDK::m_stPlayerId = "";		//玩家ID
bool FaceBookSDK::m_bIsGuest = true;		//是否游客登陆
string FaceBookSDK::m_stSig = "";			//用户用于与服务器上用户验证的签名

bool FaceBookSDK::m_bIsSessionValid = false;
string FaceBookSDK::m_stUserType = "";

void FaceBookSDK::onLogin(int iRetCode){
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
	//路径必须要给出class的名字，但不能带有.java的后缀
	JniMethodInfo methodInfo; //用于获取函数体  
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "onLogIn", "(I)V");  

	CCLOG("[facebook]=====[trace]FaceBookSDK::onLogin,start to call java onLogin"); 

	if (isHave) {  
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, iRetCode);  
	} else  {  
		CCLOG("[error]FaceBookSDK::onLogin,get jni method fail!!!");  
	} 
#endif  
}

void FaceBookSDK::onSDKData(std::string stPlayerId, bool bIsGuest, std::string stSig){
	m_stPlayerId = stPlayerId;
	m_bIsGuest = bIsGuest;
	m_stSig = stSig;

	if(!bIsGuest){			//facebook登陆成功
		onFaceBookLogin();
	}else{	//游客登陆成功
		onGuestLogin();
	}
}

void FaceBookSDK::onBind(){
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
	//路径必须要给出class的名字，但不能带有.java的后缀
	JniMethodInfo methodInfo; //用于获取函数体  
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "onBind", "()V");  

	CCLOG("[facebook]=====[trace]FaceBookSDK::onBind,start to call java onBind"); 

	if (isHave) {  
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);  
	} else  {  
		CCLOG("[error]FaceBookSDK::onBind,get jni method fail!!!");  
	} 
#endif  
}

void FaceBookSDK::onFaceBookLogin(){
	CCLOG("[facebook]=====FaceBookSDK onFaceBookLogin Success");

	P_USER_OBJ->getMutableUserInfo()->openID = m_stPlayerId;

	P_LOGIN_CTRL->doAfterSDKInited();  //这里要重新去拉一次dir，否则要出问题

	CCTextureCache::sharedTextureCache()->reloadAllTextures();

	P_LOGIN_CTRL->getChooseSvrPanel()->closeLoading();
	P_LOGIN_CTRL->getLoginScreenView()->onLoginInfoDone(genLoginJson()); //赋值的是loginInfo字段
}

void FaceBookSDK::onGuestLogin(){
	CCLOG("[facebook]=====FaceBookSDK onGuestLogin Success");

	P_USER_OBJ->getMutableUserInfo()->openID = m_stPlayerId;

	P_LOGIN_CTRL->doAfterSDKInited();  //这里要重新去拉一次dir，否则要出问题

	CCTextureCache::sharedTextureCache()->reloadAllTextures();

	P_LOGIN_CTRL->getChooseSvrPanel()->closeLoading();
	P_LOGIN_CTRL->getLoginScreenView()->onLoginInfoDone(genLoginJson()); //赋值的是loginInfo字段
}

void FaceBookSDK::onFaceBookBlind(std::string stPlayerId, std::string stSig){
	CCLOG("[facebook]=====FaceBookSDK onBlind Success");

	m_bIsGuest = false;
	m_stPlayerId = stPlayerId;
	m_stSig = stSig;

	COMMON_DIALOG->show(TextStr(TEXT_SDK_BIND_SUCCESS_TXT));
}

void FaceBookSDK::onLogOut(){
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
	//路径必须要给出class的名字，但不能带有.java的后缀
	JniMethodInfo methodInfo; //用于获取函数体  
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "onLogOut", "()V");  

	CCLOG("[facebook]=====[trace]FaceBookSDK::onLogOut,start to call java onLogOut");  

	if (isHave) {  
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);  
	} else  {  
		CCLOG("[error]FaceBookSDK::onLogOut,get jni method fail!!!");  
	} 
#endif  
}

void FaceBookSDK::onFaceBookLogout(){
	CCLOG("[trace]FaceBookSDK::onFaceBookLogout, return success!!");

	//P_LOGIN_CTRL->getGameExitPanel()->show(TextStr(TEXT_SDK_LOGOUT_SUCCESS_TXT));

	COMMON_DIALOG->closeLoading();
	P_GAME_INF->onClickClose();
}

string FaceBookSDK::genLoginJson(){
	//rapidjson::Document jsonDoc;
 //   jsonDoc.Parse<0>("{}");
 //   rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
 //   
 //   jsonDoc.AddMember("channel", "nd91", allocator);
 //   jsonDoc.AddMember("others", "facebookchannel", allocator);
 //   jsonDoc.AddMember("token", P_USER_OBJ->getUserInfo()->openID, allocator);

 //   rapidjson::StringBuffer  buffer;
 //   rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
 //   jsonDoc.Accept(writer);
	//string stLoginInfo = buffer.GetString();

	string stOpenID = P_USER_OBJ->getUserInfo()->openID;
	char buff[300] = {0};
	snprintf(buff, 300, "{\"channel\":\"nd91\",\"others\":\"facebookchannel\",\"token\":\"%s\"}", stOpenID.c_str());
    
	string stLoginInfo = buff;

	CCLOG("[trace]FaceBookSDK::genLoginJson ==== the login str === [%s]", stLoginInfo.c_str());

	return stLoginInfo;
}

bool FaceBookSDK::isSessionValid(){
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  

	//路径必须要给出class的名字，但不能带有.java的后缀
	JniMethodInfo methodInfo; //用于获取函数体  
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "isSessionValid", "()Z");  

	CCLOG("[facebook]=====[trace]FaceBookSDK::isSessionValid,start to call java isSessionValid");   

	jboolean bIsValid;
	if (isHave) {  
		bIsValid = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
		m_bIsSessionValid = bIsValid;
	} else  {  
		CCLOG("[error]FaceBookSDK::isSessionValid,get jni method fail!!!");  
	} 


	return m_bIsSessionValid;
#endif 

	return false;
}

string FaceBookSDK::getUserType(){
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  

	//路径必须要给出class的名字，但不能带有.java的后缀
	JniMethodInfo methodInfo; //用于获取函数体  
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "getUserType", "()Ljava/lang/String;");  

	CCLOG("[facebook]=====[trace]FaceBookSDK::getUserType,start to call java isSessionValid");  
	jstring stType;
	if (isHave) {  
		stType = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);  
		m_stUserType = JniHelper::jstring2string(stType);
	} else  {  
		CCLOG("[error]FaceBookSDK::getUserType,get jni method fail!!!");  
	} 
	return m_stUserType;
#endif 

	return "";
}

void FaceBookSDK::onPay(int iMoney){
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
	//路径必须要给出class的名字，但不能带有.java的后缀
	JniMethodInfo methodInfo; //用于获取函数体  
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"com/game/tgame/JavaHelper", "onPayFun", "(Ljava/lang/String;Ljava/lang/String;)V");  

	string strUin = intToString(UINT64_t(P_USER_OBJ->getUserInfo()->uin));
	string strMoney = intToString(iMoney);
	string strCustomInfo = strUin + "|" + strMoney;
	string strZoneID = intToString(P_USER_OBJ->getUserInfo()->zoneid);
	jstring jstrCustomInfo = methodInfo.env->NewStringUTF(strCustomInfo.c_str()); 
	jstring jstrZoneId = methodInfo.env->NewStringUTF(strZoneID.c_str()); 

	CCLOG("[facebook]=====[trace]FaceBookSDK::onPay, start to call java onPayFun, the zoneid is == %s, the custominfo is == %s", strZoneID.c_str(), strCustomInfo.c_str()); 

	if (isHave) {  
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrCustomInfo, jstrZoneId);  
	} else  {  
		CCLOG("[error]FaceBookSDK::onPay, get jni method fail!!!");  
	} 
#endif  
}

void FaceBookSDK::onSDKPayBack(int iRetCode){
	CCLOG("[trace]FaceBookSDK::onSDKPayBack, the iRetCode ==== %d", iRetCode); 

	P_SHOP_CTRL->setRMBBuyLoading(false);

	if(iRetCode == 1){  //支付成功
		COMMON_DIALOG->show(TextStr(TEXT_SDK_BUY_SUCCESS));
	}else if(iRetCode == 2){  //支付失败
		COMMON_DIALOG->show(TextStr(TEXT_SDK_BUY_FAIL));
	}
}

void FaceBookSDK::onShared(std::string name, std::string link, std::string pic, std::string caption, std::string desc)
{
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
	//路径必须要给出class的名字，但不能带有.java的后缀
	JniMethodInfo methodInfo; //用于获取函数体  
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo, "com/game/tgame/JavaHelper", "onShared", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	CCLOG("[facebook]=====[trace]FaceBookSDK::onShared,start to call java onShared");

	jstring jstrName = methodInfo.env->NewStringUTF(name.c_str());
	jstring jstrLink = methodInfo.env->NewStringUTF(link.c_str());
	jstring jstrPic = methodInfo.env->NewStringUTF(pic.c_str());
	jstring jstrCaption = methodInfo.env->NewStringUTF(caption.c_str());
	jstring jstrDesc = methodInfo.env->NewStringUTF(desc.c_str());

	if (isHave) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrName, jstrLink, jstrPic, jstrCaption, jstrDesc);
	}
	else  {
		CCLOG("[error]FaceBookSDK::onShared,get jni method fail!!!");
	}
#endif
}

void FaceBookSDK::onFeedBack(std::string extraInfo)
{
#if( CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)  
	//路径必须要给出class的名字，但不能带有.java的后缀
	JniMethodInfo methodInfo; //用于获取函数体  
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo, "com/game/tgame/JavaHelper", "onFeedBack", "(Ljava/lang/String;)V");
	CCLOG("[facebook]=====[trace]FaceBookSDK::onFeedBack,start to call java onFeedBack");

	jstring jstrExtraInfo = methodInfo.env->NewStringUTF(extraInfo.c_str());
	
	if (isHave) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrExtraInfo);
	}
	else  {
		CCLOG("[error]FaceBookSDK::onFeedBack,get jni method fail!!!");
	}
#endif
}

void FaceBookSDK::onFaceBookShared(int iret)
{
	CCLOG("[trace]FaceBookSDK::onFaceBookShared, the iRetCode ==== %d", iret);

	if (iret == 1){  //分享成功
		COMMON_DIALOG->show(TextStr(TEXT_SDK_SHARE_SUCCESS));
		P_TASK_CTRL->sendTriggerDailyTaskRqst(CS_CMD_DAILYTASK_TRIGGER);
	}
	else if (iret == 2){  //分享失败
		COMMON_DIALOG->show(TextStr(TEXT_SDK_SHARE_FAILED));
	}
}
