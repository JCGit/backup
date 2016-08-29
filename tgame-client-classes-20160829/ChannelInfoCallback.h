#ifndef __ChannelInfoCallback_H_
#define __ChannelInfoCallback_H_

#include <string>

class ChannelInfoCallback {
public:
    static void onLogin(int code, const std::string * loginInfo);
    static void onChargeCallback(int code);
    static void onBuyCallback(int code);
    static void onInited(int ret, bool isDebug);

	static void onSwitchAccount(int code, const std::string * loginInfo);
	static void preAccountSwitch();
	static void afterAccountSwitch(int code, const std::string * loginInfo);

    //cyou
    static void onCyouSDKLogin(bool isSuccess, const std::string& strSDKJson);
    static void onCyouSDKPayResult(const std::string& strRetJson);

    //common
    static void onSDKLogout(int iRetCode);
    static void onSDKExit(int iRetCode);

    static void doGameLogin(); //���Ե�����Ϸ��½��
    static void resetSDKLoginInfo(); //
    static bool isExitQQmsdk();
    static void resetIsExitQQmsdk();
	static bool isAnySdk(){
		return m_bIsAnySdk;
	};
	static std::string getChannelName();
private:
    static std::string m_strSDKLoginInfo;
    static bool m_bIsSDKInited;
    static bool m_bIsExitQQmsdk; //�Ƿ��Ǵ���������˳���Ѷsdk

	static std::string m_stChannelName; // such as "uc"
	static bool m_bIsAnySdk;
};

#endif //__ChannelInfoCallback_H_

