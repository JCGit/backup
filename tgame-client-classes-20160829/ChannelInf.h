#ifndef __ChannelInf_H_
#define __ChannelInf_H_

#include <string>

/*
接口定义参考：proj.android\chameleon\chameleoncb\include\chameleoncb\ChameleonChannelAPI.h
win32下函数用空实现
android下的实现写在proj.android\jni\hellocpp\ChannelInfImp.cpp里

*/

class ChannelInf {
public:
    static void Init();
    static void Login();
    static std::string GetChannelName();

    static int Buy(const std::string & orderId, 
        const std::string & uidInGame, 
        const std::string & userNameInGame, 
        const std::string & serverId, 
        const std::string & productName, 
        const std::string & productId,
        const std::string & payInfo,
        int productCount,
        int realPayMoney);

    static std::string GetPayToken();
    static std::string GetSDKUid();
    static bool submitPlayerInfo(const std::string & roleId, 
        const std::string & roleName, 
        const std::string & roleLevel, 
        int zoneId,
        const std::string & zoneName);
    static void Logout();

    static void onLoginRsp(const std::string & strInfo);

    static void showToolbar(bool isVisible);

    static void doRunProtocol(int id, const std::string & protocol, const std::string & message);

	/**
     * 该平台是否支持账号切换
     * @return {bool}, 是否支持账号切换
     */
	static bool isSupportSwtichAccount();

	/**
     * 发起切换账户的请求
     * @param {int} id, 标识该请求的id
     * @return {int}, 0表示异步请求成功，否则表示失败
     */
	static int switchAccount(int id);
};

#endif //__ChannelInf_H_

