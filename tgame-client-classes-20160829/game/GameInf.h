#ifndef __GameInf_H_
#define __GameInf_H_

#include "vlib/base/CVSingleton.h"
#include "cocos2d.h"

USING_NS_CC;



/*
给base模块提供调用接口
*/

class GameInf : public CCObject {
public:
    static int GetZoneID(); //获取大区号
    static void DoConnectionClose(); //处理连接断开

public:
    GameInf() {
    }
    void showHttpFail(); //显示http连接失败

    void doQuickLoginFail();
    void doQuickLoginSuccess();
	
    //android inf
    //发送系统通知
    void sendSysNotify(const std::string& strTitle, const std::string& strTxt, const int iDelaySeconds);

    void doSDKLogout();
    void onClickClose();  
    std::string getUmengDeviceToken();
    void doSDKExit(int iRetCode);

    const char* getDirURL(); //获取dir网址

    void doRestartGame();
private:
    void showReconnectTip(); //提示重新连接
    void onClickToReconnect(); 
    void doAfterConnect(float dt);

    void doAfterTimeout(float dt); //超时

};


#define P_GAME_INF CVLib::CVSingleton<GameInf>::Instance()

#endif //__GameInf_H_
