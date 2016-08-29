#include "ChannelInfoCallback.h"
#include "cocos2d.h"
#include "game/login/LoginControl.h"
#include "game/login/LoginObj.h"
#include "game/UserObj.h"
#include "game/login/ChooseRolePanel.h"
#include "basic/QNetMgr.h"
#include "shop/ShopControl.h"
#include "evtlog/EvtLog.h"
#include "GameInf.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "ChannelInf.h"
#include "login/UserGoldControl.h"
#include "login/TencentLoginPanel.h"


std::string ChannelInfoCallback::m_strSDKLoginInfo = "";
bool ChannelInfoCallback::m_bIsExitQQmsdk = false;
std::string ChannelInfoCallback::m_stChannelName = "";
bool ChannelInfoCallback::m_bIsAnySdk = false;

void ChannelInfoCallback::onLogin(int code, const std::string * info) {
    m_strSDKLoginInfo = ""; //先重置
    CCLOG("[trace]ChannelInfoCallback::onLogin, code=[%d]", code );
    P_EVT_LOG->log(EVT_LOG_SDK_LOGIN_END, code );

	//启动初始化
	
	
    //CCTextureCache::sharedTextureCache()->reloadAllTextures();
    if(25 == code) {
        //腾讯等平台特殊处理
        if(ChannelInf::GetChannelName() == "qqmsdk") {
            //腾讯平台
            P_LOGIN_CTRL->getChooseSvrPanel()->closeLoading();
            P_LOGIN_CTRL->getTencentLoginPanel()->open();
            CCTextureCache::sharedTextureCache()->reloadAllTextures();
        }
        /*
        if(P_LOGIN_CTRL->getTencentLoginPanel()->isSelQQ() ) {
            CCLOG("[trace]ChannelInfoCallback::onLogin, ChannelInf::doRunProtocol, qqmsdk_setplat, qq" );
            ChannelInf::doRunProtocol(0, "qqmsdk_setplat", "qq");
            //重新登陆
            ChannelInf::Login();
        } else if(P_LOGIN_CTRL->getTencentLoginPanel()->isSelWX() ) {
            ChannelInf::doRunProtocol(0, "qqmsdk_setplat", "wx");
            //重新登陆
            ChannelInf::Login();
        } */
        return;
    }

    if(NULL == info) {
        CCLOG("[error]ChannelInfoCallback::onLogin, info is null!");
        P_LOGIN_CTRL->getChooseSvrPanel()->closeLoading();
        //COMMON_DIALOG->show(TextStr(TEXT_SDK_LOGIN_FAIL));
        return;
    }
    CCLOG("[trace]ChannelInfoCallback::onLogin, info [%s]", info->c_str());
	rapidjson::Document json;
	std::string json_str = *info;
	json.Parse<0>(json_str.c_str());

	if (json.HasParseError()) {
		CCLOG("[trace]ChannelInfoCallback::onLogin channel info parse failed, info [%s]", info->c_str());
	} 
	/*else if(json.HasMember("channel") && json["channel"].IsString()) {
		m_stChannelName = json["channel"].GetString();
		
	}*/
	else if (json.HasMember("channel") && json["channel"].IsString()){
		if (json["channel"].GetString() == "nd91"){
			string stAnySdkChannelName = json["others"].GetString();
			m_stChannelName = json["others"].GetString(); //anysdk 用到的
			P_USER_OBJ->getMutableUserInfo()->openID = json["token"].GetString();
			P_LOGIN_CTRL->doAfterSDKInited();  //这里要重新去拉一次dir，否则要出问题
			m_bIsAnySdk = true;
		}else{
			m_stChannelName = json["channel"].GetString();
			m_bIsAnySdk = false;

		}

	} else{
		CCLOG("[trace]ChannelInfoCallback::onLogin channel is not found!", info->c_str());
	 
	}


    
    //2015-1-27,修改为自动登陆sdk
    P_LOGIN_CTRL->getChooseSvrPanel()->closeLoading();
    if (code == 0) {
        m_strSDKLoginInfo = *info;
        P_LOGIN_CTRL->getTencentLoginPanel()->close();
        CCTextureCache::sharedTextureCache()->reloadAllTextures();
    } else {
	    CCLOG("[error]ChannelInfoCallback::onLogin, Fail to login %d", code);
    }
}

void ChannelInfoCallback::onSwitchAccount(int code, const std::string * info){
	CCLOG("[trace]ChannelInfoCallback::onSwitchAccount, code=[%d]", code );

	CCLOG("[trace]ChannelInfoCallback::onSwitchAccount, info [%s]", info->c_str());
}

void ChannelInfoCallback::preAccountSwitch(){
	CCLOG("[trace]ChannelInfoCallback::preAccountSwitch");
}

void ChannelInfoCallback::afterAccountSwitch(int code, const std::string * info){
	CCLOG("[trace]ChannelInfoCallback::afterAccountSwitch, code=[%d]", code );

	CCLOG("[trace]ChannelInfoCallback::afterAccountSwitch, info [%s]", info->c_str());

	onSDKLogout(203);

	if(NULL == info) {
        CCLOG("[error]ChannelInfoCallback::afterAccountSwitch, info is null!");
        P_LOGIN_CTRL->getChooseSvrPanel()->closeLoading();
        return;
    }
    CCLOG("[trace]ChannelInfoCallback::afterAccountSwitch, info [%s]", info->c_str());
	rapidjson::Document json;
	std::string json_str = *info;
	json.Parse<0>(json_str.c_str());

	if (json.HasParseError()) {
		CCLOG("[trace]ChannelInfoCallback::afterAccountSwitch channel info parse failed, info [%s]", info->c_str());
	}else if (json.HasMember("others") && json["others"].IsString()
		&& json.HasMember("token") && json["token"].IsString()){
		string stAnySdkChannelName = json["others"].GetString();
		m_stChannelName = json["others"].GetString(); //anysdk 用到的
		P_USER_OBJ->getMutableUserInfo()->openID = json["token"].GetString();
		P_LOGIN_CTRL->doAfterSDKInited();  //这里要重新去拉一次dir，否则要出问题
		m_bIsAnySdk = true;
	}else if (json.HasMember("channel") && json["channel"].IsString()){
		m_stChannelName = json["channel"].GetString();
		m_bIsAnySdk = false;
	}
    
    //2015-1-27,修改为自动登陆sdk
    P_LOGIN_CTRL->getChooseSvrPanel()->closeLoading();
    if (code == 0) {
        m_strSDKLoginInfo = *info;
        P_LOGIN_CTRL->getTencentLoginPanel()->close();
        CCTextureCache::sharedTextureCache()->reloadAllTextures();
    } else {
	    CCLOG("[error]ChannelInfoCallback::afterAccountSwitch, Fail to login %d", code);
    }
}

void ChannelInfoCallback::doGameLogin() {
    CCLOG("[trace]ChannelInfoCallback::doGameLogin");
    if(m_strSDKLoginInfo.size() == 0) {
        //重新登陆
        ChannelInf::Login();
        return;
    }
    P_LOGIN_CTRL->getLoginScreenView()->onLoginInfoDone(m_strSDKLoginInfo);
}


void ChannelInfoCallback::resetSDKLoginInfo() {
    CCLOG("[trace]ChannelInfoCallback::resetSDKLoginInfo");
    m_strSDKLoginInfo = "";
}


void ChannelInfoCallback::onChargeCallback(int code) {
    CCLog("[trace]ChannelInfoCallback::onChargeCallback,pay callback [%d]", code);
    // error handling
}


void ChannelInfoCallback::onBuyCallback(int code) 
{
    CCLOG("[trace]ChannelInfoCallback::onBuyCallback,code [%d]", code);
    if(16 == code) {
        //特殊处理，重新发起购买
        CCLOG("[trace]ChannelInfoCallback::onBuyCallback, need re send buy rqst");
        P_USER_GOLD_CTRL->doReSendBuy();
        return;
    }
    P_SHOP_CTRL->setRMBBuyLoading(false);
    //chameleoncb\include\chameleoncb\ChameleonErrorCode.h定义
    /*
    CHAMELEON_ERR_PAY_FAIL = 11,
    CHAMELEON_ERR_PAY_CANCEL = 12,
    CHAMELEON_ERR_PAY_IN_PROGRESS = 13,
    */
    if(13 == code) {
        COMMON_DIALOG->show(TextStr(TEXT_SDK_BUY_SUCCESS));
    } else if(11 == code || 12 == code) {
        COMMON_DIALOG->show(TextStr(TEXT_SDK_BUY_FAIL));
    }
}



void ChannelInfoCallback::onInited(int ret, bool isDebug) {
    //ChannelInf::Init 的回掉
    CCLog("[trace]ChannelInfoCallback::onInited,ret=[%d],isDebug=%d", ret, isDebug);
	//P_LOGIN_CTRL->doAfterSDKInited();
}


void ChannelInfoCallback::onCyouSDKLogin(bool isSuccess, const std::string& strSDKJson) {
    /* //Demo:
    {"data":"{\"validateInfo\":\"50d2f67deb4b7ba8ab58f2e6cda45aea4d1c98e8ccb0a24d2621455ec3bf4d1e7e697024013a07be
    5f3d42261eef9070fa8d06debd545b0244bbe6c5c19b7c9c86f7861714f3a4b9e608a8358a2b38961918ba5aeff0ecd39bdc1a8702455
    193636d2aa5934080f23939e101c69c3a774e71aeb77feadc1ba932ae154fd111666e01269027a4f73df3d275f5586b9e79715fcf8d74
    76f1dd9383fcabef6ca117fc66c41fb461909d9d2fd1995e15ccdd86bb8627f2bf97de58c9e686ed033e7b1019a467f7515d8e308105c
    989f48c9cc407b977050ef529f2591ef75eeee466b7926cb8c10d21f54c033ffb2d41b31da45b71ec235e932908ad582f117ac1cc5600
    7d08338d81f3b9a6b8dea68a00bef7257f0321e55f470653fdd9e7c1a82f87cafbfb53b9f1ea6a7706f2fe848ac1\"}",
    "message":"登录成功","state_code":300,"channel_id":"4001","opcode":"10001"}
    */
    CCLog("[trace]ChannelInfoCallback::onCyouSDKLogin,isSuccess=[%d],strSDKJson=[%s]", isSuccess, strSDKJson.c_str());

    P_EVT_LOG->log(EVT_LOG_SDK_LOGIN_END, isSuccess );

    P_LOGIN_CTRL->getChooseSvrPanel()->closeLoading();
    if (isSuccess) {
        P_LOGIN_CTRL->getLoginScreenView()->onLoginInfoDone(strSDKJson); //赋值的是loginInfo字段
    } else {
        CCLOG("Fail to login");
    }
}



void ChannelInfoCallback::onCyouSDKPayResult(const std::string& strRetJson) {
    CCLOG("[trace]ChannelInfoCallback::onCyouSDKPayResult,strRetJson=[%s]", strRetJson.c_str());
    //示例：{"data":{"order_id":"a96ba6fb0a584291a1153ca1b41b277f","uid":"513005460237223","price":"0.01"},"state_code":100" msg":"支付成功"}
    //100支付成功，101支付失败，102用户取消支付，103缺少参数，104正在支付，105订单处理中，

    rapidjson::Document jsonDoc;
    jsonDoc.Parse<0>(strRetJson.c_str());
    if(jsonDoc.HasParseError()) {
        //P_EVT_LOG->log(EVT_LOG_GET_SVR_LIST, "JsonError");
        CCLOG("[error]ChannelInfoCallback::onCyouSDKPayResult,json parse error [%s]", jsonDoc.GetParseError());
        return;
    }
    int iRetStatCode = -1;
    std::string strRetMsg;
    if(jsonDoc.HasMember("state_code") && jsonDoc["state_code"].IsInt() ) {
        iRetStatCode = jsonDoc["state_code"].GetInt();
    }
    if(jsonDoc.HasMember("message") && jsonDoc["message"].IsString() ) {
        strRetMsg = jsonDoc["message"].GetString();
    }
    if(strRetMsg.length() <= 0) {
        return;
    }
    CCLOG("[trace]retcode[%d],msg[%s]", iRetStatCode, strRetMsg.c_str() );
    //COMMON_DIALOG->show(strRetMsg);
}


void ChannelInfoCallback::onSDKLogout(int iRetCode) {
    CCLOG("[trace]ChannelInfoCallback::onSDKLogout,ret %d", iRetCode);
    //cyou 200成功，202 失败，201取消注销，203切换账号 
    COMMON_DIALOG->closeLoading();
    if(200 == iRetCode || 203 == iRetCode) {
        if("qqmsdk" == ChannelInf::GetChannelName()) {
            //如果是腾讯平台，回到登陆界面后自动打开登陆选择面板
            m_bIsExitQQmsdk = true;
        } else {
            m_bIsExitQQmsdk = false;
        }
        P_GAME_INF->onClickClose();
    }
}


void ChannelInfoCallback::onSDKExit(int iRetCode) {
    CCLOG("[trace]ChannelInfoCallback::onSDKExit,ret %d", iRetCode);
    P_GAME_INF->doSDKExit(iRetCode);
}

bool ChannelInfoCallback::isExitQQmsdk() {
    return m_bIsExitQQmsdk;
}
void ChannelInfoCallback::resetIsExitQQmsdk() {
    m_bIsExitQQmsdk = false;
}

std::string ChannelInfoCallback::getChannelName() {
	return m_stChannelName;
}
