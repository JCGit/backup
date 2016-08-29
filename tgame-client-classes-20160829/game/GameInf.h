#ifndef __GameInf_H_
#define __GameInf_H_

#include "vlib/base/CVSingleton.h"
#include "cocos2d.h"

USING_NS_CC;



/*
��baseģ���ṩ���ýӿ�
*/

class GameInf : public CCObject {
public:
    static int GetZoneID(); //��ȡ������
    static void DoConnectionClose(); //�������ӶϿ�

public:
    GameInf() {
    }
    void showHttpFail(); //��ʾhttp����ʧ��

    void doQuickLoginFail();
    void doQuickLoginSuccess();
	
    //android inf
    //����ϵͳ֪ͨ
    void sendSysNotify(const std::string& strTitle, const std::string& strTxt, const int iDelaySeconds);

    void doSDKLogout();
    void onClickClose();  
    std::string getUmengDeviceToken();
    void doSDKExit(int iRetCode);

    const char* getDirURL(); //��ȡdir��ַ

    void doRestartGame();
private:
    void showReconnectTip(); //��ʾ��������
    void onClickToReconnect(); 
    void doAfterConnect(float dt);

    void doAfterTimeout(float dt); //��ʱ

};


#define P_GAME_INF CVLib::CVSingleton<GameInf>::Instance()

#endif //__GameInf_H_
